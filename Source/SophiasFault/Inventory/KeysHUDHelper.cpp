#include "KeysHUDHelper.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Macros.h"

void UKeysHUDHelper::SetKey(FText key, FText textToShow, EKeysHUDHelperImage image)
{
	if (_keySelected && _keyDescription) {
		if (image == EKeysHUDHelperImage::KeyboardKey) {
			_keyboardKey->SetVisibility(ESlateVisibility::Visible);
			_keySelected->SetText(key);
		} else if (image == EKeysHUDHelperImage::MouseLeft) {
			_mouseLeft->SetVisibility(ESlateVisibility::Visible);
			_keySelected->SetText(FText::GetEmpty());
		} else if (image == EKeysHUDHelperImage::MouseWheel) {
			_mouseWheel->SetVisibility(ESlateVisibility::Visible);
			_keySelected->SetText(FText::GetEmpty());
		} else if (image == EKeysHUDHelperImage::MouseRight) {
			_mouseRight->SetVisibility(ESlateVisibility::Visible);
			_keySelected->SetText(FText::GetEmpty());
		}

		_keyDescription->SetText(textToShow);
	}
}