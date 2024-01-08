#include "AIController_Josephine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIController_Josephine::AAIController_Josephine()
{
	_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void AAIController_Josephine::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(_behaviorTree.Get())) {
		RunBehaviorTree(_behaviorTree.Get());
		_behaviorTreeComponent->StartTree(*_behaviorTree.Get());
	}
}

void AAIController_Josephine::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	if (IsValid(Blackboard.Get()) && IsValid(_behaviorTree.Get())) {
		Blackboard->InitializeBlackboard(*_behaviorTree.Get()->BlackboardAsset.Get());
	}
}