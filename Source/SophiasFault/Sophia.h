#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	/* ---------------------------- */


	/* ----- Mapping Contexts ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* _mainMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* _pianoMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* _earthMappingContext;
	/* ---------------------------- */


	/* ----- Game State to use variables ----- */
	class AGMS_MyGameStateBase* _myGameState;
	/* --------------------------------------- */


	/* ----- Materials ----- */
	/* --------------------- */


	/* ------ Camera ------ */
	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* _springArmComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* _cameraComponent;

	FVector _cameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "1"))
	float _cameraVelocity;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class AActor* _pianoCameraActor;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class AActor* _earthCameraActor;
	/* -------------------- */


	/* ----- Speed ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "2"))
	float _speed;
	/* ----------------- */


	/* ----- Run or Crouch and Stamina ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _runOrCrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "10"))
	float _staminaMax;

	bool _bRunningOrCrouching;
	float _staminaTimer;
	STAMINA_STATUS _staminaStatus;
	/* ------------------------------------- */


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables")
	bool _bOnChase;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _clickInteractiveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _getUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _clickRotationAction;
	/* ------------------------- */


	/* ----- Flashlight and it's reload system ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _flashlightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _rechargeFlashlightAction;
	/* --------------------------------------------- */


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
	AActor* _currentChangeCameraItem;

	class USceneComponent* _holdingComponent;
	class USceneComponent* _attachComponent;

	bool _bCanMove;
	bool _bInspectingPressed;

	float _pitchMax;
	float _pitchMin;

	FRotator _lastRotation;
	/* ----------------- */

/************* FUNCTIONS *************/
public:
	// Sets default values for this character's properties
	ASophia();

	// Called every frame
	virtual void Tick(float deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	// Get the camera component
	UCameraComponent* GetCameraComponent() { return _cameraComponent; }

	// Get the inventory component
	class UInventoryComponent* GetInventory() { return _inventory; }

	// Get the item widget class
	TSubclassOf<class UUserWidget> GetItemWidgetClass() { return _inventoryItemsClass; }

	// Get the current hand item
	class AItem* GetCurrentHandItem();

	class USceneComponent* GetHoldingComponent() { return _holdingComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void RunOrCrouch(const FInputActionValue& value);

	void Inventory(const FInputActionValue& value);

	// EARTH PUZZLE
	void ClickInteractive(const FInputActionValue& value);

	void EarthRotation(const FInputActionValue& value);

	// ITEMS
	void OnAction(const FInputActionValue& value);

	void DropItem(const FInputActionValue& value);

	void OnInspect(const FInputActionValue& value);

	void BlendBackWithCamera(const FInputActionValue& value);

	void ToggleMovement();
};