#include "DialogueWidget.h"
#include "Components/TextBlock.h"

void UDialogueWidget::SetDialogueTextAndShow(FText text, float time)
{
	if (!IsInViewport()) {
		_dialogueText->SetText(text);
		AddToViewport();

		if (IsInViewport() && IsValid(_blendIn)) {
			PlayAnimation(_blendIn);
		}

		FTimerHandle animationTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(animationTimerHandle, [this]() {
			if (IsValid(_blendOut)) {
				PlayAnimation(_blendOut);
			}
		}, time - 0.75f, false);

		FTimerHandle dialogueTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(dialogueTimerHandle, [this]() {
			RemoveFromParent();
		}, time, false);
	}
}