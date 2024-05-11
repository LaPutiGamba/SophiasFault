#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "InputActionValue.h"
#include "Sophia.generated.h"

enum STAMINA_STATUS { ST_RUNNING, ST_IDLE, ST_EXHAUSTED };

UCLASS()
class SOPHIASFAULT_API ASophia : public ACharacter
{
	GENERATED_BODY()

	/************* VARIABLES *************/
protected:
	/* ----- Global variables ----- */
	APlayerController* _playerController;

	int _sanity;
	/* ---------------------------- */


	/* ----- Mapping Contexts ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* _mainMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* _inspectMappingContext;
	/* ---------------------------- */


	/* ----- Game State to use variables ----- */
	class AGMS_MyGameStateBase* _myGameState;
	/* --------------------------------------- */


	/* ------ Camera ------ */
	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* _springArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* _cameraComponent;

	FVector _cameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "1"))
	float _cameraVelocity;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> _shakeIdle;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> _shakeWalk;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<class UCameraShakeBase> _shakeRun;
	/* -------------------- */


	/* ----- Speed ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "2"))
	float _speed;
	/* ----------------- */


	/* ----- Run and Crouch and Stamina ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _runAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _crouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "10"))
	float _staminaMax;

	bool _bIsRunning;
	bool _bIsCrouching;
	bool _bCheckCrouchingUntilNot;
	float _staminaTimer;
	STAMINA_STATUS _staminaStatus;
	/* ------------------------------------- */

	/* ----- Crouching timeline ----- */
	class UTimelineComponent* _timelineComponent;
	FOnTimelineFloat _timelineCallback;

	class UCurveFloat* _curveFloat;

	float _timelineValue;
	float _curveFloatValue;
	/* ------------------------------ */

	/* ----- Input Actions ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _pickUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _dropItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _inspectAction;
	/* ------------------------- */


	/* ----- Inventory ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _inventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeItemMouseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem0Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem4Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem5Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem6Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem7Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem8Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _changeHandItem9Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UInventoryComponent* _inventory;

	TSubclassOf<class UInventoryWidget> _inventoryHUDClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<class UUserWidget> _inventoryItemsClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	class UInventoryWidget* _inventoryHUD;

	bool _bInventoryOpen;
	/* --------------------- */


	/* ----- Items ----- */
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UStaticMeshComponent* _holdingComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UStaticMeshComponent* _flashlightCrankComponent;
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UStaticMeshComponent* _flashlightCrankHandleComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpotLightComponent* _flashlightComponent;

	bool _bCanMove;
	/* ----------------- */


	/* -------- Audio -------- */
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _footstepsSounds;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class USoundBase* _breathSounds;

	class UAudioComponent* _footstepsSoundComponent;
	class UAudioComponent* _breathSoundComponent;

	bool _bBreathPlayed;
	/* ----------------------- */

/************* FUNCTIONS *************/
public:
	// Sets default values for this character's properties
	ASophia();

	// Called every frame
	virtual void Tick(float deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	// Get the camera component
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCameraComponent() { return _cameraComponent; }

	// Get the inventory component
	class UInventoryComponent* GetInventory() { return _inventory; }

	// Get the item widget class
	TSubclassOf<class UUserWidget> GetItemWidgetClass() { return _inventoryItemsClass; }

	// Get the current hand item
	class AItem* GetCurrentHandItem();

	// Get the holding component
	class UStaticMeshComponent* GetHoldingComponent() { return _holdingComponent; }

	// Get the flashlight crank component
	class UStaticMeshComponent* GetFlashlightCrankComponent() { return _flashlightCrankComponent; }

	// Get the flashlight crank handle component
	class UStaticMeshComponent* GetFlashlightCrankHandleComponent() { return _flashlightCrankHandleComponent; }

	// Get the flashlight component
	class USpotLightComponent* GetFlashlightComponent() { return _flashlightComponent; }

	// Set the speed
	void SetSpeed(float speed) { _speed = speed; }

	void ToggleMovement(bool& bInspecting);

	UInputMappingContext* GetMainMappingContext() { return _mainMappingContext; }
	UInputMappingContext* GetInspectMappingContext() { return _inspectMappingContext; }

	class UAudioComponent* GetFootstepsSoundComponent() { return _footstepsSoundComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void Running(const FInputActionValue& value);
	void Crouching(const FInputActionValue& value);

	void Inventory(const FInputActionValue& value);

	// ITEMS
	void OnAction(const FInputActionValue& value);

	void DropItem(const FInputActionValue& value);

	// TIMELINE
	UFUNCTION()
	virtual void CrouchingTimeline(float value);
};