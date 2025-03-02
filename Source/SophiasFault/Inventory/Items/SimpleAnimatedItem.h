#pragma once

#include "CoreMinimal.h"
#include "../AnimatedItem.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "../../Interfaces/TriggerInterface.h"
#include "SimpleAnimatedItem.generated.h"

UCLASS()
class SOPHIASFAULT_API ASimpleAnimatedItem : public AAnimatedItem, public IInteractiveInterface, public ITriggerInterface
{
	GENERATED_BODY()

public:
	ASimpleAnimatedItem();

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool _bCanReverse;
	bool _bReadyState;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UseInteraction(class AItem* item) override;

	UFUNCTION(BlueprintCallable, Category = "Trigger")
	virtual void OnTriggerStart() override;
};
