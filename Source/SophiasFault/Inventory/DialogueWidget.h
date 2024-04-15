#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

UCLASS()
class SOPHIASFAULT_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* _dialogueText;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadWrite)
	class UWidgetAnimation* _blendIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadWrite)
	class UWidgetAnimation* _blendOut;

	void SetDialogueTextAndShow(FText text, float time);
};
