#include "MirrorSolution.h"
#include "Kismet/GameplayStatics.h"
#include "../Item.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Macros.h"

AMirrorSolution::AMirrorSolution()
{
	_deskDrawerActor = nullptr;
}

void AMirrorSolution::UseInteraction()
{
	// Activate mirror light solution
	const uint8 mirrorLightResult[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

	bool correct = true;

	for (size_t i = 0; i < _myGameState->GetPositionedMirrorLights()->Num(); i++) {
		if (_myGameState->GetPositionedMirrorLights()->operator[](i) != mirrorLightResult[i]) {
			correct = false;
			break;
		}
	}

	printBool(correct);

	if (correct && _deskDrawerActor != nullptr) {
		FVector moveDrawerVector = FVector(_deskDrawerActor->GetActorLocation());
		moveDrawerVector.X += 800.f;
		_deskDrawerActor->SetActorRelativeLocation(moveDrawerVector);
	}
}