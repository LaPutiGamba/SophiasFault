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

	FInputBindingHandle* _bindingHandle;

	virtual void UseInteraction() override;

	virtual void BlendBack() override;
};
