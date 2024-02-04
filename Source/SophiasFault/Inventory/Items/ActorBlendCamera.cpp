#include "ActorBlendCamera.h"
#include "../../Macros.h"

AActorBlendCamera::AActorBlendCamera()
{
	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
}