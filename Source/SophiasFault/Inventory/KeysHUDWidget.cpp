#include "KeysHUDWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "../Macros.h"

void UKeysHUDWidget::AddKeyToHorizontalBox(FText key, FText textToShow, bool _bClearChildren, UKeysHUDHelper::EKeysHUDHelperImage image)
{
	if (_keysHorizontalBox) {
		if (_bClearChildren)
			_keysHorizontalBox->ClearChildren();

		AGMS_MyGameStateBase* gameState = Cast<AGMS_MyGameStateBase>(GetWorld()->GetGameState());

		if (!gameState)
			return;

		UKeysHUDHelper* keysHUDHelper = CreateWidget<UKeysHUDHelper>(GetWorld(), gameState->_keysHudHelperClass);
		if (keysHUDHelper) {
			keysHUDHelper->SetKey(key, textToShow, image);
			_keysHorizontalBox->AddChild(keysHUDHelper);

			UHorizontalBoxSlot* keysSlot = Cast<UHorizontalBoxSlot>(_keysHorizontalBox->GetChildAt(_keysHorizontalBox->GetChildrenCount() - 1)->Slot);
			if (keysSlot) {
				keysSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				keysSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				keysSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}
		}
	}
}

void UKeysHUDWidget::ShowHUD(float time)
{
	if (!IsInViewport()) {
		AddToViewport();

		if (IsInViewport() && IsValid(_blendIn)) {
			PlayAnimation(_blendIn);
		}

		if (time == -1.f)
			return;

		FTimerHandle animationTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(animationTimerHandle, [this]() {
			if (IsValid(_blendOut)) {
				PlayAnimation(_blendOut);
			}
		}, time - 0.75f, false);

		FTimerHandle keysHudTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(keysHudTimerHandle, [this]() {
			RemoveFromParent();
		}, time, false);
	}
}

void UKeysHUDWidget::HideHUD(float time)
{
	if (IsInViewport()) {
		if (IsValid(_blendOut)) {
			PlayAnimation(_blendOut);
		}

		FTimerHandle removeHudTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(removeHudTimerHandle, [this]() {
			RemoveFromParent();
		}, 0.75f, false);

		if (time == -1.f)
			return;

		FTimerHandle keysHudTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(keysHudTimerHandle, [this]() {
			AddToViewport();
		}, time, false);
	}
}