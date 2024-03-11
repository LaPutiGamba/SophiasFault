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

	UPROPERTY(EditAnywhere)
	class ABasicBlendActor* _basicBlendActor;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _clickInteractiveAction;
	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _clickRotationAction;

	FVector2D _initialMousePos;
	bool _bIsRotating;

	virtual void UseInteraction() override;

	virtual void BlendBack() override;

	void EarthRotation(const FInputActionValue& value);
	void EndEarthRotation();
	void ClickInteractive(const FInputActionValue& value);
};
