#include "Ladder.h"
#include "Camera/CameraActor.h"
#include "Curves/CurveVector.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "../../Core/GMS_MyGameStateBase.h"
#include "EnhancedInputSubsystems.h" 
#include "Blueprint/UserWidget.h" 
#include "../../Sophia.h"
#include "../../Macros.h"

ALadder::ALadder()
{
	_curveVectorLocation = CreateDefaultSubobject<UCurveVector>(TEXT("CurveVectorLocation"));
	_curveVectorRotation = CreateDefaultSubobject<UCurveVector>(TEXT("CurveVectorRotation"));

	_bIsUp = true;
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();

	_playerController = GetWorld()->GetFirstPlayerController();
	_sophia = Cast<ASophia>(_playerController->GetPawn());
	
	if (_curveVectorLocation && _curveVectorRotation) {
		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(1.f, 447.f + 7.5f);

		// Add keys
		_curveVectorLocation->FloatCurves[0].UpdateOrAddKey(1.7f, -366.3f);
		_curveVectorLocation->FloatCurves[1].UpdateOrAddKey(1.7f, 2460.5f);
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(1.7f, 447.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(1.7f, 0.f);
		_curveVectorRotation->FloatCurves[1].UpdateOrAddKey(1.7f, -50.f);
		_curveVectorRotation->FloatCurves[2].UpdateOrAddKey(1.7f, 180.f);

		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(3.2f, 402.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(3.2f, 0.f);

		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(4.2f, 345.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(4.2f, -12.f);

		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(5.2f, 288.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(5.2f, 12.f);

		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(6.2f, 231.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(6.2f, -12.f);

		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(7.2f, 174.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(7.2f, 12.f);

		// Add keys
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(8.2f, 117.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(8.2f, -12.f);

		// Add keys
		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(9.2f, 12.f);

		// Add keys
		_curveVectorLocation->FloatCurves[0].UpdateOrAddKey(9.7f, -295.f);
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(9.7f, 90.f);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(9.7f, 0.f);
		_curveVectorRotation->FloatCurves[1].UpdateOrAddKey(9.7f, -15.f);


		// Timeline
		_timelineCallback.BindUFunction(this, FName("ControlLadderAnim"));
		_timelineFinishedCallback.BindUFunction(this, FName("FinishAnim"));

		_timelineComponent->AddInterpVector(_curveVectorLocation, _timelineCallback);
		_timelineComponent->AddInterpVector(_curveVectorRotation, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}
}

void ALadder::SetInitialKeys(float time)
{
	// Add initial keys
	_curveVectorLocation->FloatCurves[0].UpdateOrAddKey(time, _sophia->GetActorLocation().X);
	_curveVectorLocation->FloatCurves[1].UpdateOrAddKey(time, _sophia->GetActorLocation().Y);
	_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(time, _sophia->GetActorLocation().Z);

	_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(time, _sophia->GetActorRotation().Roll);
	_curveVectorRotation->FloatCurves[1].UpdateOrAddKey(time, _sophia->GetActorRotation().Pitch);

	// Activate the cubic interpolation mode
	for (int i = 0; i < 3; i++) {
		FRichCurve& curveComponentLocation = _curveVectorLocation->FloatCurves[i];
		FRichCurve& curveComponentRotation = _curveVectorRotation->FloatCurves[i];

		for (auto& key : curveComponentLocation.Keys) {
			key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
		}
		for (auto& key : curveComponentRotation.Keys) {
			key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
		}
	}
}

void ALadder::ControlLadderAnim(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveVectorLocationValue = _curveVectorLocation->GetVectorValue(_timelineValue);
	_curveVectorRotationValue = _curveVectorRotation->GetVectorValue(_timelineValue);

	_sophia->SetActorLocation(_curveVectorLocationValue);
	_playerController->SetControlRotation(FRotator(_curveVectorRotationValue.Y, _curveVectorRotationValue.Z, _curveVectorRotationValue.X));
}

void ALadder::FinishAnim()
{
	_myGameState->_hudWidget->SetVisibility(ESlateVisibility::Visible);
	SetGameState(true);
}

void ALadder::SetGameState(bool bState)
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
		if (bState) 
			subsystem->AddMappingContext(_sophia->GetMainMappingContext(), 0);
		else 
			subsystem->RemoveMappingContext(_sophia->GetMainMappingContext());
	}

	_sophia->GetCapsuleComponent()->SetCollisionEnabled(bState ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	_sophia->GetCapsuleComponent()->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	_myGameState->_hudWidget->SetVisibility(bState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ALadder::UseInteraction(AItem* item)
{
	SetGameState(false);

	if (_bIsUp && !_timelineComponent->IsPlaying()) {
		SetInitialKeys(0.f);
		_bIsUp = false;

		_curveVectorRotation->FloatCurves[2].UpdateOrAddKey(0.f, _sophia->GetActorRotation().Yaw);
		_timelineComponent->PlayFromStart();

		FTimerHandle ladderHandle;
		GetWorld()->GetTimerManager().SetTimer(ladderHandle, [this]() {
			if (_metaSound) {
				_soundComponent->SetSound(_metaSound);
				_soundComponent->Play();
			}
		}, 1.7f, false);
	} else if (!_bIsUp && !_timelineComponent->IsPlaying()) {
		SetInitialKeys(10.7f);
		_bIsUp = true;

		_curveVectorRotation->FloatCurves[2].UpdateOrAddKey(0.f, 180.f);
		_timelineComponent->Reverse();

		FTimerHandle ladderHandle;
		GetWorld()->GetTimerManager().SetTimer(ladderHandle, [this]() {
			if (_interactSound) {
				_soundComponent->SetSound(_interactSound);
				_soundComponent->Play();
			}
		}, 1.f, false);
	}
}