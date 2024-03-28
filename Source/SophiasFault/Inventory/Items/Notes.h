#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/PickUpInterface.h"
#include "../../Interfaces/OnActionInterface.h"
#include "Notes.generated.h"

UCLASS()
class SOPHIASFAULT_API ANotes : public AItem, public IPickUpInterface, public IOnActionInterface
{
	GENERATED_BODY()
	
public:
	ANotes();

	class APlayerController* _playerController;

	UPROPERTY(EditAnywhere, Category = "Note", meta = (MultiLine = true, BindWidget))
	FText _noteText;

	virtual void BeginPlay() override;

	virtual void OnAction() override;
};
