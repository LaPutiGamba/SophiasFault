#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Cameras/CameraBlend.h"
#include "ActorBlendCamera.generated.h"

UCLASS()
class SOPHIASFAULT_API AActorBlendCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AActorBlendCamera();

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* _staticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Camera Actor Blend")
	TSubclassOf<ACameraBlend> _cameraActorBlend;
};
