#pragma once

#include "../../Item.h"
#include "PianoKey.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API APianoKey : public AItem
{
	GENERATED_BODY()

public:
	APianoKey();

	UPROPERTY(EditAnywhere, Category = "Piano")
	uint8 _pianoKeyID;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	bool _bReadyState;

	float _timelineValue;
	float _curveFloatValue;

	TSubclassOf<class UUserWidget> _dialogueWidgetClass;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ControlPianoKey(float value);
	UFUNCTION()
	virtual void SetState();

	void UseInteraction();
};
