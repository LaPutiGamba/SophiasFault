#include "Padlock.h"
#include "PadlockNumber.h"
#include "../../../Cameras/PadlockCamera.h"
#include "../../../Macros.h"
#include "../Door/Door.h"

APadlock::APadlock()
{
}

void APadlock::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> camerasActorBlend;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APadlockCamera::StaticClass(), _cameraTag, camerasActorBlend);

	for (AActor* cameraActor : camerasActorBlend) {
		_cameraActorBlend = Cast<APadlockCamera>(cameraActor);
		if (_cameraActorBlend)
			break;
	}
}

void APadlock::UseInteraction()
{
	if (_firstNumber && _secondNumber && _thirdNumber && _fourthNumber) {
		FRotator firstNumberRot = _firstNumber->_meshComponent->GetRelativeRotation();
		FRotator secondNumberRot = _secondNumber->_meshComponent->GetRelativeRotation();
		FRotator thirdNumberRot = _thirdNumber->_meshComponent->GetRelativeRotation();
		FRotator fourthNumberRot = _fourthNumber->_meshComponent->GetRelativeRotation();

		if (firstNumberRot.Yaw >= 153.f && firstNumberRot.Yaw <= 155.f &&
			secondNumberRot.Yaw <= -61.f && secondNumberRot.Yaw >= -63.f &&
			thirdNumberRot.Yaw <= -61.f && thirdNumberRot.Yaw >= -63.f &&
			fourthNumberRot.Yaw <= -97.f && fourthNumberRot.Yaw >= -99.f) {
			if (_metaSound != nullptr) {
				_soundComponent->SetSound(_metaSound);
				_soundComponent->Play();
			}

			_meshComponent->SetSimulatePhysics(true);
			_owningInventory->_currentChangeCameraItem = nullptr;

			FTimerHandle deactivatePadlockHandle;
			GetWorld()->GetTimerManager().SetTimer(deactivatePadlockHandle, [this]() {
				_meshComponent->SetSimulatePhysics(false);
				_meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				if (_doorToUnlock != nullptr)
					_doorToUnlock->_bDoorLocked = false;
				if (_doorToUnlock2 != nullptr)
					_doorToUnlock2->_bDoorLocked = false;
			}, 1.f, false);
		} else {
			if (_interactSound != nullptr) {
				_soundComponent->SetSound(_interactSound);
				_soundComponent->Play();
			}
		}
	}
}