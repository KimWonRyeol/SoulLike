// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnableSkills.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTTask_EnableSkills : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EnableSkills();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	class ABoss* Boss;
};
