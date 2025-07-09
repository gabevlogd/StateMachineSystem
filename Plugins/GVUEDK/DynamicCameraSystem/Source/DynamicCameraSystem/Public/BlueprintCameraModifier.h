// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraPOV.h"
#include "GvCameraModifier.h"
#include "BlueprintCameraModifier.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICCAMERASYSTEM_API UBlueprintCameraModifier : public UGvCameraModifier
{
	GENERATED_BODY()

private:
	UPROPERTY()
	ACharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraPOV* CustomPOV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ToolTip = "If true, when the camera modifier is applied, no other camera modifier will be applied"))
	bool bHasPriority = false;
	FCameraInfo DefaultPov;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.01", AllowPrivateAccess = "true"))
	float TransitionTimeIn;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.01", AllowPrivateAccess = "true"))
	float TransitionTimeOut;
	

public:
	UBlueprintCameraModifier();
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnable();
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	bool ApplyModifierIn(ACharacter* Context);
	UFUNCTION(BlueprintNativeEvent)
	bool ApplyModifierOut(ACharacter* Context);
	UFUNCTION(BlueprintNativeEvent)
	void BeginPlay(ACharacter* Context);

	
	
private:
	virtual void AddedToCamera(APlayerCameraManager* Camera) override;
	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
	virtual void Initialize() override;
};
