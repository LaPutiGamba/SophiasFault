#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeysHUDHelper.generated.h"

UCLASS()
class SOPHIASFAULT_API UKeysHUDHelper : public UUserWidget
{
	GENERATED_BODY()
	
public:
	enum class EKeysHUDHelperImage
	{
		KeyboardKey,
		MouseLeft, 
		MouseWheel,
		MouseRight
	};

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* _keyboardKey;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* _mouseLeft;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* _mouseWheel;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage* _mouseRight;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* _keySelected;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock* _keyDescription;

	EKeysHUDHelperImage _imageType;

	void SetKey(FText key, FText textToShow, EKeysHUDHelperImage image = EKeysHUDHelperImage::KeyboardKey);
};
