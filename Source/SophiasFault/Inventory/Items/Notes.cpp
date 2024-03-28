#include "Notes.h"
#include "../../Sophia.h"
#include "Blueprint/UserWidget.h" 
#include "Components/TextBlock.h" 
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h" 
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Macros.h"

ANotes::ANotes()
{
	PrimaryActorTick.bCanEverTick = true;
	_playerController = nullptr;
}

void ANotes::BeginPlay()
{
	Super::BeginPlay();

	_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ANotes::OnAction()
{
	if (_myGameState != nullptr) {
		if (_myGameState->_noteWidget != nullptr && !_myGameState->_noteWidget->IsPendingKill() && _playerController && _owningInventory->_bInspecting) {
			if (!_myGameState->_noteWidget->IsInViewport()) {
				if (_myGameState != nullptr) {
					if (_myGameState->_noteWidget != nullptr && _myGameState->_noteWidgetText != nullptr)
						_myGameState->_noteWidgetText->SetText(_noteText);
				}
				_bNoSwitchableItem = true;
				_playerController->SetInputMode(FInputModeGameAndUI());
				_myGameState->_noteWidget->AddToViewport();
				_playerController->SetIgnoreLookInput(true);
				_playerController->SetIgnoreMoveInput(true);
			} else {
				_bNoSwitchableItem = false;
				_myGameState->_noteWidget->RemoveFromParent();
				_playerController->SetInputMode(FInputModeGameOnly());
				_playerController->bShowMouseCursor = false;
				_playerController->SetIgnoreLookInput(false);
				_playerController->SetIgnoreMoveInput(false);
			}
		}
	}
}