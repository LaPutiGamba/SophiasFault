#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GMS_MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AGMS_MyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGMS_MyGameStateBase();

	virtual void BeginPlay() override;

	bool _onChase;
};
