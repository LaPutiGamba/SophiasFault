#pragma once

#include "CoreMinimal.h"
#include "../ItemInteractive.h"
#include "EarthContinent.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AEarthContinent : public AItemInteractive
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