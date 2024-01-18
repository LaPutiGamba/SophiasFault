#pragma once

#include "CoreMinimal.h"
#include "../ItemPhysic.h"
#include "Components/TimelineComponent.h"
#include "MirrorLight.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AMirrorLight : public AItemPhysic
{
	GENERATED_BODY()
	
public:
	AMirrorLight();

	UPROPERTY(EditAnywhere)
	uint8 _mirrorLightID;

	AActor* _mirrorLightPositioned;

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	// TIMELINE
	class UTimelineComponent* _timelineComponent;

	UPROPERTY(EditAnywhere, Category = "Timeline")
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
};
