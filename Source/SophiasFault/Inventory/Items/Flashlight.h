#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/PickUpInterface.h"
#include "EnhancedInputComponent.h"
#include "Flashlight.generated.h"

enum FLASHLIGHT_STATE { ST_LIGHTON, ST_LIGHTOFF, ST_NEEDRECHARGE };

UCLASS()
class SOPHIASFAULT_API AFlashlight : public AItem, public IPickUpInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "1000"))
	float _flashlightMaxDuration;

	class USpotLightComponent* _flashlight;
	bool _bIsRechargingFlashlight;
	float _flashlightTimer;
	FLASHLIGHT_STATE _flashlightState;

	float _flashlightIntensity;
	float _flashlightIntensityMax;
	FTimerHandle _intensityTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial* _flashlightMaterial;
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial* _flickerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _flashlightAction;
	FInputBindingHandle* _flashlightBindingHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _rechargeFlashlightAction;
	FInputBindingHandle* _rechargeFlashlightBindingHandle;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _rechargeFlashlightSound;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	float _timelineValue;
	float _curveFloatValue;

	class APlayerController* _playerController;
	class ASophia* _sophia;

public:
	bool _bTurnedOn;

	AFlashlight();

	virtual void BeginPlay();
	virtual void Tick(float deltaTime) override;

	void ToggleFlashlightOn();

	void UseFlashlight(const FInputActionValue& value);
	void RechargeFlashlight(const FInputActionValue& value);
	UFUNCTION()
	void FinishRechargeFlashlight();

	virtual void PickUpItem(class AItem* item) override;
	virtual void DropItem(class AItem* item) override;

	UFUNCTION()
	void ControlCrankHandle(float value);
};
