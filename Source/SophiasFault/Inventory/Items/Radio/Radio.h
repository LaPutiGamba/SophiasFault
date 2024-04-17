#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/ActorBlendInterface.h"
#include "Radio.generated.h"

UCLASS()
class SOPHIASFAULT_API ARadio : public AItem, public IActorBlendInterface
{
	GENERATED_BODY()
	
public:
	ARadio();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Radio")
	class AFrecuencyMeter* _frecuencyMeter;

	UPROPERTY(EditAnywhere, Category = "Radio")
	class AActor* _volumeMeter;

	UPROPERTY(EditAnywhere, Category = "Radio")
	class AActor* _buttonActor;	

	UPROPERTY(EditAnywhere, Category = "Radio")
	class USoundBase* _turnOnSound;
	UPROPERTY(EditAnywhere, Category = "Radio")
	class USoundBase* _turnOffSound;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	float _timelineValue;
	float _curveFloatValue;
	float _volume;

	bool _bRadioOn;

	void TurnOffRadio();
	void ChangeVolume(const FInputActionValue& value);

	UFUNCTION()
	void ControlRadioButton(float value);
	UFUNCTION()
	void SetState();
};
