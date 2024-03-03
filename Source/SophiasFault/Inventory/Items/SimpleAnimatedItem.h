#pragma once

#include "CoreMinimal.h"
#include "../AnimatedItem.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "SimpleAnimatedItem.generated.h"

UCLASS()
class SOPHIASFAULT_API ASimpleAnimatedItem : public AAnimatedItem, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	ASimpleAnimatedItem();

	UPROPERTY(EditAnywhere, Category = "Animation")
	bool _bCanReverse;
	bool _bReadyState;

	virtual void BeginPlay() override;

	virtual void UseInteraction() override;
};
