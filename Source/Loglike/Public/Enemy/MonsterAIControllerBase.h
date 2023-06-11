// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API AMonsterAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIControllerBase();
	virtual void OnPossess(APawn* InPawn) override;
	void SetStun(bool enable, float sec);
	void SetPain(bool enable, float sec);
	void SetDead();
	static const FName HomePosKey;
	static const FName TargetKey;
	static const FName IsStunKey;
	static const FName IsDeadKey;
	static const FName IsPainKey;
	static const FName PainTimeKey;
	static const FName StunTimeKey;
	static const FName AttackTimeKey;

protected:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;

};
