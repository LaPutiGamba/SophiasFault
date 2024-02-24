#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
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