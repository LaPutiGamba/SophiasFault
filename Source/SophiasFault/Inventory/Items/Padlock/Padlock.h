#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/ActorBlendInterface.h"
#include "Padlock.generated.h"

UCLASS()
class SOPHIASFAULT_API APadlock : public AItem, public IActorBlendInterface
{
	GENERATED_BODY()

public:
	APadlock();

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Padlock")
	FName _cameraTag;

	UPROPERTY(EditAnywhere, Category = "Padlock")
	class ADoor* _doorToUnlock;
	UPROPERTY(EditAnywhere, Category = "Padlock")
	class ADoor* _doorToUnlock2;

	UPROPERTY(EditAnywhere, Category = "Padlock")
	class APadlockNumber* _firstNumber;
	UPROPERTY(EditAnywhere, Category = "Padlock")
	class APadlockNumber* _secondNumber;
	UPROPERTY(EditAnywhere, Category = "Padlock")
	class APadlockNumber* _thirdNumber;
	UPROPERTY(EditAnywhere, Category = "Padlock")
	class APadlockNumber* _fourthNumber;

	void UseInteraction();
};
