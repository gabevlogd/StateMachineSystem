// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TransitionEvents.generated.h"

class UStateBase;
struct FInputActionValue;
struct FGameplayTag;


UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class STATEMACHINESYSTEM_API UTransitionEvents final : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, Category = "State")
	bool TransitionCondition(AActor* StateContext, const FGameplayTag& InputAction, const FInputActionValue& Value, const UStateBase* CurrentState, const FGameplayTag& NextState);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "State")
	void OnTransition(AActor* StateContext, const FGameplayTag& InputAction, const FInputActionValue& Value, const UStateBase* CurrentState, const FGameplayTag& NextState);
}; 
