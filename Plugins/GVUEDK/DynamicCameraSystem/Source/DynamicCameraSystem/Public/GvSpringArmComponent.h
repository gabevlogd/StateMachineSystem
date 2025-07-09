// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "GvSpringArmComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpringArmHitSomething);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DYNAMICCAMERASYSTEM_API UGvSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSpringArmHitSomething OnSpringArmHitSomething;
	
	/** Natural length of the spring arm when there are no collisions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTargetArmLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bApplyCustomBlendLocation = true;
	
	/** Value to add to the target arm length, e.g. caused by camera modifiers. */
	float TargetArmLengthModifier;


private:
	UPROPERTY()
	ACharacter* OwnerCharacter;
	bool bHasValidOwner = false;
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bShowBlendLocationDebug = false;
	

public:

	UGvSpringArmComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;

protected:
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;
	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime) override;

};
