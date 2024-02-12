#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "DoorKey.h"
#include "../../Interfaces/InteractiveInterface.h"
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

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _doorFrameComponent;

	class ASophia* _sophiaCharacter;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	bool _readyState;
	bool _openState;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool _doorOpened;

	UPROPERTY(EditAnywhere, Category = "Door")
	bool _rotateDirection;

	float _timelineValue;
	float _curveFloatValue;

	UPROPERTY(EditAnywhere, Category = "Door")
	uint8 _keyID;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ControlDoor(float value);
	UFUNCTION()
	virtual void SetState();

	virtual void UseInteraction() override;
};
