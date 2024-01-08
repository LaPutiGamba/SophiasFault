#pragma once

#include "CoreMinimal.h"
#include "../ItemPhysic.h"
#include "Stair.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AStair : public AItemPhysic
{
	GENERATED_BODY()
	
public:
	AStair();

	UPROPERTY(EditAnywhere)
	class ATriggerBox* _triggerBox;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* _defaultMaterial;

	bool _triggered;
};
