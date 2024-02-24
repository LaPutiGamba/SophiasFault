#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Cameras/CameraBlend.h"
#include "Kismet/GameplayStatics.h"
#include "ActorBlendInterface.generated.h"

UINTERFACE(MinimalAPI)
class UActorBlendInterface : public UInterface
{
	GENERATED_BODY()
};

class SOPHIASFAULT_API IActorBlendInterface
{
	GENERATED_BODY()

public:
	ACameraBlend* _cameraActorBlend = nullptr;
};
