#include "GMS_MyGameStateBase.h"
#include "../Macros.h"
#include "Kismet/GameplayStatics.h"
#include "../Inventory/Item.h"
#include "../Inventory/Items/Earth/EarthContinent.h"
#include "../Inventory/Items/Mirror Light/MirrorSolution.h"
#include "../Cameras/EarthBallCamera.h"
#include "../Cameras/PianoCamera.h"
#include "../Inventory/DialogueWidget.h"
#include "../Inventory/KeysHUDWidget.h"
#include "../Inventory/KeysHUDHelper.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" 

AGMS_MyGameStateBase::AGMS_MyGameStateBase()
{
	_bOnChase = false;
	_bPianoPuzzleSolved = false;
	_bStairPositioned = true;
	_onBlendTime = 0.f;
		
	const uint8 PianoResult[] = { 3, 13, 6, 8, 6, 13, 8, 10, 10 };
	_pianoKeysResult.Append(PianoResult, UE_ARRAY_COUNT(PianoResult));

	const uint8 MirrorLightResult[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	_positionedMirrorLights.Append(MirrorLightResult, UE_ARRAY_COUNT(MirrorLightResult));

	_hudWidget = nullptr;
	_keysHudWidget = nullptr;
	_dialogueWidget = nullptr;
	_noteWidget = nullptr;
	_noteWidgetText = nullptr;

	ConstructorHelpers::FClassFinder<UUserWidget> hudFinderClass(TEXT("/Game/Items/Widgets/WBP_HUD"));
	if (hudFinderClass.Succeeded())
		_hudWidgetClass = hudFinderClass.Class;
	else
		_hudWidgetClass = nullptr;

	ConstructorHelpers::FClassFinder<UKeysHUDWidget> hudKeysFinderClass(TEXT("/Game/Items/Widgets/WBP_KeysHUD"));
	if (hudKeysFinderClass.Succeeded())
		_keysHudWidgetClass = hudKeysFinderClass.Class;
	else
		_keysHudWidgetClass = nullptr;

	ConstructorHelpers::FClassFinder<UKeysHUDHelper> hudKeysHelperFinderClass(TEXT("/Game/Items/Widgets/WBP_KeysHUDHelper"));
	if (hudKeysHelperFinderClass.Succeeded())
		_keysHudHelperClass = hudKeysHelperFinderClass.Class;
	else
		_keysHudHelperClass = nullptr;

	ConstructorHelpers::FClassFinder<UUserWidget> dialogueFinderClass(TEXT("/Game/Items/Widgets/WBP_Dialogue"));
	if (dialogueFinderClass.Succeeded())
		_dialogueWidgetClass = dialogueFinderClass.Class;
	else
		_dialogueWidgetClass = nullptr;

	ConstructorHelpers::FClassFinder<UUserWidget> noteFinderClass(TEXT("/Game/Items/Widgets/WBP_Note"));
	if (noteFinderClass.Succeeded())
		_noteWidgetClass = noteFinderClass.Class;
	else
		_noteWidgetClass = nullptr;
}

void AGMS_MyGameStateBase::BeginPlay() 
{
	Super::BeginPlay();

	_bOnChase = false;
	
	if (APlayerController* playerController = GetWorld()->GetFirstPlayerController()) {
		if (_dialogueWidgetClass && _noteWidgetClass) {
			_keysHudWidget = CreateWidget<UKeysHUDWidget>(playerController, _keysHudWidgetClass);
			_dialogueWidget = CreateWidget<UDialogueWidget>(playerController, _dialogueWidgetClass);

			_noteWidget = CreateWidget<UUserWidget>(playerController, _noteWidgetClass);
			if (_noteWidget != nullptr) {
				_noteWidgetText = Cast<UTextBlock>(_noteWidget->GetWidgetFromName("NoteText"));

				if (_noteWidgetText != nullptr)
					_noteWidgetText->SetText(FText::FromString("I'm sorry, Sophia. I've been trying to fix my mistakes. I've left you a series of puzzles to solve. I hope you can forgive me. I love you."));
			}
		}
	}
}

void AGMS_MyGameStateBase::ActivatePianoSolution()
{
	TArray<AActor*> actorsToFind;

	if (UWorld* world = GetWorld())
		UGameplayStatics::GetAllActorsOfClass(world, APianoCamera::StaticClass(), actorsToFind);
	
	APianoCamera* pianoCamera = nullptr;

	for (AActor* pianoCameraActor : actorsToFind) {
		if (pianoCameraActor->IsA<APianoCamera>()) {
			pianoCamera = Cast<APianoCamera>(pianoCameraActor);
			break;
		}
	}

	if (pianoCamera != nullptr)
		pianoCamera->ActivatePianoSolution();
}

void AGMS_MyGameStateBase::ActivateEarthSolution()
{
	// Order of the eart solution 
	const uint8 earthResult[] = { 1, 2, 3, 4, 5, 6 };
	
	if (_selectedContinents.Num() == 6) {
		bool correct = true;

		for (size_t i = 0; i < _selectedContinents.Num(); i++) {
			if (_selectedContinents[i] != earthResult[i]) {
				correct = false;
				break;
			}
		}

		if (correct) {
			TArray<AActor*> actorsToFind;
			
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AEarthBallCamera::StaticClass(), "EarthCamera", actorsToFind);
			AEarthBallCamera* earthCamera = nullptr;
			for (AActor* earthCameraActor : actorsToFind) {
				if ((earthCamera = Cast<AEarthBallCamera>(earthCameraActor)) != nullptr) {
					earthCamera->_bPuzzleSolved = true;
					break;
				}
			}

			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "EarthDeskDrawer", actorsToFind);

			AItem* deskDrawerCast = nullptr;
			for (AActor* deskDrawerActor : actorsToFind) {
				if ((deskDrawerCast = Cast<AItem>(deskDrawerActor)) != nullptr) {
					FVector moveDrawerVector = FVector(deskDrawerCast->GetActorLocation());
					moveDrawerVector.X += 800.f;
					deskDrawerCast->SetActorRelativeLocation(moveDrawerVector);
					break;
				}
			}

			if (UWorld* world = GetWorld())
				UGameplayStatics::GetAllActorsOfClass(world, AEarthContinent::StaticClass(), actorsToFind);

			for (AActor* actor : actorsToFind) {
				if (AEarthContinent* continent = Cast<AEarthContinent>(actor))
					continent->_meshComponent->SetRenderCustomDepth(false);
			}
		} else {
			_selectedContinents.Reset();

			TArray<AActor*> actorsToFind;

			if (UWorld* world = GetWorld())
				UGameplayStatics::GetAllActorsOfClass(world, AEarthContinent::StaticClass(), actorsToFind);

			for (AActor* actor : actorsToFind) {
				if (AEarthContinent* continent = Cast<AEarthContinent>(actor))
					continent->Reset();
			}
		}
	}
}