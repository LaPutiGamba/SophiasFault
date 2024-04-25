#include "DistributionPanel.h"
#include "Engine/Light.h" 
#include "Components/LightComponent.h"
#include "Kismet/GameplayStatics.h"

ADistributionPanel::ADistributionPanel()
{
	_bPoweredOnce = false;
}

void ADistributionPanel::UseInteraction(AItem* item)
{
	if (!_bPoweredOnce) {
		_bPoweredOnce = true;

		_soundComponent->Play();

		for (auto light : _lights) {
			light->GetLightComponent()->SetVisibility(false);
		}
	}
}