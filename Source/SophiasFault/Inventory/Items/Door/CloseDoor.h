#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "../../../Interfaces/TriggerInterface.h"
#include "CloseDoor.generated.h"

UCLASS()
class SOPHIASFAULT_API ACloseDoor : public ADoor, public ITriggerInterface
{
	GENERATED_BODY()
	
public:
	ACloseDoor();

	bool _bClosingDoorAnim;
	bool _bTriggeredOnce;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* _curveFloatClose;

	class UCurveFloat* _curveFloatTemp;

	virtual void SetState() override;

	virtual void OnTriggerStart() override;
};
