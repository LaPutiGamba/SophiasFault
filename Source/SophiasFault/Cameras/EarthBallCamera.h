#pragma once

#include "CoreMinimal.h"
#include "CameraBlend.h"
#include "EarthBallCamera.generated.h"

UCLASS()
class SOPHIASFAULT_API AEarthBallCamera : public ACameraBlend
{
	GENERATED_BODY()
	
public:
	AEarthBallCamera();

	FInputBindingHandle* _clickInteractiveHandle;
	FInputBindingHandle* _clickRotationHandle;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _clickInteractiveAction;
	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _clickRotationAction;

	UPROPERTY(EditAnywhere, Category = "Earth Puzzle")
	FVector _earthBallLocation;

	virtual void UseInteraction() override;

	virtual void BlendBack() override;

	void EarthRotation(const FInputActionValue& value);
	void ClickInteractive(const FInputActionValue& value);
};
