// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RangedRotation.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTTask_RangedRotation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RangedRotation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	FRotator TargetRot;
	FVector LookVector;
};
