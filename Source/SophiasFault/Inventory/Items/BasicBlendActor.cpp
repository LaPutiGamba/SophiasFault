#include "BasicBlendActor.h"
#include "../../Cameras/CameraBlend.h"

ABasicBlendActor::ABasicBlendActor()
{
	_cameraTag = "";
}

void ABasicBlendActor::BeginPlay()
{
	Super::BeginPlay();

	if (_cameraTag != "") {
		TArray<AActor*> camerasActorBlend;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), _cameraTag, camerasActorBlend);

		for (AActor* cameraActor : camerasActorBlend) {
			_cameraActorBlend = Cast<ACameraBlend>(cameraActor);
			if (_cameraActorBlend)
				break;
		}
	}
}