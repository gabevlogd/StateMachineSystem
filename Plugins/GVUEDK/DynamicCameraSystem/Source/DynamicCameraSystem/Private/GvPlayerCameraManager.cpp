// Copyright Villains, Inc. All Rights Reserved.


#include "GvPlayerCameraManager.h"
#include "GvCameraModifier.h"

void AGvPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeModifiers();
}

void AGvPlayerCameraManager::InitializeModifiers()
{
	CameraModifiers = TMap<FGameplayTag, UGvCameraModifier*>();
	for (UCameraModifier* Element : ModifierList)
	{
		if(UGvCameraModifier* CameraModifier = Cast<UGvCameraModifier>(Element))
		{
			if (CameraModifiers.Contains(CameraModifier->ModifierTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("Camera Modifier with tag %s already exists, %s will not be added to modifier list"), *CameraModifier->ModifierTag.ToString(), *CameraModifier->GetName());
				continue;
			}
			CameraModifier->Initialize();
			CameraModifiers.Add(CameraModifier->ModifierTag, CameraModifier);
		}
	}
}

UGvCameraModifier* AGvPlayerCameraManager::TryGetModifierByTag(const FGameplayTag Tag) const
{
	if (CameraModifiers.Contains(Tag))
	{
		return CameraModifiers[Tag];
	}
	return nullptr;
}
