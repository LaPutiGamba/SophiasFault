#include "MyTriggerBox.h"
#include "../Macros.h"
#include "../Sophia.h"
#include "../Inventory/Items/PhysicItems/Stair.h"

AMyTriggerBox::AMyTriggerBox()
{
    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::OnOverlapBegin(AActor* overlappedActor, AActor* otherActor)
{
    if (otherActor && (otherActor != this)) {
        ASophia* character = Cast<ASophia>(otherActor);
        if (character) {
            AStair* stair = Cast<AStair>(character->GetCurrentHandItem());
            if (stair)
                stair->_triggered = true;
        }
    }
}

void AMyTriggerBox::OnOverlapEnd(AActor* overlappedActor, AActor* otherActor)
{
    if (otherActor && (otherActor != this)) {
        ASophia* character = Cast<ASophia>(otherActor);
        if (character) {
            AStair* stair = Cast<AStair>(character->GetCurrentHandItem());
            if (stair)
                stair->_triggered = false;
        }
    }
}