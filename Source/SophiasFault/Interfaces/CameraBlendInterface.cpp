#include "CameraBlendInterface.h"
#include "../Cameras/CameraBlend.h"
#include "Kismet/GameplayStatics.h"

void ICameraBlendInterface::UseInteraction(ACameraBlend* item)
{
	if (item == nullptr)
		return;

	if (item->_interactSound)
		UGameplayStatics::PlaySoundAtLocation(item, item->_interactSound, item->GetActorLocation());
}