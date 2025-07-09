// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/StateBase.h"
#include "Test.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINESYSTEM_API UTest : public UStateBase
{
	GENERATED_BODY()

	virtual void NativeOnInitialize(AActor* Context) override;

	virtual FGameplayTag NativeOnHandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value) override;
};
