// Copyright Villains, Inc. All Rights Reserved.


#include "CameraInfo.h"


FCameraInfo::FCameraInfo()
{
	FOV = 0;
	SpringArmLength = 0;
	DepthOfFieldFocalDistance = 0;
	Position = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator;
}

bool FCameraInfo::Equals(const FCameraInfo& Other) const
{
	FCameraInfo Delta;
	Delta.FOV = FOV - Other.FOV;
	Delta.SpringArmLength = SpringArmLength - Other.SpringArmLength;
	Delta.DepthOfFieldFocalDistance = DepthOfFieldFocalDistance - Other.DepthOfFieldFocalDistance;
	Delta.Position = Position - Other.Position;
	Delta.Rotation = Rotation - Other.Rotation;
	return Delta.IsZero();
}

bool FCameraInfo::IsZero() const
{
	return FMath::Abs(FOV) < SMALL_NUMBER && FMath::Abs(SpringArmLength) < SMALL_NUMBER &&
		FMath::Abs(DepthOfFieldFocalDistance) < SMALL_NUMBER &&
		FMath::Abs(Position.X) < SMALL_NUMBER && FMath::Abs(Position.Y) < SMALL_NUMBER &&
		FMath::Abs(Position.Z) < SMALL_NUMBER && FMath::Abs(Rotation.Pitch) < SMALL_NUMBER &&
		FMath::Abs(Rotation.Yaw) < SMALL_NUMBER && FMath::Abs(Rotation.Roll) < SMALL_NUMBER;
}
