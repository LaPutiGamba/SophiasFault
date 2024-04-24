#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/ActorBlendInterface.h"
#include "CameraMonitor.generated.h"

UCLASS()
class SOPHIASFAULT_API ACameraMonitor : public AItem, public IActorBlendInterface
{
	GENERATED_BODY()
	
public:
	ACameraMonitor();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Blend", meta = (DisplayPriority = "1"))
	FName _cameraTag;

	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	TArray<AActor*> _cameras;

	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	UMaterial* _cameraMaterialRef;
	UMaterialInstanceDynamic* _cameraMat;

	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	float _screenBrightness;
	UPROPERTY(EditAnywhere, Category = "Camera Monitor")
	FText _cameraMonitorDialogue;
	bool _bDialoguePlayed;

	int _screenWidth, _screenHeight;
	int _currentCameraIndex;
	APlayerController* _playerController;

	void ChangeSecurityCamera();
};
