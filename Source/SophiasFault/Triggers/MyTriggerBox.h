#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AMyTriggerBox();

	UFUNCTION()
	void OnOverlapBegin(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* overlappedActor, class AActor* otherActor);
};
