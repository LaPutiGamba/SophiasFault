#pragma once

#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"
#include "InventoryComponent.h"
#include "Item.generated.h"

/**
 *
 */
UCLASS()
class SOPHIASFAULT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:
	// ITEM MESH
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _meshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _itemComponent;

	UPROPERTY(EditAnywhere)
	FString _name;

	// TIMELINE
	class UTimelineComponent* _timelineComponent;

	FOnTimelineEvent _timelineFinishedCallback;

	// AUDIO
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _metaSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _interactSound;

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
	float _noSwitchableItemSpeed;
};