#include "CloseDoor.h"
#include "Kismet/GameplayStatics.h"

ACloseDoor::ACloseDoor()
{
	_bTriggeredOnce = false;
	_bClosingDoorAnim = false;
}

void ACloseDoor::SetState()
{
	if (_bClosingDoorAnim) {
		_selectedCurveFloat = _curveFloatTemp;
		_bReadyState = true;
		_bClosingDoorAnim = false;
	} else {
		_bReadyState = true;
	}
}

void ACloseDoor::OnTriggerStart()
{
	if (!_bTriggeredOnce && _metaSound != nullptr) {
		_bTriggeredOnce = true;
		_bDoorLocked = true;

		FTimerHandle doorTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(doorTimerHandle, [this]() {
			if (_curveFloatClose) {
				_curveFloatTemp = _selectedCurveFloat;
				_selectedCurveFloat = _curveFloatClose;
				_timelineComponent->PlayFromStart();
				_bClosingDoorAnim = true;

				_soundComponent->SetIntParameter("Door State", 3);
				_soundComponent->SetVolumeMultiplier(3.0f);
				_soundComponent->Play();

				// Play camera shake
				if (_shakeDoor)
					UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(_shakeDoor, 1.0f);
			}
		}, 0.5f, false);
	}
}