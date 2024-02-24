#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/InteractiveInterface.h"
#include "EarthContinent.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AEarthContinent : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	AEarthContinent();

	UPROPERTY(EditAnywhere)
	uint8 _continentID;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* _defaultMaterial;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* _outlineMaterial;

	class AGMS_MyGameStateBase* _myGameState;

	virtual void BeginPlay() override;

	virtual void UseInteraction() override;

	void Reset();
};