// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ARangedMonsterAIController::ARangedMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/UndeadPack/AI/Ranged/BB_Ranged.BB_Ranged'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/UndeadPack/AI/Ranged/BT_Ranged.BT_Ranged'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}