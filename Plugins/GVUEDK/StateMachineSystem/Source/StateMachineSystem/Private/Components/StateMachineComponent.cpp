// Copyright Villains, Inc. All Rights Reserved.


#include "Components/StateMachineComponent.h"

#include "NativeGameplayTags.h"
#include "Components/MultiStateMachineComponent.h"


UStateMachineComponent::UStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsRunning = false;
	bInitialized = false;
	EntryStateClass = nullptr;
	CurrentState = nullptr;
	PreviousState = nullptr;
	Context = nullptr;
	EntryState = nullptr;
	bPaused = false;
}

UStateBase* UStateMachineComponent::ChangeState(const FGameplayTag NextState)
{
	if (!bInitialized || bPaused) return nullptr;

	UStateBase* NextStateInstance = nullptr; 
	if (TryGetState(NextState, NextStateInstance))
	{
		if (NextStateInstance->GetIsRunning())
		{
			UE_LOG(LogStateMachine, Warning, TEXT("%s already running."), *NextState.ToString());
			return nullptr;
		}
		
		//if this state machine is running under a multi-state machine,
		//we will check the negators of the next state to know if it can be loaded
		if (IsValid(MultiStateMachine))
		{
			if (IsNegated(NextStateInstance))
			{
				return nullptr;
			}
		}
		
		PreviousState = CurrentState;
		CurrentState->Exit();
		CurrentState = NextStateInstance;
		NextStateInstance->Enter();
	}
	return NextStateInstance;
}

bool UStateMachineComponent::TryGetState(const FGameplayTag StateTag, UStateBase*& OutState) const
{
	if (bInitialized)
	{
		if (StatesMap.Contains(StateTag))
		{
			OutState = StatesMap[StateTag];
			return true;
		}
		OutState = nullptr;
		UE_LOG(LogStateMachine, Warning, TEXT("State Tag: %s not found"), *StateTag.ToString());
		return false;
	}
	OutState = nullptr;
	UE_LOG(LogStateMachine, Warning, TEXT("Component not initialized"));
	return false;
}

UStateBase* UStateMachineComponent::GetState(const FGameplayTag StateTag) const
{
	if (bInitialized)
	{
		if (StatesMap.Contains(StateTag))
		{
			return StatesMap[StateTag];
		}
		UE_LOG(LogStateMachine, Warning, TEXT("State Tag: %s not found"), *StateTag.ToString());
		return nullptr;
	}
	UE_LOG(LogStateMachine, Warning, TEXT("Component not initialized"));
	return nullptr;
}

void UStateMachineComponent::Initialize()
{
	Context = GetOwner();

	//States initialization
	StatesMap = TMap<FGameplayTag, UStateBase*>();
	
	if (IsValid(EntryStateClass))
	{
		EntryState = NewObject<UStateBase>(this, EntryStateClass);
		EntryState->Initialize(Context, this);
		StatesMap.Add(EntryState->StateTag, EntryState);
		UE_LOG(LogStateMachine, Log, TEXT("State %s created"), *EntryState->GetName());
	}
	else
	{
		UE_LOG(LogStateMachine, Warning, TEXT("Missing EntryState, state machine will not work"));
		bInitialized = false;
		return;
	}
	
	for (auto Element : StateClasses)
	{
		if (!IsValid(Element))
		{
			UE_LOG(LogStateMachine, Warning, TEXT("Invalid State"));
			continue;
		}
		UStateBase* StateInstance = NewObject<UStateBase>(this, Element);
		if (StatesMap.Contains(StateInstance->GetStateTag()))
		{
			UE_LOG(LogStateMachine, Warning, TEXT("State Tag: %s already exists, state: %s will not work"), *StateInstance->GetStateTag().ToString(), *StateInstance->GetName());
			continue;
		}
		StateInstance->Initialize(Context, this);
		StatesMap.Add(StateInstance->GetStateTag(), StateInstance);
		UE_LOG(LogStateMachine, Log, TEXT("State %s created"), *StateInstance->GetName());
	}
	
	bInitialized = !StatesMap.IsEmpty();

	// if state machine initialized correctly run the entry point
	if (bInitialized)
	{
		bIsRunning = true;
		CurrentState = EntryState;
		CurrentState->Enter();
	}
}

bool UStateMachineComponent::IsNegated(const UStateBase* StateToCheck) const
{
	for (auto Element : MultiStateMachine->StateMachinesMap)
	{
		// Skip if the element is the same state machine as the current one
		if (Element.Key == GetStateMachineTag())
		{
			continue;
		}
		for (auto Tag : StateToCheck->Negators)
		{
			if (Tag == Element.Value->GetCurrentStateTag())
			{
				UE_LOG(LogStateMachine, Log, TEXT("%s cannot run because it is negated by %s in state machine %s"), 
					*StateToCheck->GetStateTag().ToString(), *Element.Value->GetCurrentState()->GetStateTag().ToString(), *Element.Key.ToString());
				return true;
			}
		}
	}
	return false;
}

UStateBase* UStateMachineComponent::HandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value)
{
	if (!bInitialized || bPaused)
	{
		return nullptr;
	}
	const FGameplayTag HandleInputResult = CurrentState->HandleInput(InputActionTag, Value);
	if (HandleInputResult != GetCurrentStateTag())
	{
		return ChangeState(HandleInputResult);
	}
	return nullptr;
}

UStateBase* UStateMachineComponent::InterruptCurrentState(const FGameplayTag Interrupter, const bool bForceEntryState)
{
	if (!bInitialized)
	{
		return nullptr;
	}

	auto GoToEntryState = [this]()
	{
		PreviousState = CurrentState;
		CurrentState->Exit();
		CurrentState = EntryState;
		EntryState->Enter();
		return EntryState;
	};

	const FGameplayTag NextStateTag = CurrentState->Interrupt(Interrupter);

	if (bForceEntryState)
	{
		return GoToEntryState();
	}

	// if next state tag is empty, not valid, not in the states map or is the same of the current running state, we will return to the entry state by default
	if (!NextStateTag.IsValid() || !StatesMap.Contains(NextStateTag) || NextStateTag == CurrentState->StateTag)
	{
		if (CurrentState == EntryState)
        {
            UE_LOG(LogStateMachine, Warning, TEXT("Entry state %s interrupted and returned to itself, this should not happen, check your state machine logic"), *EntryState->GetName());
        }
		return GoToEntryState();
	}

	return ChangeState(NextStateTag);
}

void UStateMachineComponent::Pause(const bool bResetToEntryState)
{
	bPaused = true;
	if (bResetToEntryState)
	{
		InterruptCurrentState(FGameplayTag::EmptyTag, true);
	}
}

void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bInitialized && !bPaused)
	{
		CurrentState->Update(DeltaTime);
	}
}
