// Copyright Villains, Inc. All Rights Reserved.


#include "State/StateBase.h"
#include "TransitionEvents.h"


void UStateBase::Initialize(AActor* Context, UStateMachineComponent* StateMachine)
{
	if (!IsValid(Context))
	{
		bInitialized = false;
		UE_LOG(LogStateBase, Warning, TEXT("State %s: Context is not valid"), *GetName());
		return;
	}
	StateContext = Context;
	RelativeStateMachine = StateMachine;
	bInitialized = true;
	NativeOnInitialize(Context);
	OnInitialize(Context);
}

void UStateBase::Enter()
{
	if (!bInitialized)
	{
		return;
	}
	//UE_LOG(LogStateBase, Log, TEXT("State %s: Enter"), *GetName());
	bIsRunning = true;
	NativeOnEnter();
	OnEnter();
}

void UStateBase::Update(const float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	NativeOnUpdate(DeltaTime);
	OnUpdate(DeltaTime);
}

void UStateBase::Exit()
{
	if (!bInitialized)
	{
		return;
	}
	//UE_LOG(LogStateBase, Warning, TEXT("State %s: Exit"), *GetName());
	bIsRunning = false;
	NativeOnExit();
	OnExit();
}

FGameplayTag UStateBase::HandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return StateTag;
	}

	// Check if the input action tag is in the transitions map, if so, return the corresponding state tag
	for (auto Element : Transitions)
	{
		if (Element.Key == InputActionTag)
		{
			if (IsValid(Element.Value.TransitionEvents))
			{
				if (!Element.Value.TransitionEvents->TransitionCondition(StateContext, InputActionTag, Value, this, Element.Value.ToState))
				{
					//UE_LOG(LogStateBase, Warning, TEXT("Transition Condition failed for transition %s"), *Element.Value.TransitionEvents->GetName());
					break;
				}
				Element.Value.TransitionEvents->OnTransition(StateContext, InputActionTag, Value, this, Element.Value.ToState);
			}
			return Element.Value.ToState;
		}
	}

	// If the InputActionTag is not in the transitions map, call the native and blueprint handle input events
	FGameplayTag Result = NativeOnHandleInput(InputActionTag, Value);
	if (Result.IsValid() && Result != StateTag)
	{
		return Result;
	}

	
	Result = OnHandleInput(InputActionTag, Value);
	if (Result.IsValid() && Result != StateTag)
	{
		return Result;
	}

	return StateTag;
}

FGameplayTag UStateBase::Interrupt(const FGameplayTag Interrupter)
{
	if (!bInitialized)
	{
		return StateTag;
	}
	//UE_LOG(LogStateBase, Error, TEXT("State %s: Interrupting"), *GetName());
	FGameplayTag Result = NativeOnInterrupt(Interrupter);
	if (Result.IsValid() && Result != StateTag)
	{
		return Result;
	}

	Result = OnInterrupt(Interrupter);
	if (Result.IsValid() && Result != StateTag)
	{
		return Result;
	}

	return StateTag;
}
