#include "FrecuencyMeter.h"
#include "../../../Cameras/RadioCamera.h"
#include "Kismet/GameplayStatics.h"
#include "../../Items/Radio/Radio.h"
#include "../../../Macros.h"

AFrecuencyMeter::AFrecuencyMeter()
{
    _currentFrequency = 98.1f;
    _currentRadioFrecuency = 0.f;
	_mouseAxisAction = nullptr;
}

void AFrecuencyMeter::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> camerasActorBlend;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARadioCamera::StaticClass(), camerasActorBlend);
    
    for (AActor* cameraActor : camerasActorBlend) {
		_cameraActorBlend = Cast<ARadioCamera>(cameraActor);
        if (_cameraActorBlend)
			break;
	}
}

void AFrecuencyMeter::ChangeFrequency(const FInputActionValue& value)
{
    float mouseAxis = value.Get<float>();

    if (mouseAxis > 0.f) {
        if (_currentFrequency < 108.f) {
            _currentFrequency += 0.1f;

            AddActorLocalRotation(FRotator(1.f, 0.f, 0.f));

            if (_needleActor != nullptr)
                _needleActor->AddActorLocalOffset(FVector(-0.075f, 0.f, 0.f));
        }
    } else if (mouseAxis < 0.f) {
        if (_currentFrequency > 88.1f) {
            _currentFrequency -= 0.1f;

            AddActorLocalRotation(FRotator(-1.f, 0.f, 0.f));

            if (_needleActor != nullptr)
                _needleActor->AddActorLocalOffset(FVector(0.075f, 0.f, 0.f));
        }
    }

    int radioFrecuency;

    if (_currentFrequency > 92.01f && _currentFrequency < 92.2f)
        radioFrecuency = 1;
    else if (_currentFrequency > 100.01f && _currentFrequency < 100.2f)
        radioFrecuency = 2;
    else if (_currentFrequency > 105.01f && _currentFrequency < 105.2f)
        radioFrecuency = 3;
    else if (_currentFrequency > 89.01f && _currentFrequency < 89.2f)
        radioFrecuency = 4;
    else 
        radioFrecuency = 0;

    if (_currentRadioFrecuency != radioFrecuency && _radio->_bRadioOn) {
        _currentRadioFrecuency = radioFrecuency;

        if (_radio->_soundComponent->IsPlaying())
            _radio->_soundComponent->Stop();

        _radio->_soundComponent->SetIntParameter("Radio Frecuency", radioFrecuency);
        _currentRadioFrecuency = radioFrecuency;
        _radio->_soundComponent->Play();
    }

    printFloat(_currentFrequency);
}
