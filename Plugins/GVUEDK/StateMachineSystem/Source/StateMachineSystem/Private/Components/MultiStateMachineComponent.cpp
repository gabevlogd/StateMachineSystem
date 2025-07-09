// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MultiStateMachineComponent.h"
#include "Components/StateMachineComponent.h"


UMultiStateMachineComponent::UMultiStateMachineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bInitialized = false;
	bPaused = false;
}

void UMultiStateMachineComponent::ChangeState(const FGameplayTag NextState, const FGameplayTag RelativeStateMachine)
{
	if (!bInitialized || bPaused)
	{
		return;
	}

	if (!StateMachinesMap.Contains(RelativeStateMachine))
	{
		UE_LOG(LogMultiStateMachine, Warning, TEXT("Invalid State Machine tag"));
		return;
	}

	UStateBase* NewState = StateMachinesMap[RelativeStateMachine]->ChangeState(NextState);
	// Check if the new state is valid and if it is, check if it has to interrupt other states of other state machines
	if (IsValid(NewState))
	{
		//InterruptionChain = TArray<UStateBase*>();
		//CheckStateToInterrupt(NewState);
	}
}

void UMultiStateMachineComponent::HandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value, const FGameplayTag RelativeStateMachine)
{
	if (!bInitialized || bPaused)
	{
		return;
	}

	if (!StateMachinesMap.Contains(RelativeStateMachine))
	{
		UE_LOG(LogMultiStateMachine, Warning, TEXT("Invalid state machine tag"));
		return;
	}

	UStateBase* NewState = StateMachinesMap[RelativeStateMachine]->HandleInput(InputActionTag, Value);
	// Check if the new state is valid and if it is, check if it has to interrupt other states of other state machines
	if (IsValid(NewState))
	{
		InterruptionChain = TArray<UStateBase*>();
		CheckStateToInterrupt(NewState);
	}
}

void UMultiStateMachineComponent::PauseAll(const bool bResetToEntryState)
{
	for (const auto Element : StateMachinesMap)
	{
		Element.Value->Pause(bResetToEntryState);
	}
	bPaused = true;
}

void UMultiStateMachineComponent::Pause(FGameplayTag StateMachineTag, const bool bResetToEntryState)
{
	if (!StateMachinesMap.Contains(StateMachineTag))
	{
		UE_LOG(LogMultiStateMachine, Warning, TEXT("Invalid state machine tag"));
		return;
	}

	StateMachinesMap[StateMachineTag]->Pause(bResetToEntryState);
}

void UMultiStateMachineComponent::UnPauseAll()
{
	for (const auto Element : StateMachinesMap)
	{
		Element.Value->UnPause();
	}
	bPaused = false;
}

void UMultiStateMachineComponent::UnPause(FGameplayTag StateMachineTag)
{
	if (!StateMachinesMap.Contains(StateMachineTag))
	{
		UE_LOG(LogMultiStateMachine, Warning, TEXT("Invalid state machine tag"));
		return;
	}

	StateMachinesMap[StateMachineTag]->UnPause();
}

void UMultiStateMachineComponent::CheckStateToInterrupt(const UStateBase* Interrupter)
{
	for (auto Element : StateMachinesMap)
	{
		// Check if the state machine is the same as the one of the interrupter
		if (Element.Key == Interrupter->GetRelativeStateMachine()->GetStateMachineTag())
        {
            continue;
        }

		if (Element.Value->GetCurrentState()->Interrupters.Contains(Interrupter->GetStateTag()))
		{
			UStateBase* OutState = Element.Value->InterruptCurrentState(Interrupter->GetStateTag());
			
			if (!IsValid(OutState))
			{
				continue;
			}
			
			if (!InterruptionChain.Contains(OutState))
			{
				InterruptionChain.Add(OutState);
				CheckStateToInterrupt(OutState);
			}
			else
			{
				UE_LOG(LogMultiStateMachine, Warning, TEXT("Infinite loop detected, check interrupters of state %s"), *Interrupter->GetName());
				return;
			}
		}
	}
}

void UMultiStateMachineComponent::Initialize()
{
	StateMachinesMap = TMap<FGameplayTag, UStateMachineComponent*>();
	for (const auto Element : StateMachines)
	{
		if (!IsValid(Element))
		{
			UE_LOG(LogMultiStateMachine, Warning, TEXT("Invalid State Machine found"));
			continue;
		}
		
		if (StateMachinesMap.Contains(Element->GetStateMachineTag()))
		{
			UE_LOG(LogMultiStateMachine, Warning, TEXT("State Machine Tag: %s already exists, associated state machine will not work"), *Element->GetStateMachineTag().ToString());
			continue;
		}

		StateMachinesMap.Add(Element->GetStateMachineTag(), Element);
		Element->MultiStateMachine = this;
		Element->BeginPlay();
		UE_LOG(LogMultiStateMachine, Log, TEXT("State Machine %s added to state machines map"), *Element->GetName());
	}

	bInitialized = !StateMachinesMap.IsEmpty();
}

void UMultiStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void UMultiStateMachineComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bInitialized)
	{
		return;
	}
	
	for (const auto Element : StateMachinesMap)
	{
		Element.Value->TickComponent(DeltaTime, TickType, ThisTickFunction);
	}
}

