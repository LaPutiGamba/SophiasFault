#include "Notes.h"
#include "../../Sophia.h"
#include "Blueprint/UserWidget.h" 
#include "Components/TextBlock.h" 
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h" 

ANotes::ANotes()
{
	PrimaryActorTick.bCanEverTick = true;
	_playerController = nullptr;

	ConstructorHelpers::FClassFinder<UUserWidget> noteFinderClass(TEXT("/Game/Items/Widgets/WBP_Note"));
	if (noteFinderClass.Succeeded())
		_noteWidgetClass = noteFinderClass.Class;
	else
		_noteWidgetClass = nullptr;

	_noteWidget = nullptr;
}

void ANotes::BeginPlay()
{
	Super::BeginPlay();

	_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (_noteWidgetClass) {
		if ((_noteWidget = CreateWidget<UUserWidget>(GetWorld(), _noteWidgetClass)) != nullptr) {
			if (UTextBlock* noteTextBlock = Cast<UTextBlock>(_noteWidget->GetWidgetFromName("NoteText")))
				noteTextBlock->SetText(_noteText);
		}
	}
}

void ANotes::OnAction()
{
	if (_noteWidget) {
		if (!_noteWidget->IsInViewport()) {
			_bNoSwitchableItem = true;
			_owningInventory->_bInspecting = true;
			_playerController->SetInputMode(FInputModeGameAndUI());
			_noteWidget->AddToViewport();
			_playerController->SetIgnoreLookInput(true);
			_playerController->SetIgnoreMoveInput(true);
		} else {
			_bNoSwitchableItem = false;
			_owningInventory->_bInspecting = false;
			_noteWidget->RemoveFromParent();
			_playerController->SetInputMode(FInputModeGameOnly());
			_playerController->bShowMouseCursor = false;
			_playerController->SetIgnoreLookInput(false);
			_playerController->SetIgnoreMoveInput(false);
		}
	}
}