#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "VectorAnimatedItem.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AVectorAnimatedItem : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	AVectorAnimatedItem();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool _bUseOnce;

	bool _bUseState;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UCurveVector* _curveVectorLocation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	class UCurveVector* _curveVectorRotation;

	float _timelineValue;
	FVector _curveVectorLocValue;
	FVector _curveVectorRotValue;
	FVector _locationApplied;
	FVector _rotationApplied;
	FVector newLocation;
	FVector newRotation;

	FOnTimelineVector _timelineCallback;

	UPROPERTY(EditAnywhere, Category = "Animation")
    TEnumAsByte<ERichCurveInterpMode> _interpolationMode;

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool _bLocation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	bool _bRotation;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (Units = "Seconds"))
	float _initialLocationTime;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (Units = "Seconds"))
	float _finalLocationTime;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (Units = "Seconds"))
	float _initialRotationTime;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (Units = "Seconds"))
	float _finalRotationTime;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FVector _initialPosition;
	UPROPERTY(EditAnywhere, Category = "Animation")
	FVector _finalPosition;
	UPROPERTY(EditAnywhere, Category = "Animation")
	FVector _initialRotation;
	UPROPERTY(EditAnywhere, Category = "Animation")
	FVector _finalRotation;

	UFUNCTION()
	virtual void ControlVectorAnimation(FVector value);
	UFUNCTION()
	virtual void SetState();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UseInteraction(AItem* item) override;
};
