#include "Item.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../../Macros.h"

AItem::AItem()
{
	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(_meshComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	_myCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	_playerCamera = _myCharacter->FindComponentByClass<UCameraComponent>();

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