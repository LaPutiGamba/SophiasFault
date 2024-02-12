#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/PickUpInterface.h"
#include "../../Interfaces/OnActionInterface.h"
#include "DoorKey.generated.h"

/**
 *
 */
UCLASS()
class SOPHIASFAULT_API ADoorKey : public AItem, public IPickUpInterface, public IOnActionInterface
{
	GENERATED_BODY()
	
public:
	ADoorKey();

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	FVector _initialLocation;

	UPROPERTY(EditAnywhere, Category = "Key")
	uint8 _keyID;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveVector* _curveVector;

	float _timelineValue;
	FVector _curveVectorValue;

	FOnTimelineVector _timelineCallback;

	UFUNCTION()
	virtual void ControlKey(FVector value);
	UFUNCTION()
	virtual void SetState();

	virtual void OnAction() override;
};
