#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/PickUpInterface.h"
#include "EnhancedInputComponent.h"
#include "Tickable.h"
#include "Flashlight.generated.h"

enum FLASHLIGHT_STATE { ST_LIGHTON, ST_LIGHTOFF, ST_NEEDRECHARGE };

UCLASS()
class SOPHIASFAULT_API AFlashlight : public AItem, public FTickableGameObject, public IPickUpInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "360"))
	float _flashlightMaxDuration;

	class USpotLightComponent* _flashlight;
	bool _flashlightOn;
	bool _rechargingFlashlight;
	float _flashlightTimer;
	FLASHLIGHT_STATE _flashlightState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _flashlightAction;
	FInputBindingHandle* _flashlightBindingHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _rechargeFlashlightAction;
	FInputBindingHandle* _rechargeFlashlightBindingHandle;

	class APlayerController* _playerController;

public:
	AFlashlight();

	virtual void BeginPlay();

	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(AFlashlight, STATGROUP_Tickables); }
	virtual bool IsTickableWhenPaused() const {	return true; }
	virtual bool IsTickableInEditor() const { return false;	}
	virtual void Tick(float deltaTime) override;

	void ToggleFlashlightOn() { _flashlightOn = !_flashlightOn; }
	void ToggleRechargingFlashlight() { _rechargingFlashlight = !_rechargingFlashlight; }

	void UseFlashlight(const FInputActionValue& value);
	void RechargeFlashlight(const FInputActionValue& value);

	virtual void PickUpItem(class AItem* item) override;
	virtual void DropItem(class AItem* item) override;
};
