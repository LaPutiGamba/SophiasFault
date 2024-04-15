#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "Candle.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ACandle : public AItem
{
	GENERATED_BODY()
	
public:
	ACandle();

	virtual void BeginPlay() override;

	// TIMELINE
	UPROPERTY(EditAnywhere, Category = "Candle")
	class UCurveFloat* _curveFloat;

	float _timelineValue;
	float _curveFloatValue;

	FOnTimelineFloat _timelineCallback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Candle")
	class UParticleSystemComponent* _particleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Candle")
	class UPointLightComponent* _pointLight;

	UPROPERTY(EditAnywhere, Category = "Candle")
	float _minIntensity;
	UPROPERTY(EditAnywhere, Category = "Candle")
	float _maxIntensity;
	UPROPERTY(EditAnywhere, Category = "Candle")
	bool _bEnabled;

	UFUNCTION()
	virtual void ControlCandle(float value);

	void SetLight(bool bVisibility);
};
