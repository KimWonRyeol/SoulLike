// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_BossAttack.h"
#include "Enemy/BossAIController.h"
#include "Enemy/Boss.h"

UBTTask_BossAttack::UBTTask_BossAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (Boss == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (Boss->GetIsAttacking())
	{
		return EBTNodeResult::Succeeded;
	}

	Boss->Attack();
	// Boss->AttackCount();

	return EBTNodeResult::Succeeded;
}