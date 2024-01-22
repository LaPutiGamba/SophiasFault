#pragma once

#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ALightSwitch : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	ALightSwitch();

	UPROPERTY(EditAnywhere)
	TArray<class AActor*> _lightActor;

	virtual void UseInteraction() override;
};
