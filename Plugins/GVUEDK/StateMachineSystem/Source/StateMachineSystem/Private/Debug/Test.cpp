// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/Test.h"

void UTest::NativeOnInitialize(AActor* Context)
{
	UE_LOG(LogTemp, Log, TEXT("Test State Initialized"));
}

FGameplayTag UTest::NativeOnHandleInput(const FGameplayTag InputActionTag, const FInputActionValue& Value)
{
	//do stuff
	return Super::NativeOnHandleInput(InputActionTag, Value);
}
