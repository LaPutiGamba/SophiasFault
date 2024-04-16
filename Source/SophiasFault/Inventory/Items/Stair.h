#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/OnActionInterface.h"
#include "../../Interfaces/PickUpInterface.h"
#include "../../Interfaces/TriggerInterface.h"
#include "Stair.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AStair : public AItem, public IOnActionInterface, public IPickUpInterface, public ITriggerInterface
{
	GENERATED_BODY()
	
public:
	AStair();

	virtual void BeginPlay() override;

	bool _triggered;

	UPROPERTY(EditAnywhere)
	class AActor* _finalPosition;
	class UStaticMeshComponent* _finalPositionMesh;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* _defaultMaterial;

	virtual void OnAction() override;

	virtual void OnTriggerStart() override;
	virtual void OnTriggerEnd() override;
};
