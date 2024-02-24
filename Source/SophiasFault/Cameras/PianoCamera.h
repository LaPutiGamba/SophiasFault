#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CameraBlend.h"
#include "PianoCamera.generated.h"

UCLASS()
class SOPHIASFAULT_API APianoCamera : public ACameraBlend
{
	GENERATED_BODY()
	
public:
    APianoCamera();

	UPROPERTY(EditAnywhere, Category = "Final Animation")
	class APianoKey* _pianoHollowKey;

	UPROPERTY(EditAnywhere, Category = "Final Animation")
	class ADoorKey* _doorKey;

	UPROPERTY(EditAnywhere, Category = "Final Animation")
	class UCurveFloat* _curveFloatFinal;

	UPROPERTY(EditAnywhere, Category = "Cameras") 
	class ACameraActor* _sheetCamera;

	FInputBindingHandle* _clickInteractiveHandle;
	FInputBindingHandle* _blendCameraHandle;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _clickInteractiveAction;
	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _blendCameraAction;

	virtual void UseInteraction() override;

	virtual void BlendBack() override;

	void ClickInteractive(const FInputActionValue& value);

	void LookPianoSheet(const FInputActionValue& value);

	void ActivatePianoSolution();
};
