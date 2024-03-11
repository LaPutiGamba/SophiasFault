#include "CameraBlend.h"
#include "Blueprint/UserWidget.h" 
#include "../Inventory/InventoryComponent.h"
#include "../Inventory/Item.h"
#include "../Macros.h"
#include "../Sophia.h"

ACameraBlend::ACameraBlend()
{
	_playerController = nullptr;
	_myGameState = nullptr;
	_enhancedInputComponent = nullptr;
	_bPuzzleSolved = false;
}

void ACameraBlend::BeginPlay()
{
	Super::BeginPlay();

	_playerController = GetWorld()->GetFirstPlayerController();
	_sophia = Cast<ASophia>(_playerController->GetPawn());
	_myGameState = Cast<AGMS_MyGameStateBase>(GetWorld()->GetGameState());

	_enhancedInputComponent = Cast<UEnhancedInputComponent>(_playerController->InputComponent);
}

void ACameraBlend::UseInteraction() 
{
	if (_sophia->GetInventory()->_currentHandItem != nullptr)
		_sophia->GetInventory()->_currentHandItem->_itemComponent->SetVisibility(false);

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
		subsystem->RemoveMappingContext(_mainMappingContext);
		subsystem->AddMappingContext(_puzzleMappingContext, 0);
	}

	if (_myGameState->_hudWidget != nullptr) {
		_myGameState->_hudWidget->RemoveFromParent();
		_myGameState->_hudWidget = nullptr;
	}
}

void ACameraBlend::BlendBack()
{
    if (_myGameState->_onBlendTime <= 0.001f) {
		if (_sophia->GetInventory()->_currentHandItem != nullptr)
			_sophia->GetInventory()->_currentHandItem->_itemComponent->SetVisibility(true);

        _playerController->SetViewTargetWithBlend(_sophia, 0.75f);
        _myGameState->_onBlendTime = 0.75f;
        _playerController->bShowMouseCursor = false;

        if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
            subsystem->RemoveMappingContext(_puzzleMappingContext);
            subsystem->AddMappingContext(_mainMappingContext, 0);
        }

        if (_enhancedInputComponent)
            _enhancedInputComponent->RemoveBinding(*_getUpHandle);

        if (_myGameState->_hudWidget == nullptr) {
            _myGameState->_hudWidget = CreateWidget<UUserWidget>(_playerController, _myGameState->_hudWidgetClass);
            _myGameState->_hudWidget->AddToViewport();
        }
    }
}
