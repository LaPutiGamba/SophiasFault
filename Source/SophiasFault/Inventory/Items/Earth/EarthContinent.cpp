#include "EarthContinent.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "../../../Macros.h"

AEarthContinent::AEarthContinent()
{
	_myGameState = nullptr;

	_continentID = 0;
}

void AEarthContinent::BeginPlay()
{
	AItem::BeginPlay();

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;
}

void AEarthContinent::AddOrRemoveContinent()
{
	if (_myGameState != nullptr) {
		if (_myGameState->GetSelectedContinents()->Num() == 0) {
			_myGameState->GetSelectedContinents()->Add(_continentID);
			_meshComponent->SetRenderCustomDepth(true);
		} else {
			int value = _myGameState->GetSelectedContinents()->Find(_continentID);
			if (value == -1) {
				_myGameState->GetSelectedContinents()->Add(_continentID);
				_meshComponent->SetRenderCustomDepth(true);
			} else {
				_myGameState->GetSelectedContinents()->Remove(_continentID);
				_meshComponent->SetRenderCustomDepth(false);
				return;
			}
		}

		if (_myGameState->GetSelectedContinents()->Num() == 6)
			_myGameState->ActivateEarthSolution();
	}
}

void AEarthContinent::Reset()
{
	_meshComponent->SetRenderCustomDepth(false);
}