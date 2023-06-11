// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MonsterAIControllerBase.h"
#include "Enemy/MonsterBase.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

const FName AMonsterAIControllerBase::HomePosKey(TEXT("HomePos"));
const FName AMonsterAIControllerBase::TargetKey(TEXT("Target"));
const FName AMonsterAIControllerBase::IsStunKey(TEXT("IsStun"));
const FName AMonsterAIControllerBase::IsDeadKey(TEXT("IsDead"));
const FName AMonsterAIControllerBase::IsPainKey(TEXT("IsPain"));
const FName AMonsterAIControllerBase::StunTimeKey(TEXT("StunTime"));
const FName AMonsterAIControllerBase::PainTimeKey(TEXT("PainTime"));
const FName AMonsterAIControllerBase::AttackTimeKey(TEXT("AttackTime"));

AMonsterAIControllerBase::AMonsterAIControllerBase()
{
	
}

void AMonsterAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AMonsterBase* Monster = Cast<AMonsterBase>(InPawn);
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		Blackboard->SetValueAsBool(IsDeadKey, false);
		Blackboard->SetValueAsFloat(AttackTimeKey, Monster->GetAttackSpeed());
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s BehaviorTree is not Run"),*(GetOwner()->GetName()));
		}
	}
}

void AMonsterAIControllerBase::SetPain(bool enable, float sec)
{
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsBool(IsPainKey, enable);
		Blackboard->SetValueAsFloat(PainTimeKey, sec);
	}

}
void AMonsterAIControllerBase::SetStun(bool enable, float sec)
{
	/*if (enable)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartStun"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EndStun"));
	}*/
	if (Blackboard != nullptr)
	{
		Blackboard->SetValueAsBool(IsStunKey, enable); 
		Blackboard->SetValueAsFloat(StunTimeKey, sec);
	}
}

void AMonsterAIControllerBase::SetDead()
{
	if (Blackboard != nullptr)
		Blackboard->SetValueAsBool(IsDeadKey, true);
}
