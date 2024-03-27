#include "SimpleAnimatedItem.h"
#include "Curves/CurveFloat.h" 
#include "Components/SkeletalMeshComponent.h"
#include "../../Macros.h"

ASimpleAnimatedItem::ASimpleAnimatedItem()
{
	_bReadyState = false;
	_bCanReverse = false;
}

void ASimpleAnimatedItem::BeginPlay()
{
	Super::BeginPlay();

	_skeletalMeshComponent->SetSimulatePhysics(false);
    _skeletalMeshComponent->SetEnableGravity(false);

	if (_animationSequence) {
		_skeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		_skeletalMeshComponent->SetAnimation(_animationSequence);
	}
}

void ASimpleAnimatedItem::UseInteraction()
{
	if (!_bCanReverse) {
		_skeletalMeshComponent->SetPosition(0.f);
		_skeletalMeshComponent->Play(false);
	} else {
		if (!_bReadyState) {
			_bReadyState = true;
			_skeletalMeshComponent->SetPosition(_skeletalMeshComponent->GetPosition());
			_skeletalMeshComponent->SetPlayRate(1.f);
			_skeletalMeshComponent->Play(false);
		} else {
			_bReadyState = false;
			_skeletalMeshComponent->SetPosition(_skeletalMeshComponent->GetPosition());
			_skeletalMeshComponent->SetPlayRate(-1.f);
			_skeletalMeshComponent->Play(false);
		}
	}

	if (_soundComponent != nullptr)
		_soundComponent->Play();
}

void ASimpleAnimatedItem::OnTriggerStart()
{
	UseInteraction();
}