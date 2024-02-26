#include "Item.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "../Sophia.h"
#include "../Macros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(_meshComponent);

	_timelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));

	_name = "Item";

	_soundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Component"));
	_soundComponent->bAutoActivate = false;
	_soundComponent->SetupAttachment(RootComponent);
	_soundComponent->SetRelativeLocation(FVector(0.f, 0.0f, 0.0f));

	_metaSound = nullptr;
	_owningInventory = nullptr;
	_myCharacter = nullptr;
	_playerCamera = nullptr;
	_itemComponent = nullptr;
	_bNoSwitchableItem = false;
	_myGameState = nullptr;

	_pickUpLocation = FVector(50.f, 30.f, -12.f);
	_inspectDistance = 100.0f;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;

	_myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	_playerCamera = Cast<ASophia>(_myCharacter)->GetCameraComponent();
	_owningInventory = Cast<ASophia>(_myCharacter)->GetInventory();
	_itemComponent = Cast<ASophia>(_myCharacter)->GetHoldingComponent();

	if (_metaSound != nullptr && _soundComponent != nullptr)
		_soundComponent->SetSound(_metaSound);
}