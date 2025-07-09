// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Camera/PlayerCameraManager.h"
#include "GvPlayerCameraManager.generated.h"

class UGvCameraModifier;
/**
 * 
 */
UCLASS()
class DYNAMICCAMERASYSTEM_API AGvPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UGvCameraModifier*> CameraModifiers;
	
	virtual void BeginPlay() override;
	void InitializeModifiers();

public:
	UFUNCTION(BlueprintCallable)
	UGvCameraModifier* TryGetModifierByTag(const FGameplayTag Tag) const;
};
