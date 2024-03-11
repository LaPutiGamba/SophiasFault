#pragma once

#include "CoreMinimal.h"
#include "DoorKey.h"
#include "../SimpleAnimatedItem.h"
#include "../../../Interfaces/InteractiveInterface.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ADoor : public AAnimatedItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	ADoor();

	class ASophia* _sophiaCharacter;

	UPROPERTY(EditAnywhere, Category = "Door")
	class ASimpleAnimatedItem* _doorHandle;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _selectedCurveFloat;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _openCurveFloat;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _lockedCurveFloat;

	class UTimelineComponent* _timelineComponent;

	FOnTimelineEvent _timelineFinishedCallback;
	FOnTimelineFloat _timelineCallback;

	bool _bReadyState;
	bool _bOpenState;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool _bDoorLocked;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool _bRotateDirection;

	float _timelineValue;
	float _curveFloatValue;
	float _rotationApplied;

	UPROPERTY(EditAnywhere, Category = "Door")
	uint8 _keyID;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ControlDoor(float value);
	UFUNCTION()
	virtual void SetState();

	virtual void UseInteraction() override;
};
