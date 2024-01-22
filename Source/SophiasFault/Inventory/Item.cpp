#include "Item.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../Sophia.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "../Macros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(_meshComponent);

	_timelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));

	_name = "Item";

	_soundCue = nullptr;
	_soundComponent = nullptr;
	_owningInventory = nullptr;
	_myCharacter = nullptr;
	_playerCamera = nullptr;
	_itemComponent = nullptr;
	_bNoSwitchableItem = false;
	_myGameState = nullptr;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;

	_myCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	_playerCamera = _myCharacter->FindComponentByClass<UCameraComponent>();
	_owningInventory = Cast<ASophia>(_myCharacter)->GetInventory();

	TArray<USceneComponent*> components;
	_myCharacter->GetComponents(components);

	if (components.Num() > 0) {
		for (auto& comp : components) {
			if (comp->GetName() == "HoldingComponent") {
				_itemComponent = Cast<USceneComponent>(comp);
				break;
			}
		}
	}
}