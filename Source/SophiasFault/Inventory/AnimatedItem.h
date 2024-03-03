#pragma once

#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"
#include "AnimatedItem.generated.h"

UCLASS()
class SOPHIASFAULT_API AAnimatedItem : public AActor
{
	GENERATED_BODY()

public:
	AAnimatedItem();

protected:
	virtual void BeginPlay() override;

public:
	// ITEM MESH
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* _skeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _itemComponent;

	UPROPERTY(EditAnywhere)
	FString _name;

	// ANIMATION
	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimSequence* _animationSequence;

	// AUDIO
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _metaSound;

	class UAudioComponent* _soundComponent;

	// GENERAL VARIABLES
	UPROPERTY(EditAnywhere, Category = "General Variables")
	FVector _pickUpLocation;
	UPROPERTY(EditAnywhere, Category = "General Variables")
	FRotator _pickUpRotation;
	UPROPERTY(EditAnywhere, Category = "General Variables")
	float _inspectDistance;

	class UInventoryComponent* _owningInventory;

	class AGMS_MyGameStateBase* _myGameState;
	class ACharacter* _myCharacter;
	class UCameraComponent* _playerCamera;

	bool _bNoSwitchableItem;
};