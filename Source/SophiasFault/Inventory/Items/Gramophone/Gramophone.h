#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/InteractiveInterface.h"
#include "Gramophone.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AGramophone : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	AGramophone();

	UPROPERTY(EditAnywhere, Category = "Gramophone")
	class AItem* _firstShape;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	class AItem* _secondShape;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	class AItem* _thirdShape;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	AActor* _crank;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	AActor* _record;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	AActor* _cover;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloat;

	FOnTimelineFloat _timelineCallback;

	bool _bReadyState;

	float _timelineValue;
	float _curveFloatValue;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UseInteraction(AItem* item) override;

	UFUNCTION()
	void ControlGramophone();
};
