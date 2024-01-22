#include "ActorBlendCamera.h"
#include "../../Macros.h"

AActorBlendCamera::AActorBlendCamera()
{
	_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
}