#pragma once

#include "CoreMinimal.h"
#include "ActorBlendCamera.h"
#include "CameraMonitor.generated.h"

UCLASS()
class SOPHIASFAULT_API ACameraMonitor : public AActorBlendCamera
{
	GENERATED_BODY()
	
public:
	ACameraMonitor();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	TArray<AActor*> _cameras;

	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	UMaterial* _cameraMaterialRef;
	UMaterialInstanceDynamic* _cameraMat;

	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	float _screenBrightness;
	int _screenWidth, _screenHeight;
	int _currentCameraIndex;
	APlayerController* _playerController;

	void ChangeSecurityCamera();
};
