#include "AnimatedItem.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "../Sophia.h"
#include "../Macros.h"

AAnimatedItem::AAnimatedItem()
{
	PrimaryActorTick.bCanEverTick = true;

	_skeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Skeletal Mesh"));
	SetRootComponent(_skeletalMeshComponent);

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

	_animationSequence = nullptr;
}

void AAnimatedItem::BeginPlay()
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