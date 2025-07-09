// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "UObject/Object.h"
#include "StateBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogStateBase, All, All);

class UTransitionEvents;

/**
 *  Data structure that holds the information needed to transition to a state
 */
USTRUCT(Blueprintable, BlueprintType)
struct FTransitionData
{
	GENERATED_BODY()

	/**
	 * The state tag to transition to
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transitions")
	FGameplayTag ToState;

	/**
	 * The transition event to call when transitioning (Optional)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Transitions")
	UTransitionEvents* TransitionEvents;	
};


UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class STATEMACHINESYSTEM_API UStateBase : public UObject
{
	GENERATED_BODY()

protected:
	friend class UMultiStateMachineComponent;
	friend class UStateMachineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	FGameplayTag StateTag;
	
	/**
	 * Transitions map where: the KEY is the InputActionTag that represents
	 * the input that triggers the transition,
	 * and the VALUE is the TransitionData that contains the tag of the state to transition to and
	 * the transition event to call when transitioning (Optional)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State")
	TMap<FGameplayTag, FTransitionData> Transitions;

	/**
     *  Array of tags that represent the states of other state machines that can interrupt this state
     */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> Interrupters;

	/** 
	 * List of tags that prevent this state from being activated 
 	 * if any other active state in a different state machine contains one of them.
 	 * 
 	 * Unlike Interrupters, which forcibly stop a running state, 
 	 * Negators are used to deny the activation of this state 
 	 * when conflicting conditions are already present.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> Negators;
	
	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	AActor* StateContext;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	UStateMachineComponent* RelativeStateMachine;
	
	/**
	 *  True if the state has been initialized correctly.
	 *  If false, the state will not run
	 */
	UPROPERTY(BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bInitialized;

	/**
	 *  True if the state is currently running in the relative state machine
	 */
	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	FGameplayTag GetStateTag() const { return StateTag; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	UStateMachineComponent* GetRelativeStateMachine() const { return RelativeStateMachine; }
	
protected:

	//C++ overridable events
	
	virtual void NativeOnInitialize(AActor* Context) {}
	
	virtual void NativeOnEnter() {}
	
	virtual void NativeOnUpdate(const float DeltaTime) {}
	
	virtual void NativeOnExit() {}

	/**
	 *  Returns the tag of the state to transition to (if no transition is needed, return the current state tag or FGameplayTag::EmptyTag)
	 */
	virtual FGameplayTag NativeOnHandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value) { return StateTag; }

	virtual FGameplayTag NativeOnInterrupt(const FGameplayTag Interrupter) { return StateTag; }

	//Blueprint events
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInitialize(AActor* Context);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnter();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdate(const float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnExit();

	/**
	 *  Returns the tag of the state to transition to (if no transition is needed, return the current state tag or FGameplayTag::EmptyTag)
	 */
	UFUNCTION(BlueprintImplementableEvent)
	FGameplayTag OnHandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	FGameplayTag OnInterrupt(const FGameplayTag Interrupter);

private:
	void Initialize(AActor* Context, UStateMachineComponent* StateMachine);
	void Enter();
	void Update(const float DeltaTime);
	void Exit();
	FGameplayTag HandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value);

	/**
     *  @return the tag of the state to transition to after the interruption (if the tag is not valid or the same as the interrupted state, the state to transition to will be the entry state)
     */
	FGameplayTag Interrupt(const FGameplayTag Interrupter);

#if WITH_EDITOR
	virtual bool ImplementsGetWorld() const override { return true; }
# endif

	virtual UWorld* GetWorld() const override { return StateContext->GetWorld(); }
};
