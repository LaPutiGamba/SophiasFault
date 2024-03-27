#include "BooleanTrigger.h"
#include "../Macros.h"
#include "../Sophia.h"

ABooleanTrigger::ABooleanTrigger()
{
    OnActorBeginOverlap.AddDynamic(this, &ABooleanTrigger::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ABooleanTrigger::OnOverlapEnd);
}

void ABooleanTrigger::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
    if (otherActor && (otherActor != this)) {
        ASophia* character = Cast<ASophia>(otherActor);
        if (character) {
            for (auto triggeredActor : _triggerActors) {
                if (ITriggerInterface* castedActor = Cast<ITriggerInterface>(triggeredActor))
                    castedActor->OnTriggerStart();
            }
        }
    }
}

void ABooleanTrigger::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
    if (otherActor && (otherActor != this)) {
        ASophia* character = Cast<ASophia>(otherActor);
        if (character) {
            for (auto triggeredActor : _triggerActors) {
                if (ITriggerInterface* castedActor = Cast<ITriggerInterface>(triggeredActor))
                    castedActor->OnTriggerEnd();
            }

            if (_bHasToTriggerOnce)
				Destroy();
        }
    }
}