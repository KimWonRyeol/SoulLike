// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterAIControllerBase.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API ABossAIController : public AMonsterAIControllerBase
{
	GENERATED_BODY()
	
public:
	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;

	void PlayedStartEvent(bool enable);
	void SetSkill(bool enable);
	void SetEntranceKey(bool enable);
	void BossHitPoint(float HP);
	void CountAttack();

public:
	static const FName CheckEntranceKey;
	static const FName CurrentHPKey;
	static const FName ChargedAtkKey;
	static const FName RangedAtkKey;
	static const FName JumpAtkKey;

	int AttackNum;
};
