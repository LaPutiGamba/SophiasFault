#pragma once

#include "CoreMinimal.h"
#include "CameraBlend.h"
#include "RadioCamera.generated.h"

UCLASS()
class SOPHIASFAULT_API ARadioCamera : public ACameraBlend
{
	GENERATED_BODY()

public:
	ARadioCamera();

	UPROPERTY(EditAnywhere, Category = "Radio")
	class AFrecuencyMeter* _frecuencyMeter;
	UPROPERTY(EditAnywhere, Category = "Radio")
	class ARadio* _radio;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _mouseAxisAction;
	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _turnOffAction;

	FInputBindingHandle* _mouseAxisHandle;
	FInputBindingHandle* _turnOffHandle;

	virtual void UseInteraction(ACameraBlend* item) override;
	virtual void BlendBack() override;
};
