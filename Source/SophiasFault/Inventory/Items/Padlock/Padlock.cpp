#include "Padlock.h"
#include "PadlockNumber.h"
#include "../../../Cameras/PadlockCamera.h"

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
		FRotator firstNumberRot = _firstNumber->GetActorRotation();
		FRotator secondNumberRot = _secondNumber->GetActorRotation();
		FRotator thirdNumberRot = _thirdNumber->GetActorRotation();
		FRotator fourthNumberRot = _fourthNumber->GetActorRotation();

		if (firstNumberRot.Roll <= -134.f && firstNumberRot.Roll >= -136.f &&
			secondNumberRot.Roll >= 134.f && secondNumberRot.Roll <= 136.f &&
			thirdNumberRot.Roll <= -44.f && thirdNumberRot.Roll >= -46.f) {
			if (_metaSound != nullptr) {
				_soundComponent->SetSound(_metaSound);
				_soundComponent->Play();
			}
		} else {
			if (_interactSound != nullptr) {
				_soundComponent->SetSound(_interactSound);
				_soundComponent->Play();
			}
		}
	}
}