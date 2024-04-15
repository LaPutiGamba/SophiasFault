#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraBlendInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCameraBlendInterface : public UInterface
{
	GENERATED_BODY()
};

class SOPHIASFAULT_API ICameraBlendInterface
{
	GENERATED_BODY()

public:
	virtual void UseInteraction(class ACameraBlend* item);
	
	virtual void BlendBack() {};
};
