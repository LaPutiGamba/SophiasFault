#include "Radio.h"
#include "../../../Cameras/RadioCamera.h"
#include "FrecuencyMeter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "../../../Macros.h"

ARadio::ARadio()
{
	_bRadioOn = false;
}

void ARadio::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> camerasActorBlend;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARadioCamera::StaticClass(), camerasActorBlend);

	for (AActor* cameraActor : camerasActorBlend) {
		_cameraActorBlend = Cast<ARadioCamera>(cameraActor);
		if (_cameraActorBlend)
			break;
	}

	if (_metaSound != nullptr) {
		if (_metaSound->IsValidLowLevelFast())
			_soundComponent->SetSound(_metaSound);
	}

	// Timeline
	if (_curveFloat) {
		_timelineCallback.BindUFunction(this, FName("ControlRadioButton"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}
}

void ARadio::ControlRadioButton(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	if (_buttonActor != nullptr) 
		_buttonActor->SetActorRelativeRotation(FQuat(FRotator(_curveFloatValue, 0.f, 0.f)));
}

void ARadio::SetState()
{
}

void ARadio::TurnOffRadio()
{
	_bRadioOn = !_bRadioOn;

	if (_bRadioOn) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), _turnOnSound, GetActorLocation());
		UKismetSystemLibrary::Delay(GetWorld(), 0.5f, FLatentActionInfo());
		int radioFrecuency = 0;

		if (_frecuencyMeter->_currentFrequency > 92.0f && _frecuencyMeter->_currentFrequency < 92.2f)
			radioFrecuency = 1;
		else if (_frecuencyMeter->_currentFrequency > 100.0f && _frecuencyMeter->_currentFrequency < 100.2f)
			radioFrecuency = 2;
		else if (_frecuencyMeter->_currentFrequency > 105.0f && _frecuencyMeter->_currentFrequency < 105.2f)
			radioFrecuency = 3;
		else if (_frecuencyMeter->_currentFrequency > 89.0f && _frecuencyMeter->_currentFrequency < 89.2f)
			radioFrecuency = 4;

		_timelineComponent->PlayFromStart();
		_soundComponent->SetIntParameter("Radio Frecuency", radioFrecuency);
		_soundComponent->Play();
	} else {
		_timelineComponent->Reverse();
		_soundComponent->Stop();

		UKismetSystemLibrary::Delay(GetWorld(), 0.1f, FLatentActionInfo());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), _turnOffSound, GetActorLocation());
	}
}