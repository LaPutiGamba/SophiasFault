#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "EarthContinent.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AEarthContinent : public AItem
{
	GENERATED_BODY()
	
public:
	AEarthContinent();

	UPROPERTY(EditAnywhere)
	uint8 _continentID;

	class AGMS_MyGameStateBase* _myGameState;

	virtual void BeginPlay() override;

	void AddOrRemoveContinent();
	void Reset();
};