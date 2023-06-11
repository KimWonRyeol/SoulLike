// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_BossJump.h"
#include "Enemy/Boss.h"
#include "Enemy/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_BossJump::UBTTask_BossJump()
{
	bNotifyTick = true;
	IsJumping = false;
}

void UBTTask_BossJump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsJumping)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
EBTNodeResult::Type UBTTask_BossJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	// bool PlaySkill = OwnerComp.GetBlackboardComponent()->GetValueAsBool(ABossAIController::PlaySkillKey);

	Boss->JumpAttack();
	IsJumping = true;

	Boss->JumpAttackIsEnded.AddLambda([this]() -> void {
		IsJumping = false;
	});

	auto BossAI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	int32 AtkNum = BossAI->AttackNum;
	BossAI->AttackNum = AtkNum++;

	Boss->ReadyToSkill(false);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ABossAIController::JumpAtkKey, false);
	return EBTNodeResult::InProgress;

}