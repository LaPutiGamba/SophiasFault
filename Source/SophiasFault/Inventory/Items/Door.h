#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
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

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	bool _readyState;
	bool _openState;

	float _rotateValue;
	float _timelineValue;
	float _curveFloatValue;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ControlDoor(float value);
	UFUNCTION()
	virtual void SetState();

	virtual void UseInteraction() override;
};
