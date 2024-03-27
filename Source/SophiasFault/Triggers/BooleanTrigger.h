#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "../Interfaces/TriggerInterface.h"
#include "BooleanTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API ABooleanTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ABooleanTrigger();

	UPROPERTY(EditAnywhere, Category = "Trigger")
	TArray<AActor*> _triggerActors;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool _bHasToTriggerOnce;
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* overlappedActor, class AActor* otherActor);
};
