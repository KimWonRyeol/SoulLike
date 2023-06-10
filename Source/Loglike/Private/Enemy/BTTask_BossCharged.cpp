// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_BossCharged.h"
#include "Enemy/Boss.h"
#include "Enemy/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_BossCharged::UBTTask_BossCharged()
{
	bNotifyTick = true;
	IsCharging = false;
}

void UBTTask_BossCharged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsCharging)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_BossCharged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	// bool PlaySkill = OwnerComp.GetBlackboardComponent()->GetValueAsBool(ABossAIController::PlaySkillKey);


	Boss->ChargedAttack();
	IsCharging = true;

	Boss->ChargedAttackIsEnded.AddLambda([this]() -> void {
		IsCharging = false;
	});

	auto BossAI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	int32 AtkNum = BossAI->AttackNum;
	BossAI->AttackNum = AtkNum++;

	Boss->ReadyToSkill(false);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::ChargedAtkKey, false);
	return EBTNodeResult::InProgress;

}