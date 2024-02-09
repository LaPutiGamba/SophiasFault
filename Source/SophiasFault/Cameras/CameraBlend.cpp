#include "CameraBlend.h"

ACameraBlend::ACameraBlend()
{
	_playerController = nullptr;
	_myGameState = nullptr;
}

void ACameraBlend::BeginPlay()
{
	Super::BeginPlay();

	_playerController = GetWorld()->GetFirstPlayerController();
	_myGameState = Cast<AGMS_MyGameStateBase>(GetWorld()->GetGameState());

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(_playerController->InputComponent);
}

void ACameraBlend::BlendBack()
{
	_playerController->SetViewTargetWithBlend(_playerController->GetViewTarget(), 0.75f);
	_myGameState->_onBlendTime = 0.75f;
	_playerController->bShowMouseCursor = false;
	_playerController->bEnableClickEvents = false;
	_playerController->bEnableMouseOverEvents = false;

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
		subsystem->RemoveMappingContext(_puzzleMappingContext);
		subsystem->AddMappingContext(_mainMappingContext, 0);
	}
}
