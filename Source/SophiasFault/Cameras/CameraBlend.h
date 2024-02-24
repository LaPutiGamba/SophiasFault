#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "../Interfaces/CameraBlendInterface.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CameraBlend.generated.h"

UCLASS()
class SOPHIASFAULT_API ACameraBlend : public ACameraActor, public ICameraBlendInterface
{
	GENERATED_BODY()
	
public:
	ACameraBlend();

	virtual void BeginPlay() override;

	virtual void UseInteraction() override;

	virtual void BlendBack() override;

	UEnhancedInputComponent* _enhancedInputComponent;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _getUpAction;

	FInputBindingHandle* _getUpHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mapping Context")
	class UInputMappingContext* _mainMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mapping Context")
	class UInputMappingContext* _puzzleMappingContext;

	APlayerController* _playerController;
	class ASophia* _sophia;
	class AGMS_MyGameStateBase* _myGameState;
};
