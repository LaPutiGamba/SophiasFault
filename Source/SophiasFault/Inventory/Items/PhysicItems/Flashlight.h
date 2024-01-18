#pragma once

#include "CoreMinimal.h"
#include "../ItemPhysic.h"
#include "Tickable.h"
#include "Flashlight.generated.h"

enum FLASHLIGHT_STATUS { ST_LIGHTON, ST_LIGHTOFF, ST_NEEDRECHARGE };

UCLASS()
class SOPHIASFAULT_API AFlashlight : public AItemPhysic, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "10"))
	float _flashlightMaxDuration;

	class USpotLightComponent* _flashlight;
	bool _flashlightOn;
	bool _rechargingFlashlight;
	float _flashlightTimer;
	FLASHLIGHT_STATUS _flashlightStatus;

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
};
