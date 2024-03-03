#include "PianoSheet.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h" 

APianoSheet::APianoSheet()
{
}

void APianoSheet::BeginPlay()
{
	Super::BeginPlay();
}

void APianoSheet::OnAction()
{
	FHitResult hit;
	FVector start, forwardVector, end;
	if (_playerCamera != nullptr)
		start = _playerCamera->GetComponentLocation();
	if (_playerCamera != nullptr)
		forwardVector = _playerCamera->GetForwardVector();
	end = ((forwardVector * 200.f) + start);

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (hit.GetActor()->ActorHasTag("PianoSheet")) {
			if (UBoxComponent* boxComponent = Cast<UBoxComponent>(hit.GetActor()->GetComponentByClass(UBoxComponent::StaticClass())))
				boxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			AttachToActor(hit.GetActor(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			SetActorRelativeLocation(FVector(10.f, 70.f, 1.f));
			_meshComponent->SetVisibility(true);
			_owningInventory->RemoveItem(this);
		}
	}
}