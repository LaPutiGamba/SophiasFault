#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../../Interfaces/ActorBlendInterface.h"
#include "BasicBlendActor.generated.h"

UCLASS()
class SOPHIASFAULT_API ABasicBlendActor : public AItem, public IActorBlendInterface
{
	GENERATED_BODY()

public:
	ABasicBlendActor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Blend", meta = (DisplayPriority = "1"))
	FName _cameraTag;
};
