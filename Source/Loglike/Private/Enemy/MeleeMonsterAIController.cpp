// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
AMeleeMonsterAIController::AMeleeMonsterAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/UndeadPack/AI/Melee/BB_Melee.BB_Melee'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/UndeadPack/AI/Melee/BT_Melee.BT_Melee'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}