// Copyright Villains, Inc. All Rights Reserved.


#include "BlueprintCameraModifier.h"

UBlueprintCameraModifier::UBlueprintCameraModifier()
{
	TransitionTimeIn = 0.01f;
	TransitionTimeOut = 0.01f;
	DefaultPov = FCameraInfo();
}

bool UBlueprintCameraModifier::ApplyModifierIn_Implementation(ACharacter* Context)
{
	return true;
}

bool UBlueprintCameraModifier::ApplyModifierOut_Implementation(ACharacter* Context)
{
	return true;
}

void UBlueprintCameraModifier::Initialize()
{
	if (GetWorld())
	{
		Character = GetWorld()->GetFirstPlayerController()->GetCharacter();
		if (!Character)
		{
			bInitialized = false;
			return;
		}
	}
	BeginPlay(Character);
	
	if (!IsValid(CustomPOV))
	{
		bInitialized = false;
		return;
	}
	
	//temporary bug fix, comment out and set AimPOV.Rotation to 0, 0, 0 and any other values different from 0 to see the bug
	if (CustomPOV->POV.Rotation == FRotator::ZeroRotator)
	{
		CustomPOV->POV.Rotation.Yaw = 0.0001f;
	}

	bInitialized = true;
}

void UBlueprintCameraModifier::BeginPlay_Implementation(ACharacter* Context)
{
}

void UBlueprintCameraModifier::AddedToCamera(APlayerCameraManager* Camera)
{
	Super::AddedToCamera(Camera);
	OnEnable();
}

bool UBlueprintCameraModifier::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);
	if (!IsValid(CustomPOV))
	{
		return false;
	}
	if (ApplyModifierIn(Character))
	{
		ApplyCameraTransition(CustomPOV->POV, TransitionTimeIn, InOutPOV, DeltaTime);
	}
	else if (ApplyModifierOut(Character))
	{
		if (ApplyCameraTransition(DefaultPov, TransitionTimeOut, InOutPOV, DeltaTime))
		{
			DisableModifier();
		}
	}
	else DisableModifier();
	
	return bHasPriority;
}


