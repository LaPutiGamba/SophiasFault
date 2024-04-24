#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/PickUpInterface.h"
#include "../../../Interfaces/OnActionInterface.h"
#include "PianoSheet.generated.h"

UCLASS()
class SOPHIASFAULT_API APianoSheet : public AItem, public IPickUpInterface, public IOnActionInterface
{
	GENERATED_BODY()
	
public:
	APianoSheet();

	virtual void BeginPlay() override;

	virtual void OnAction() override;
};
