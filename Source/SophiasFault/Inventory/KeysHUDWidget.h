#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeysHUDHelper.h"
#include "KeysHUDWidget.generated.h"

UCLASS()
class SOPHIASFAULT_API UKeysHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UHorizontalBox* _keysHorizontalBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadWrite)
	class UWidgetAnimation* _blendIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadWrite)
	class UWidgetAnimation* _blendOut;

	bool _bClearChildren;

	void AddKeyToHorizontalBox(FText key, FText textToShow, bool bClearChildren = false, UKeysHUDHelper::EKeysHUDHelperImage image = UKeysHUDHelper::EKeysHUDHelperImage::KeyboardKey);
	void ShowHUD(float time = -1.f, bool bClearChildren = false);
	void HideHUD(float time = -1.f);
	void ClearChildrenFromBox();
};
