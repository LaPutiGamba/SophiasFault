#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Animation/WidgetAnimation.h" 
#include "GMS_MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AGMS_MyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

private:
	bool _bOnChase;
	bool _bPianoPuzzleSolved;
	TArray<uint8> _pianoKeysPressed;
	TArray<uint8> _pianoKeysResult;
	TArray<uint8> _selectedContinents;
	TArray<uint8> _positionedMirrorLights;

public:
	AGMS_MyGameStateBase();

	virtual void BeginPlay() override;
	
	float _onBlendTime;
	bool _bStairPositioned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* _hudWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKeysHUDWidget* _keysHudWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDialogueWidget* _dialogueWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UUserWidget* _noteWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTextBlock* _noteWidgetText;

	TSubclassOf<class UUserWidget> _hudWidgetClass;
	TSubclassOf<class UKeysHUDWidget> _keysHudWidgetClass;
	TSubclassOf<class UKeysHUDHelper> _keysHudHelperClass;
	TSubclassOf<class UUserWidget> _noteWidgetClass;
	TSubclassOf<class UDialogueWidget> _dialogueWidgetClass;

	// AI functions
	void SetOnChase(bool value) { _bOnChase = value; }
	bool GetOnChase() { return _bOnChase; }

	// Piano functions
	void SetPianoPuzzleSolved(bool value) { _bPianoPuzzleSolved = value; }
	bool GetPianoPuzzleSolved() { return _bPianoPuzzleSolved; }

	/***** PUZZLE *****/
	// Piano functions
	TArray<uint8>* GetPianoKeysPressed() { return &_pianoKeysPressed; }
	TArray<uint8>* GetPianoKeysResult() { return &_pianoKeysResult; }
	void ActivatePianoSolution();

	// Continents functions
	TArray<uint8>* GetSelectedContinents() { return &_selectedContinents; }
	void ActivateEarthSolution();

	// Mirror Light functions
	TArray<uint8>* GetPositionedMirrorLights() { return &_positionedMirrorLights; }
};