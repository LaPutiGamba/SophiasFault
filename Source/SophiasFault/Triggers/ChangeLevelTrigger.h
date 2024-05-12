#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ChangeLevelTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AChangeLevelTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AChangeLevelTrigger();

	UPROPERTY(EditAnywhere, Category = "Trigger")
	FName _levelName;

	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool _bHasToTriggerOnce;
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);
};
