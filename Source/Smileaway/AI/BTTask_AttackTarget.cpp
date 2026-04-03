// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackTarget.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Smileaway/Character/EnemyBase.h"

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) return EBTNodeResult::Failed;

	auto* ControlledCharacter = Cast<AEnemyBase>(AIController->GetPawn());
	if (ControlledCharacter == nullptr) return EBTNodeResult::Failed;

	auto* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (TargetActor == nullptr) return EBTNodeResult::Failed;

	ControlledCharacter->Attack(TargetActor);

	return EBTNodeResult::Succeeded;
}
