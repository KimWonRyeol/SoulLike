// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTDecorator_IsInAttackRange.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "Enemy/MeleeMonsterBase.h"
#include "Enemy/MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	if (Cast<AMeleeMonsterBase>(ControllingPawn))
	{
		if (Cast<AMeleeMonsterBase>(ControllingPawn)->GetIsAttacking())
			return true;
	}


	//거리계산
	auto Target = Cast<ALoglikeCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIControllerBase::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= Range);
	return bResult;
}
