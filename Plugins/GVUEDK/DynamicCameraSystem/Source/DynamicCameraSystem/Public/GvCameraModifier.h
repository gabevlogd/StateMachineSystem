// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraInfo.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraModifier.h"
#include "GvCameraModifier.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICCAMERASYSTEM_API UGvCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ModifierTag;

protected:
	bool bInitialized = false;
	
private:
	UPROPERTY()
	FCameraInfo CurrentModifiers;
	UPROPERTY()
	FCameraInfo TargetModifiers;
	
	float TotalTransitionTime;
	float RemainingTransitionTime;
	
public:
	virtual void Initialize();
protected:
	/**
	 * Apply the camera transition
	 * @param CameraInfo The camera info to transition to
	 * @param TransitionTime The time it takes to transition to the new camera info
	 * @param InOutPOV The current camera view
	 * @param DeltaTime The time since the last frame
	 * @return True if the transition is complete, false otherwise
	 */
	bool ApplyCameraTransition(const FCameraInfo& CameraInfo, const float TransitionTime, FMinimalViewInfo& InOutPOV, const float DeltaTime);
	//bool ApplyCameraTransition_Test(const FCameraInfo& CameraInfo, const float TransitionSpeed, FMinimalViewInfo& InOutPOV, const float DeltaTime);
	
private:
	/**
	 * Apply the camera info
	 * @param CameraInfo The camera info to apply
	 * @param Factor The factor to apply the camera info with
	 * @param InOutPOV The current camera view
	 */
	void ApplyCameraInfo(const FCameraInfo& CameraInfo, const float Factor, FMinimalViewInfo& InOutPOV) const;

};
