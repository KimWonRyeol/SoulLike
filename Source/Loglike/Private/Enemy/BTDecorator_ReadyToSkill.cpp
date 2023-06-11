// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTDecorator_ReadyToSkill.h"
#include "Enemy/BossAIController.h"
#include "Enemy/Boss.h"
#include "Enemy/MonsterBase.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_ReadyToSkill::UBTDecorator_ReadyToSkill()
{
	NodeName = TEXT("Ready To Play Skill");
	MaxHP = 500.f;
}

bool UBTDecorator_ReadyToSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	float HPComp = 500.f;

	float CurrentHP = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ABossAIController::CurrentHPKey);
	if (CurrentHP == NULL)
	{
		return false;
	}

	float calSkillHP = MaxHP * HPPercentage;

	if (CurrentHP < HPComp)
	{
		if (CurrentHP < HPComp - calSkillHP)
		{
			bResult = (CurrentHP < HPComp - calSkillHP);
			UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f | HPComp - calSkillHP: %f - %f = %f"), CurrentHP, HPComp, calSkillHP, HPComp - calSkillHP);
			HPComp = CurrentHP;
			return bResult;
		}
	}
	return false;
}

