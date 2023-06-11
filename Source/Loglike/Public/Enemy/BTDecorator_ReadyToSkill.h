// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ReadyToSkill.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBTDecorator_ReadyToSkill : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ReadyToSkill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPPercentage;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	float MaxHP;
};
