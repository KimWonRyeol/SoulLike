// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTTask_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_BossAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	class ABoss* Boss;
};
