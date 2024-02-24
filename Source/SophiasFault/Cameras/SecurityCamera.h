#pragma once

#include "CoreMinimal.h"
#include "CameraBlend.h"
#include "SecurityCamera.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ASecurityCamera : public ACameraBlend
{
	GENERATED_BODY()

public:
	ASecurityCamera();

	FInputBindingHandle* _clickInteractiveHandle;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _clickInteractiveAction;

    UPROPERTY(EditAnywhere, Category = "Camera Monitor")
    class ACameraMonitor* _cameraMonitor;

	virtual void UseInteraction() override;
	virtual void BlendBack() override;
};
