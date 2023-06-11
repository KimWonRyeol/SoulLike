// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossRanged.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTTask_BossRanged : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_BossRanged();
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	class ABoss* Boss;
	bool IsShooting = false;
};
