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

	FInputBindingHandle* _bindingHandle;

	UPROPERTY(EditAnywhere, Category = "Earth Puzzle")
	FVector _earthBallLocation;

	virtual void UseInteraction() override;

	virtual void BlendBack() override;
};
