#include "Gramophone.h"
#include "../../Macros.h"

AGramophone::AGramophone()
{
	firstShape = nullptr;
	secondShape = nullptr;
	thirdShape = nullptr;
}

void AGramophone::UseInteraction()
{
	if (firstShape != nullptr && secondShape != nullptr && thirdShape != nullptr) {
		FRotator firstShapeRot = firstShape->GetActorRotation();
		FRotator secondShapeRot = secondShape->GetActorRotation();
		FRotator thirdShapeRot = thirdShape->GetActorRotation();

		if (firstShapeRot.Pitch >= 19.f && firstShapeRot.Pitch <= 21.f &&
			secondShapeRot.Pitch >= 19.f && secondShapeRot.Pitch <= 21.f &&
			thirdShapeRot.Pitch >= 19.f && thirdShapeRot.Pitch <= 21.f) {
			printText("Gramophone is playing music");
		} else {
			printText("Not playing music!");
		}
	}
}