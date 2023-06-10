// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_BossRanged.h"
#include "Enemy/Boss.h"
#include "Enemy/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossRanged::UBTTask_BossRanged()
{
	bNotifyTick = true;
	IsShooting = false;
}

void UBTTask_BossRanged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsShooting)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_BossRanged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	// bool PlaySkill = OwnerComp.GetBlackboardComponent()->GetValueAsBool(ABossAIController::PlaySkillKey);

	Boss->RangedAttack();
	IsShooting = true;

	Boss->RangedAttackIsEnded.AddLambda([this]() -> void {
		IsShooting = false;
	});

	auto BossAI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	int32 AtkNum = BossAI->AttackNum;
	BossAI->AttackNum = AtkNum++;

	Boss->ReadyToSkill(false);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::RangedAtkKey, false);
	return EBTNodeResult::InProgress;

}