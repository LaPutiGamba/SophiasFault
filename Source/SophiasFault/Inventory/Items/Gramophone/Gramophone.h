#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/InteractiveInterface.h"
#include "Gramophone.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AGramophone : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	AGramophone();

	UPROPERTY(EditAnywhere, Category = "Gramophone")
	class AItem* firstShape;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	class AItem* secondShape;
	UPROPERTY(EditAnywhere, Category = "Gramophone")
	class AItem* thirdShape;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UseInteraction() override;
};
