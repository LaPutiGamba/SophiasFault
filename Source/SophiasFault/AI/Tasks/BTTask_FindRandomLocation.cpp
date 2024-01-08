#include "BTTask_FindRandomLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	_searchRadius = 500.f;
	NodeName = TEXT("Find Random Location");

	// Accept only vectors
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	FNavLocation location;

	// Get AI Pawn
	AAIController* AIController = ownerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();

	// Get Pawn Origin
	const FVector origin = AIPawn->GetActorLocation(); 

	// Obtain Navigation System and find a random location
	const UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (IsValid(navSystem) && navSystem->GetRandomPointInNavigableRadius(origin, _searchRadius, location)) {
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, location.Location);
	}

	// Signal the BehaviorTreeComponent that the fask finished with a success
	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
