#include "EarthContinent.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "../../../Macros.h"

AEarthContinent::AEarthContinent()
{
	_myGameState = nullptr;

	_continentID = 0;
	
	_defaultMaterial = nullptr;
	_outlineMaterial = nullptr;
}

void AEarthContinent::BeginPlay()
{
	AItem::BeginPlay();

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;

	if (_defaultMaterial != nullptr)
		_meshComponent->SetMaterial(0, _defaultMaterial);
	_meshComponent->SetMaterial(1, nullptr);
}

void AEarthContinent::UseInteraction()
{
	if (_myGameState != nullptr) {
		if (_myGameState->GetSelectedContinents()->Num() == 0) {
			_myGameState->GetSelectedContinents()->Add(_continentID);

			if (_meshComponent->GetNumMaterials() >= 2 && _outlineMaterial != nullptr)
				_meshComponent->SetMaterial(1, _outlineMaterial);
		} else {
			int value = _myGameState->GetSelectedContinents()->Find(_continentID);
			if (value == -1) {
				_myGameState->GetSelectedContinents()->Add(_continentID);

				if (_meshComponent->GetNumMaterials() >= 2 && _outlineMaterial != nullptr)
					_meshComponent->SetMaterial(1, _outlineMaterial);
			} else {
				_myGameState->GetSelectedContinents()->Remove(_continentID);

				_meshComponent->SetMaterial(1, nullptr);
				return;
			}
		}

		if (_myGameState->GetSelectedContinents()->Num() == 6)
			_myGameState->ActivateEarthSolution();
	}
}

void AEarthContinent::Reset()
{
	_meshComponent->SetMaterial(1, nullptr);
}



/*
#include "EarthContinent.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "../../../Macros.h"

AEarthContinent::AEarthContinent()
{
	_myGameState = nullptr;

	_continentID = 0;

	_defaultMaterial = nullptr;
	_outlineMaterial = nullptr;
}

void AEarthContinent::BeginPlay()
{
	AItemInteractive::BeginPlay();

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;
}

void AEarthContinent::UseInteraction()
{
	if (_myGameState != nullptr) {
		if (_myGameState->GetSelectedContinents()->Num() == 0) {
			_myGameState->GetSelectedContinents()->Add(_continentID);

			if (_outlineMaterial != nullptr)
				_meshComponent->SetMaterial(0, _outlineMaterial);

			return;
		} else {
			int value = _myGameState->GetSelectedContinents()->Find(_continentID);
			if (value == -1) {
				_myGameState->GetSelectedContinents()->Add(_continentID);

				if (_outlineMaterial != nullptr)
					_meshComponent->SetMaterial(0, _outlineMaterial);
			} else {
				_myGameState->GetSelectedContinents()->Remove(_continentID);

				if (_defaultMaterial != nullptr)
					_meshComponent->SetMaterial(0, _defaultMaterial);

				return;
			}
		}

		if (_myGameState->GetSelectedContinents()->Num() == 6)
			_myGameState->ActivateEarthSolution();
	}
}

void AEarthContinent::Reset()
{
	if (_myGameState != nullptr) {
		_myGameState->GetSelectedContinents()->Reset();

		if (_defaultMaterial != nullptr)
			_meshComponent->SetMaterial(0, _defaultMaterial);
	}
}
*/