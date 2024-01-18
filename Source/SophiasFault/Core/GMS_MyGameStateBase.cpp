#include "GMS_MyGameStateBase.h"
#include "../Macros.h"
#include "Kismet/GameplayStatics.h"
#include "../Inventory/Items/Item.h"
#include "../Inventory/Items/InteractiveItems/EarthContinent.h"
#include "../Inventory/Items/PhysicItems/MirrorLight.h"

AGMS_MyGameStateBase::AGMS_MyGameStateBase()
{
	_bOnChase = false;
	_bPianoPuzzleSolved = false;

	const uint8 PianoResult[] = { 1, 3, 3, 5, 8, 10, 13, 11, 9, 5, 1 };
	_pianoKeysResult.Append(PianoResult, UE_ARRAY_COUNT(PianoResult));
}

void AGMS_MyGameStateBase::BeginPlay() 
{
	Super::BeginPlay();

	_bOnChase = true;
}

void AGMS_MyGameStateBase::ActivatePianoSolution()
{
	TArray<AActor*> actorsToFind;

	if (UWorld* world = GetWorld())
		UGameplayStatics::GetAllActorsOfClass(world, AItem::StaticClass(), actorsToFind);
	
	AItem* deskDrawerCast = nullptr;

	for (AActor* deskDrawerActor : actorsToFind) {
		if (deskDrawerActor->ActorHasTag("PianoKeyDeskDrawer")) 
			deskDrawerCast = Cast<AItem>(deskDrawerActor);
	}

	if (deskDrawerCast != nullptr) {
		FVector moveDrawerVector = FVector(deskDrawerCast->GetActorLocation());
		moveDrawerVector.X += 800.f;
		deskDrawerCast->SetActorRelativeLocation(moveDrawerVector);
	}
}

void AGMS_MyGameStateBase::ActivateEarthSolution()
{
	// Order of the eart solution 
	const uint8 earthResult[] = { 1, 2, 3, 4, 5, 6 };
	
	if (_selectedContinents.Num() == 6) {
		bool correct = true;

		for (size_t i = 0; i < _selectedContinents.Num(); i++) {
			if (_selectedContinents[i] != earthResult[i]) {
				correct = false;
				break;
			}
		}

		if (correct) {
			TArray<AActor*> actorsToFind;

			if (UWorld* world = GetWorld())
				UGameplayStatics::GetAllActorsOfClass(world, AItem::StaticClass(), actorsToFind);

			AItem* deskDrawerCast = nullptr;

			for (AActor* deskDrawerActor : actorsToFind) {
				if (deskDrawerActor->ActorHasTag("EarthDeskDrawer"))
					deskDrawerCast = Cast<AItem>(deskDrawerActor);
			}

			if (deskDrawerCast != nullptr) {
				FVector moveDrawerVector = FVector(deskDrawerCast->GetActorLocation());
				moveDrawerVector.X += 800.f;
				deskDrawerCast->SetActorRelativeLocation(moveDrawerVector);
			}
		} else {
			_selectedContinents.Reset();

			TArray<AActor*> actorsToFind;

			if (UWorld* world = GetWorld())
				UGameplayStatics::GetAllActorsOfClass(world, AEarthContinent::StaticClass(), actorsToFind);

			for (AActor* actor : actorsToFind) {
				AEarthContinent* continent = Cast<AEarthContinent>(actor);
				continent->Reset();
			}
		}
	}
}

void AGMS_MyGameStateBase::ActivateMirrorLightSolution()
{
	const uint8 mirrorLightResult[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

	bool correct = true;

	for (size_t i = 0; i < _positionedMirrorLights.Num(); i++) {
		if (_positionedMirrorLights[i] != mirrorLightResult[i]) {
			correct = false;
			break;
		}
	}

	printBool(correct);

	if (correct) {
		TArray<AActor*> actorsToFind;

		if (UWorld* world = GetWorld())
			UGameplayStatics::GetAllActorsOfClass(world, AItem::StaticClass(), actorsToFind);

		AItem* deskDrawerCast = nullptr;

		for (AActor* deskDrawerActor : actorsToFind) {
			if (deskDrawerActor->ActorHasTag("MirrorLightDeskDrawer"))
				deskDrawerCast = Cast<AItem>(deskDrawerActor);
		}

		if (deskDrawerCast != nullptr) {
			FVector moveDrawerVector = FVector(deskDrawerCast->GetActorLocation());
			moveDrawerVector.X += 800.f;
			deskDrawerCast->SetActorRelativeLocation(moveDrawerVector);
		}
	} else {
		_positionedMirrorLights.Reset();
	}
}