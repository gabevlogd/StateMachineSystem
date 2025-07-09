// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MultiStateMachineComponent.generated.h"

class UStateBase;
struct FInputActionValue;
DEFINE_LOG_CATEGORY_STATIC(LogMultiStateMachine, All, All);

class UStateMachineComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STATEMACHINESYSTEM_API UMultiStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	friend class UStateMachineComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Multi-State Machine", Instanced, meta = (AllowPrivateAccess = "true"))
	TArray<UStateMachineComponent*> StateMachines;

	UPROPERTY(BlueprintReadOnly, Category = "Multi-State Machine", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UStateMachineComponent*> StateMachinesMap;

	/**
     *  TArray used to store the states that was interrupted consecutively.
     *  This is used to check if an infinite loop is detected when checking for states to interrupt.
     *  If a state is already in the array, it means that it was already interrupted and we will not check it again.
     */
	TArray<UStateBase*> InterruptionChain;
	
	bool bInitialized;
	bool bPaused;

public:
	UMultiStateMachineComponent();

	/**
     *  Change the state of the state machine with the given tag
     */
	UFUNCTION(BlueprintCallable, Category = "Multi-State Machine")
	void ChangeState(const FGameplayTag NextState, const FGameplayTag RelativeStateMachine);

	/**
	 *  Returns the state machine component with the given tag
	 *  @param InputActionTag - The tag of the input action to handle
	 *  @param Value - The value of the input action
	 *  @param RelativeStateMachine - The tag of the state machine which has to handle the input
	 */
	UFUNCTION(BlueprintCallable, Category = "Multi-State Machine")
	void HandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value, const FGameplayTag RelativeStateMachine);

	UFUNCTION(BlueprintCallable, Category = "Multi-State Machine")
	void PauseAll(const bool bResetToEntryState = false);

	UFUNCTION(BlueprintCallable, Category = "Multi-State Machine")
	void Pause(FGameplayTag StateMachineTag, const bool bResetToEntryState = false);
	
	UFUNCTION(BlueprintCallable, Category = "Multi-State Machine")
	void UnPauseAll();

	UFUNCTION(BlueprintCallable, Category = "Multi-State Machine")
	void UnPause(FGameplayTag StateMachineTag);

private:

	void CheckStateToInterrupt(const UStateBase* Interrupter);

	void Initialize();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
