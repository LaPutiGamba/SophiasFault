#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "CorridorDoor.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ACorridorDoor : public ADoor
{
	GENERATED_BODY()

public:
	ACorridorDoor();

	virtual void BeginPlay() override;

	virtual void ControlDoor(float value) override;
};
