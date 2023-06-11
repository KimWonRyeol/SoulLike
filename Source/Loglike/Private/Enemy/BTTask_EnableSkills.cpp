// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_EnableSkills.h"
#include "Enemy/Boss.h"
#include "Enemy/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EnableSkills::UBTTask_EnableSkills()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnableSkills::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss)
		return EBTNodeResult::Failed;

	Boss->ReadyToSkill(true);
	return EBTNodeResult::Succeeded;
}