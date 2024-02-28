#include "CloseDoor.h"

ACloseDoor::ACloseDoor()
{
	_bTriggeredOnce = false;
	_bClosingDoorAnim = false;
}

void ACloseDoor::SetState()
{
	if (_bClosingDoorAnim) {
		_curveFloat = _curveFloatTemp;
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
				_curveFloatTemp = _curveFloat;
				_curveFloat = _curveFloatClose;
				_timelineComponent->PlayFromStart();
				_bClosingDoorAnim = true;

				_soundComponent->SetIntParameter("Door State", 3);
				_soundComponent->Play();
			}
		}, 0.5f, false);
	}
}