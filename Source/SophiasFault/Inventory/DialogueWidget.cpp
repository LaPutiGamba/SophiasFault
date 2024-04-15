#include "DialogueWidget.h"
#include "Components/TextBlock.h"

void UDialogueWidget::SetDialogueTextAndShow(FText text, float time)
{
	if (!IsInViewport()) {
		_dialogueText->SetText(text);
		AddToViewport();
		PlayAnimation(_blendIn, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
		PlayAnimation(_blendOut, time - 0.8f, 1, EUMGSequencePlayMode::Forward, 1.f);

		FTimerHandle dialogueTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(dialogueTimerHandle, [this]() {
			RemoveFromParent();
		}, time, false);
	}
}