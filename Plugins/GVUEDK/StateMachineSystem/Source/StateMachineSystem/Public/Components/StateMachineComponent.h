// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "State/StateBase.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogStateMachine, All, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), EditInlineNew)
class STATEMACHINESYSTEM_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:

	friend class UMultiStateMachineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	FGameplayTag StateMachineTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "State Machine", DisplayName = "Entry State", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStateBase> EntryStateClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "State Machine", DisplayName = "States", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UStateBase>> StateClasses;

	UPROPERTY(BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UStateBase*> StatesMap;

	UPROPERTY(BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	UStateBase* EntryState;

	UPROPERTY(BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	UStateBase* CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
	UStateBase* PreviousState;

	UPROPERTY()
	AActor* Context;

	UPROPERTY()
	UMultiStateMachineComponent* MultiStateMachine;
	
	bool bIsRunning;
	bool bInitialized;
	bool bPaused;

public:
	
	UStateMachineComponent();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetStateMachineTag() const { return StateMachineTag; }
	
	UFUNCTION(BlueprintCallable)
	UStateBase* ChangeState(const FGameplayTag NextState);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool TryGetState(const FGameplayTag StateTag, UStateBase*& OutState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStateBase* GetState(const FGameplayTag StateTag) const;
	
	UFUNCTION(Blueprintable, BlueprintPure)
	UStateBase* GetCurrentState() const { return CurrentState; }
	
	UFUNCTION(Blueprintable, BlueprintPure)
	UStateBase* GetPreviousState() const { return PreviousState; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetCurrentStateTag() const { return CurrentState->StateTag; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTag GetPreviousStateTag() const { return PreviousState->StateTag; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsRunning() const { return bIsRunning; }

	/**
     *  Returns the new state entered by this state machine after handling the input if any, otherwise returns nullptr
     */
	UFUNCTION(BlueprintCallable)
	UStateBase* HandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value);
	
	UStateBase* InterruptCurrentState(const FGameplayTag Interrupter, const bool bForceEntryState = false);

	UFUNCTION(BlueprintCallable)
	void Pause(const bool bResetToEntryState = false);

	UFUNCTION(BlueprintCallable)
	void UnPause() { bPaused = false; }
	
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	
	void Initialize();

	bool IsNegated(const UStateBase* StateToCheck) const;
	
};
