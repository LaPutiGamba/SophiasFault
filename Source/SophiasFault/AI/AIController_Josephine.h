#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Josephine.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AAIController_Josephine : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Josephine();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* inPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> _behaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBehaviorTreeComponent> _behaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UBlackboardComponent> _blackboardComponent;
};
