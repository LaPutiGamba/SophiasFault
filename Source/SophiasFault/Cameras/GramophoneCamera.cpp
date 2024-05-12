#include "GramophoneCamera.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "../Inventory/KeysHUDWidget.h"
#include "../Inventory/Items/Gramophone/GramophoneShape.h"
#include "../Inventory/Items/Gramophone/Gramophone.h"

AGramophoneCamera::AGramophoneCamera()
{
}

void AGramophoneCamera::UseInteraction(ACameraBlend* item)
{
	if (_myGameState->_onBlendTime <= 0.001f) {
		ACameraBlend::UseInteraction(item);

		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->SetInputMode(FInputModeGameAndUI());

		_playerController->bShowMouseCursor = true;
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::GetEmpty(), FText::FromString("Rotate / Play"), false, UKeysHUDHelper::EKeysHUDHelperImage::MouseLeft);
		_myGameState->_keysHudWidget->ShowHUD();

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &AGramophoneCamera::BlendBack);
			_interactHandle = &_enhancedInputComponent->BindAction(_interactAction, ETriggerEvent::Triggered, this, &AGramophoneCamera::ClickInteractive);
		}
	}
}

void AGramophoneCamera::BlendBack()
{
	ACameraBlend::BlendBack();
	_playerController->SetInputMode(FInputModeGameOnly());

	if (_enhancedInputComponent)
		_enhancedInputComponent->RemoveBinding(*_interactHandle);

	if (_myGameState->_keysHudWidget != nullptr) {
		_myGameState->_keysHudWidget->SetClearChildren(true);
		_myGameState->_keysHudWidget->ClearChildrenFromBox();
		_myGameState->_keysHudWidget->RemoveFromParent();
	}
}

void AGramophoneCamera::ClickInteractive(const FInputActionValue& value)
{
	_playerController->bShowMouseCursor = true;

	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	FHitResult hit;
	if (GetWorld()->LineTraceSingleByChannel(hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (hit.GetActor()->IsA<AGramophoneShape>()) {
			AGramophoneShape* mesh = Cast<AGramophoneShape>(hit.GetActor());
			mesh->UseInteraction();
		} else if (hit.GetActor()->IsA<AGramophone>()) {
			AGramophone* mesh = Cast<AGramophone>(hit.GetActor());
			mesh->UseInteraction();
		}
	}
}
