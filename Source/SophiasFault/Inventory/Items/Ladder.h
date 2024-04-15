#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "Ladder.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ALadder : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	ALadder();

	virtual void BeginPlay() override;

	APlayerController* _playerController;
	class ASophia* _sophia;

	bool _bIsUp;

	// TIMELINE
	UPROPERTY()
	class UCurveVector* _curveVectorLocation;
	UPROPERTY()
	class UCurveVector* _curveVectorRotation;

	float _timelineValue;
	FVector _curveVectorLocationValue;
	FVector _curveVectorRotationValue;

	FOnTimelineVector _timelineCallback;

	void SetMovementAndCollisions(bool bState);
	void SetInitialKeys(float time);

	UFUNCTION()
	void ControlLadderAnim(float value);
	UFUNCTION()
	void FinishAnim();

	void UseInteraction(AItem* item) override;
};
