// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_RangedAttack.h"
#include "Enemy/RangedMonsterAIController.h"
#include "Enemy/RangedMonsterBase.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	NodeName = TEXT("Ranged Attack");
	bNotifyTick = true;
	IsShooting = false;
}

void UBTTask_RangedAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsShooting)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto RangedMonster = Cast<ARangedMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == RangedMonster)
		return EBTNodeResult::Failed;

	RangedMonster->Attack();
	IsShooting = true;

	RangedMonster->OnShootingEnd.AddLambda([this]() -> void {
		// UE_LOG(LogTemp, Warning, TEXT("BTTask_RangedAttack Is Recognized"));
		IsShooting = false;
	});

	return EBTNodeResult::InProgress;
}


