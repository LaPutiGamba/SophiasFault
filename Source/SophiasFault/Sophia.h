#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/SpotLightComponent.h" 
#include "Sophia.generated.h"

#define printText(_text) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT(_text)))
#define printFloat(_float) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%f"), _float))
#define printBool(_bool) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%s"), _bool ? TEXT("TRUE") : TEXT("FALSE")))

enum STAMINA_STATUS { RUNNING, IDLE, EXHAUSTED };
enum FLASHLIGHT_STATUS { LIGHTON, LIGHTOFF, NEEDRECHARGE };

UCLASS()
class SOPHIASFAULT_API ASophia : public ACharacter
{
	GENERATED_BODY()

/************* VARIABLES *************/
protected:
	/* ----- Game State to use variables ----- */
	class AGMS_MyGameStateBase* _myGameState;
	/* --------------------------------------- */

	/* ----- Camera velocity ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "1"))
	float _cameraVelocity;
	/* --------------------------- */

	/* ----- Speed ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "2"))
	float _speed;
	/* ----------------- */
	
	/* ----- Run or Crouch and Stamina ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _runOrCrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "10"))
	float _staminaMax;

	bool _runningOrCrouching;
	float _staminaTimer;
	STAMINA_STATUS _staminaStatus;
	/* ------------------------------------- */



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables")
	bool _onChase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* _defaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _lookAction;


	/* ----- Flashlight and it's reload system ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _flashlightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _rechargeFlashlightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement/View variables", meta = (ClampMin = "0.01", ClampMax = "10"))
	float _flashlightMax;

	class USpotLightComponent* _flashlight;
	bool _flashlightOn;
	bool _rechargingFlashlight;
	float _flashlightTimer;
	FLASHLIGHT_STATUS _flashlightStatus;
	/* --------------------------------------------- */

	/* ----- Inventory ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* _inventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	class UInventoryComponent* _inventory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<class UInventoryWidget> _inventoryHUDClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UInventoryWidget* _inventoryHUD;
	/* --------------------- */

/************* FUNCTIONS *************/
public:
	// Sets default values for this character's properties
	ASophia();

	// Called every frame
	virtual void Tick(float deltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void RunOrCrouch(const FInputActionValue& value);

	void Flashlight(const FInputActionValue& value);

	void RechargeFlashlight(const FInputActionValue& value);

	void Inventory(const FInputActionValue& value);

	void UseItem(class UItem* item);
};
