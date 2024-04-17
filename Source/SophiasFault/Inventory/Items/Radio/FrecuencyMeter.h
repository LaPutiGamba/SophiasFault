#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/ActorBlendInterface.h"
#include "EnhancedInputComponent.h"
#include "FrecuencyMeter.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AFrecuencyMeter : public AItem, public IActorBlendInterface
{
	GENERATED_BODY()
	
public:
	AFrecuencyMeter();

	UPROPERTY(EditAnywhere, Category = "Radio")
	class ARadio* _radio;

	UPROPERTY(EditAnywhere, Category = "Radio")
	class AActor* _needleActor;

	UPROPERTY(EditAnywhere, Category = "Actions")
	UInputAction* _mouseAxisAction;

	int _currentRadioFrecuency;
	float _currentFrequency;

	virtual void BeginPlay() override;

	void ChangeFrequency(const FInputActionValue& value);
};
