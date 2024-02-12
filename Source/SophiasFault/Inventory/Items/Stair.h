#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/OnActionInterface.h"
#include "../../Interfaces/PickUpInterface.h"
#include "Stair.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AStair : public AItem, public IOnActionInterface, public IPickUpInterface
{
	GENERATED_BODY()
	
public:
	AStair();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class ATriggerBox* _triggerBox;

	UPROPERTY(EditAnywhere)
	class AActor* _finalPosition;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* _defaultMaterial;

	bool _triggered;

	virtual void OnAction() override;
};
