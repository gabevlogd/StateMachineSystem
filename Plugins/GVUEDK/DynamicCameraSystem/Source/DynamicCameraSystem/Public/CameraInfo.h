// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraInfo.generated.h"

USTRUCT(BlueprintType)
struct DYNAMICCAMERASYSTEM_API FCameraInfo 
{

	GENERATED_BODY()

	/** The field of view (in degrees) in perspective mode (ignored in Orthographic mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float FOV;

	/** Distance between the camera and the view target, in cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float SpringArmLength;

	/** Distance in which the Depth of Field effect should be sharp, in unreal units (cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DepthOfField)
	float DepthOfFieldFocalDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;

	FCameraInfo();

	/** Compares both camera property sets for equality. */
	bool Equals(const FCameraInfo& Other) const;

	/** Whether this property set modifies the camera at all. */
	bool IsZero() const;
};
