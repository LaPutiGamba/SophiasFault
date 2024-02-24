#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "FloatAnimatedItem.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AFloatAnimatedItem : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	AFloatAnimatedItem();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	bool _bIsActivated;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UCurveFloat* _curveFloat;

	float _timelineValue;
	float _curveFloatValue;

	FOnTimelineFloat _timelineCallback;

	// General properties
	UPROPERTY(EditAnywhere, Category = "Animation");
	bool _bLocationOrRotation;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (Units = "Seconds"))
	float _initialTime;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (Units = "Seconds"))
	float _finalTime;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float _initialPosition;
	UPROPERTY(EditAnywhere, Category = "Animation")
	float _finalPosition;

	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 _XorYorZ;

	UFUNCTION()
	virtual void ControlFloatAnimation(float value);
	UFUNCTION()
	virtual void SetState();

	virtual void UseInteraction() override;
};
