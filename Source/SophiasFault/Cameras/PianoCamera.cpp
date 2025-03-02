#include "PianoCamera.h"
#include "../Inventory/Items/Piano/PianoKey.h"
#include "../Inventory/Items/Door/DoorKey.h"
#include "../Interfaces/PickUpInterface.h"
#include "Components/AudioComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" 
#include "../Inventory/KeysHUDWidget.h"
#include "Engine/RectLight.h" 
#include "../Sophia.h"
#include "../Macros.h"

APianoCamera::APianoCamera()
{
}

void APianoCamera::UseInteraction(ACameraBlend* item)
{
	if (_myGameState->_onBlendTime <= 0.001f) {
		ACameraBlend::UseInteraction(item);

		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->SetInputMode(FInputModeGameAndUI());

		FTimerHandle blendCameraHandle;
		GetWorld()->GetTimerManager().SetTimer(blendCameraHandle, [this]() {
			_playerController->bShowMouseCursor = true;

			_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("W"), FText::FromString("Piano Sheet"), true);
			_myGameState->_keysHudWidget->ShowHUD();
		}, 0.8f, false);

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &APianoCamera::BlendBack);
			_clickInteractiveHandle = &_enhancedInputComponent->BindAction(_clickInteractiveAction, ETriggerEvent::Triggered, this, &APianoCamera::ClickInteractive);
			_blendCameraHandle = &_enhancedInputComponent->BindAction(_blendCameraAction, ETriggerEvent::Triggered, this, &APianoCamera::LookPianoSheet);
		}
	}
}

void APianoCamera::BlendBack()
{
	ACameraBlend::BlendBack();
	_playerController->SetInputMode(FInputModeGameOnly());
	
	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
		_enhancedInputComponent->RemoveBinding(*_blendCameraHandle);
	}

	Cast<ASophia>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetInventory()->_currentChangeCameraItem = nullptr;

	if (_myGameState->_keysHudWidget != nullptr) {
		_myGameState->_keysHudWidget->RemoveFromParent();
	}
}

void APianoCamera::ClickInteractive(const FInputActionValue& value)
{
	_playerController->bShowMouseCursor = true;

	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	FHitResult _hit;
	if (GetWorld()->LineTraceSingleByChannel(_hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (_hit.GetActor()->IsA<APianoKey>()) {
			APianoKey* mesh = Cast<APianoKey>(_hit.GetActor());
			mesh->UseInteraction();
		} else if (_hit.GetActor()->IsA<ADoorKey>()) {
			if (_myGameState->GetPianoPuzzleSolved()) {
				IPickUpInterface* doorKey = Cast<IPickUpInterface>(_hit.GetActor());
				doorKey->PickUpItem(Cast<AItem>(_hit.GetActor()));
			}
		}
	}
}

void APianoCamera::LookPianoSheet(const FInputActionValue& value)
{
	bool bIsPianoSheet = value.Get<bool>();

	if (_pianoHollowKey->_timelineComponent->IsPlaying())
		return;

	FTimerHandle blendCameraHandle;
	if (bIsPianoSheet) {
		GetWorld()->GetTimerManager().SetTimer(blendCameraHandle, [this]() {
			_playerController->SetViewTargetWithBlend(_sheetCamera, 0.5f);
			_myGameState->_onBlendTime = 0.5f;
			_playerController->bShowMouseCursor = false;
			_playerController->bEnableClickEvents = false;
			_playerController->bEnableMouseOverEvents = false;
		}, _myGameState->_onBlendTime, false);
	} else {
		GetWorld()->GetTimerManager().SetTimer(blendCameraHandle, [this]() {
			_playerController->SetViewTargetWithBlend(this, 0.5f);
			_myGameState->_onBlendTime = 0.5f;
			_playerController->bShowMouseCursor = true;
			_playerController->bEnableClickEvents = true;
			_playerController->bEnableMouseOverEvents = true;
		}, _myGameState->_onBlendTime, false);
	}
}

void APianoCamera::ActivatePianoSolution()
{
	if (_pianoHollowKey != nullptr) {
		_pianoHollowKey->_curveFloat = _curveFloatFinal;
		_pianoHollowKey->_timelineComponent->AddInterpFloat(_pianoHollowKey->_curveFloat, _pianoHollowKey->_timelineCallback);
		_pianoHollowKey->_timelineComponent->PlayFromStart();

		if (_pianoHollowKey->_soundComponent != nullptr) {
			_pianoHollowKey->_soundComponent->SetIntParameter("Piano Note", _pianoHollowKey->_pianoKeyID);
			_pianoHollowKey->_soundComponent->Play();
		}

		FTimerHandle drawerTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(drawerTimerHandle, [this]() {
			_drawer->AddActorLocalOffset(FVector(30.f, 0.f, 0.f));
			_keyLight->SetActorHiddenInGame(false);
		}, 10.f, false);
	}
}
