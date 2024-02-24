#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../Interfaces/ActorBlendInterface.h"
#include "CameraMonitor.generated.h"

UCLASS()
class SOPHIASFAULT_API ACameraMonitor : public AActor, public IActorBlendInterface
{
	GENERATED_BODY()
	
public:
	ACameraMonitor();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* _meshComponent;

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

	// AUDIO
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _metaSound;

	class UAudioComponent* _soundComponent;

	void ChangeSecurityCamera();
};
