// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BossDetectRange.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTService_BossDetectRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_BossDetectRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
