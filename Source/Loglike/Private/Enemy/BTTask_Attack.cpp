// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_Attack.h"
#include "Enemy/MeleeMonsterAIController.h"
#include "Enemy/MeleeMonsterBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Monster = Cast<AMeleeMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
		return EBTNodeResult::Failed;
	if(Monster->GetIsAttacking())
		return EBTNodeResult::Succeeded;
	//공격 시작
	Monster->ComboAttack();
	return EBTNodeResult::Succeeded;
}



