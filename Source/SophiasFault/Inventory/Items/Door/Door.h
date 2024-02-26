#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "DoorKey.h"
#include "../../../Interfaces/InteractiveInterface.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ADoor : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	ADoor();

	class ASophia* _sophiaCharacter;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

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

	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _metaSoundClose;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _metaSoundLocked;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ControlDoor(float value);
	UFUNCTION()
	virtual void SetState();

	virtual void UseInteraction() override;
};
