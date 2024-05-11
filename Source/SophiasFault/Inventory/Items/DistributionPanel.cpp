#include "DistributionPanel.h"
#include "Engine/Light.h" 
#include "Components/LightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Door/CloseDoor.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../DialogueWidget.h"

ADistributionPanel::ADistributionPanel()
{
	_bPoweredOnce = false;
}

void ADistributionPanel::UseInteraction(AItem* item)
{
	if (!_bPoweredOnce && _myGameState->_bStairPositioned) {
		_bPoweredOnce = true;

		_soundComponent->Play();

		for (auto light : _lights) {
			light->GetLightComponent()->SetVisibility(false);
		}

		if (_junkRoomDoor != nullptr) 
			_junkRoomDoor->_bDoorLocked = false;
	} else if (!_myGameState->_bStairPositioned) {
		_myGameState->_dialogueWidget->SetDialogueTextAndShow(FText::FromString("Debería encontrar algo que ilumine antes de apagar las luces de emergencia."), 3.5f);
	}
}