#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "DistributionPanel.generated.h"

UCLASS()
class SOPHIASFAULT_API ADistributionPanel : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	ADistributionPanel();

	virtual void BeginPlay() override;

	bool _bPoweredOnce;

	UPROPERTY(EditAnywhere, Category = "Distribution Panel")
	TArray<class ALight*> _lights;

	void UseInteraction(class AItem* item) override;
};
