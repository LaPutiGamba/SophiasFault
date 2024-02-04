#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/OnActionInterface.h"
#include "../../Interfaces/PickUpInterface.h"
#include "Components/TimelineComponent.h"
#include "MirrorLight.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AMirrorLight : public AItem, public IOnActionInterface, public IPickUpInterface
{
	GENERATED_BODY()
	
public:
	AMirrorLight();

	UPROPERTY(EditAnywhere)
	uint8 _mirrorLightID;

	AActor* _mirrorLightPositioned;

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	virtual void PickUpItem(class AItem* item) override;

	// TIMELINE
	class UTimelineComponent* _timelineComponent;
	class UCurveVector* _curveVector;

	float _timelineValue;
	FVector _curveVectorValue;

	FOnTimelineEvent _timelineFinishedCallback;
	FOnTimelineVector _timelineCallback;

	FVector _lightFinishPosition;

	UFUNCTION()
	virtual void ControlMirrorLight(FVector value);
	UFUNCTION()
	virtual void SetState();

	void PositionLight(FVector placeablePosition);

	virtual void OnAction() override;
};
