#pragma once

#include "../ItemInteractive.h"
#include "PianoKey.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API APianoKey : public AItemInteractive
{
	GENERATED_BODY()

public:
	APianoKey();

	UPROPERTY(EditAnywhere)
	uint8 _pianoKeyID;

	class AGMS_MyGameStateBase* _myGameState;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	bool _bReadyState;

	float _timelineValue;
	float _curveFloatValue;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ControlPianoKey(float value);
	UFUNCTION()
	virtual void SetState();

	virtual void UseInteraction() override;
};
