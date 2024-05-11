#pragma once

#include "CoreMinimal.h"
#include "CameraBlend.h"
#include "PadlockCamera.generated.h"

UCLASS()
class SOPHIASFAULT_API APadlockCamera : public ACameraBlend
{
	GENERATED_BODY()
	
public:
	APadlockCamera();

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _interactAction;

	FInputBindingHandle* _interactHandle;

	virtual void UseInteraction(ACameraBlend* item) override;
	virtual void BlendBack() override;
	void ClickInteractive(const FInputActionValue& value);
};
