#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/InteractiveInterface.h"
#include "MirrorSolution.generated.h"

UCLASS()
class SOPHIASFAULT_API AMirrorSolution : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	AMirrorSolution();

	UPROPERTY(EditAnywhere)
	AActor* _deskDrawerActor;

	virtual void UseInteraction() override;
};