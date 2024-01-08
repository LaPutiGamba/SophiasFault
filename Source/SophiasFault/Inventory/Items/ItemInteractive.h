#pragma once

#include "Item.h"
#include "Components/TimelineComponent.h"
#include "ItemInteractive.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AItemInteractive : public AItem
{
	GENERATED_BODY()
	
public:
	AItemInteractive();

	virtual void Tick(float deltaTime) override;
	
	// TIMELINE
	class UTimelineComponent* _timelineComponent;

	FOnTimelineEvent _timelineFinishedCallback;

	// AUDIO
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundCue* _soundCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* _soundComponent;

	// INTERACTION
	virtual void UseInteraction() {};
};
