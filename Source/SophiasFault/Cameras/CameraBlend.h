#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "../Interfaces/InteractiveInterface.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CameraBlend.generated.h"

UCLASS()
class SOPHIASFAULT_API ACameraBlend : public ACameraActor, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	ACameraBlend();

	virtual void BeginPlay() override;

	virtual void BlendBack();

	UEnhancedInputComponent* _enhancedInputComponent;

	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	class UInputAction* _getUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mapping Context")
	class UInputMappingContext* _mainMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mapping Context")
	class UInputMappingContext* _puzzleMappingContext;

	APlayerController* _playerController;
	class AGMS_MyGameStateBase* _myGameState;
};
