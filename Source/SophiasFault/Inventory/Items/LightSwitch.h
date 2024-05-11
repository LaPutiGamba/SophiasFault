#pragma once

#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "LightSwitch.generated.h"

/**
 *
 */
UCLASS()
class SOPHIASFAULT_API ALightSwitch : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	ALightSwitch();

	bool _bLightOn;

	UPROPERTY(EditAnywhere)
	bool _bOldLight;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _lightSwitch;

	UPROPERTY(EditAnywhere)
	TArray<class ALight*> _lightActor;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	float _timelineValue;
	float _curveFloatValue;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UseInteraction(AItem* item) override;

	UFUNCTION()
	void ControlLightSwitch();
};