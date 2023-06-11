// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossAIController.h"
#include "Enemy/Boss.h"
#include "Enemy/BossAnimInstance.h"
#include "Enemy/Projectile.h"


#include "Character/LoglikeCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABossAIController::CheckEntranceKey(TEXT("CheckEntrance"));
const FName ABossAIController::ChargedAtkKey(TEXT("ChargedAtk"));
const FName ABossAIController::RangedAtkKey(TEXT("RangedAtk"));
const FName ABossAIController::JumpAtkKey(TEXT("JumpAtk"));
const FName ABossAIController::CurrentHPKey(TEXT("CurrentHP"));

ABossAIController::ABossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/ParagonRampage/AI/BB_Boss.BB_Boss'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/ParagonRampage/AI/BT_Boss.BT_Boss'"));
	if (BTObject.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss BT Is Succeded!!"));
		BTAsset = BTObject.Object;
	}
	else UE_LOG(LogTemp, Warning, TEXT("Boss BT Is Fail"));
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("BOSS AI OnPossess IS RUN"));
	Super::OnPossess(InPawn);
	ABoss* Boss = Cast<ABoss>(InPawn);
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	ALoglikeCharacter* Player = Cast<ALoglikeCharacter>(this);

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		Blackboard->SetValueAsBool(IsDeadKey, false);
		Blackboard->SetValueAsBool(IsStunKey, false);
		Blackboard->SetValueAsFloat(AttackTimeKey, Boss->GetAttackSpeed());
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		
		Blackboard->SetValueAsBool(CheckEntranceKey, false);
		Blackboard->SetValueAsBool(ChargedAtkKey, true);
		Blackboard->SetValueAsBool(RangedAtkKey, true);
		Blackboard->SetValueAsBool(JumpAtkKey, true);


		Blackboard->SetValueAsFloat(CurrentHPKey, Boss->GetHitPoint());
	
		if (Player && Player->GetController()->IsPlayerController())
		{
			Blackboard->SetValueAsObject(TargetKey, Player);
		}

		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s BehaviorTree is not Run"), *(GetOwner()->GetName()));
		}
	}
	AttackNum = 0;

}

void ABossAIController::PlayedStartEvent(bool enable)
{
	Blackboard->SetValueAsBool(CheckEntranceKey, enable);
}

void ABossAIController::SetSkill(bool enable)
{
	//Blackboard->SetValueAsBool(PlaySkillKey, enable);
	if (enable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SETSKILL IS TRUE"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SETSKILL IS FALSE"));
	}

}

void ABossAIController::BossHitPoint(float HP)
{
	Blackboard->SetValueAsFloat(CurrentHPKey, HP);
}

void ABossAIController::SetEntranceKey(bool enable)
{
	Blackboard->SetValueAsBool(CheckEntranceKey, enable);
}

void ABossAIController::CountAttack()
{
	AttackNum++;
	UE_LOG(LogTemp, Warning, TEXT("AttackNum: %d"), AttackNum);

	if ((AttackNum % 5) == 0)
	{
		Blackboard->SetValueAsBool(JumpAtkKey, true);
	}

	if ((AttackNum % 4) == 0)
	{
		Blackboard->SetValueAsBool(ChargedAtkKey, true);
	}
	else if ((AttackNum % 7) == 0)
	{
		Blackboard->SetValueAsBool(RangedAtkKey, true);
	}
}