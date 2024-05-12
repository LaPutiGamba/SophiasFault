#include "ChangeLevelTrigger.h"
#include "../Sophia.h"
#include "Kismet/GameplayStatics.h"

AChangeLevelTrigger::AChangeLevelTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &AChangeLevelTrigger::OnOverlapBegin);
}

void AChangeLevelTrigger::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && (otherActor != this)) {
		ASophia* character = Cast<ASophia>(otherActor);
		if (character) {
			UGameplayStatics::OpenLevel(GetWorld(), _levelName);
		}
	}
}