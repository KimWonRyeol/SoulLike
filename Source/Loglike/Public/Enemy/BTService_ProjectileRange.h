// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ProjectileRange.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTService_ProjectileRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_ProjectileRange();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRadius;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
