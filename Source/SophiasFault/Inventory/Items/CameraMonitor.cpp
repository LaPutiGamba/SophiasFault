#include "CameraMonitor.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetRenderingLibrary.h" 
#include "Components/SceneCaptureComponent2D.h" 
#include "GameFramework/PlayerController.h"
#include "../../Macros.h"

ACameraMonitor::ACameraMonitor()
{
	_cameraMaterialRef = nullptr;
	_screenBrightness = 0.5f;
}

void ACameraMonitor::BeginPlay()
{
	Super::BeginPlay();

	_playerController = GetWorld()->GetFirstPlayerController();

	if (_cameras.IsValidIndex(0)) {
		_cameraMat = UMaterialInstanceDynamic::Create(_cameraMaterialRef, this);
		this->_meshComponent->SetMaterial(0, _cameraMat);

		_playerController->GetViewportSize(_screenWidth, _screenHeight);
		UTextureRenderTarget2D* cameraRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, _screenWidth, _screenHeight);
		
		USceneCaptureComponent2D* sceneCaptureComponent = _cameras[0]->FindComponentByClass<USceneCaptureComponent2D>();
		if (sceneCaptureComponent != nullptr) {
			sceneCaptureComponent->TextureTarget = cameraRT;
			_cameraMat->SetTextureParameterValue("Texture", cameraRT);
			_cameraMat->SetScalarParameterValue("ScreenBrightness", _screenBrightness);
		}
	}
}

void ACameraMonitor::ChangeSecurityCamera()
{
	USceneCaptureComponent2D* sceneCaptureComponent = _cameras[_currentCameraIndex]->FindComponentByClass<USceneCaptureComponent2D>();
	if (sceneCaptureComponent != nullptr) {
		sceneCaptureComponent->TextureTarget = nullptr;

		if (_playerController->PlayerInput->IsPressed(EKeys::LeftMouseButton)) {
			_currentCameraIndex++;
			if (_currentCameraIndex >= _cameras.Num())
				_currentCameraIndex = 0;
		} else {
			_currentCameraIndex--;
			if (_currentCameraIndex < 0)
				_currentCameraIndex = _cameras.Num() - 1;
		}

		_playerController->GetViewportSize(_screenWidth, _screenHeight);
		UTextureRenderTarget2D* cameraRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, _screenWidth, _screenHeight);

		sceneCaptureComponent = _cameras[_currentCameraIndex]->FindComponentByClass<USceneCaptureComponent2D>();
		if (sceneCaptureComponent != nullptr) {
			sceneCaptureComponent->TextureTarget = cameraRT;
			_cameraMat->SetTextureParameterValue("Texture", cameraRT);
		}
	}
}
