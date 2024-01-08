#pragma once

#include "../ItemInteractive.h"
#include "LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ALightSwitch : public AItemInteractive
{
	GENERATED_BODY()
	
public:
	ALightSwitch();

	UPROPERTY(EditAnywhere)
	TArray<class AActor*> _lightActor;

	virtual void UseInteraction() override;
};
