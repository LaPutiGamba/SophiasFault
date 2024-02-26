#include "PianoCamera.h"
#include "../Inventory/Items/Piano/PianoKey.h"
#include "../Inventory/Items/Door/DoorKey.h"
#include "Components/AudioComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "../Sophia.h"
#include "../Macros.h"

#define ECC_Selectable ECC_GameTraceChannel1

APianoCamera::APianoCamera()
{
}

void APianoCamera::UseInteraction()
{
	if (_myGameState->_onBlendTime <= 0.001f) {
		ACameraBlend::UseInteraction();

		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->bShowMouseCursor = true;
		_playerController->bEnableClickEvents = true;
		_playerController->bEnableMouseOverEvents = true;

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

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
		_enhancedInputComponent->RemoveBinding(*_blendCameraHandle);
	}

	Cast<ASophia>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetInventory()->_currentChangeCameraItem = nullptr;
}

void APianoCamera::ClickInteractive(const FInputActionValue& value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	FHitResult _hit;
	if (GetWorld()->LineTraceSingleByChannel(_hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Selectable, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (_hit.GetActor()->IsA<APianoKey>()) {
			APianoKey* mesh = Cast<APianoKey>(_hit.GetActor());
			mesh->UseInteraction();
		}
	}
}

void APianoCamera::LookPianoSheet(const FInputActionValue& value)
{
	bool bIsPianoSheet = value.Get<bool>();

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
		}, 10.f, false);
	}
}
