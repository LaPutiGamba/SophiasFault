#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/PickUpInterface.h"
#include "../../Interfaces/OnActionInterface.h"
#include "FlashlightCrystal.generated.h"

UCLASS()
class SOPHIASFAULT_API AFlashlightCrystal : public AItem, public IOnActionInterface, public IPickUpInterface
{
	GENERATED_BODY()
	
public:
	AFlashlightCrystal();

	UPROPERTY(EditAnywhere, meta = (GetOptions = "GetCrystalColor"), Category = "Flashlight")
	FString _crystalColor;

	UFUNCTION()
	TArray<FString> GetCrystalColor() const { return { "Blue", "Red", "Yellow", "Green" }; }


	UPROPERTY(EditAnywhere, Category = "Flashlight")
	AFlashlight* _flashlight;

	void OnAction() override;
};
