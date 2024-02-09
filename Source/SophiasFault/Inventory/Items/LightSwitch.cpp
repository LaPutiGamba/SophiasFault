#include "LightSwitch.h"
#include "Components/PointLightComponent.h" 

ALightSwitch::ALightSwitch()
{
}

void ALightSwitch::UseInteraction()
{
	for (auto& lights : _lightActor) {
		if (lights) {
			if (lights->GetClass()->GetName() == "PointLight") {
				lights->GetRootComponent()->ToggleVisibility();

				if (lights->GetRootComponent()->IsVisible()) {
					//GetMesh()->PlayAnimation(_animationInteract, false);
				} else {
					//GetMesh()->Pause????Animation(_animationInteract, false);
				}
			}
		}
	}
}