#include "ItemInteractive.h"
#include "../../Macros.h"

AItemInteractive::AItemInteractive() 
{
	PrimaryActorTick.bCanEverTick = true;

	_timelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

void AItemInteractive::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	_timelineComponent->TickComponent(deltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}