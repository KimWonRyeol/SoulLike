// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAnimBase.h"
#include "MeleeAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UMeleeAnimInstance : public UMonsterAnimBase
{
	GENERATED_BODY()
public:
	UMeleeAnimInstance();
	void PlayComboAttackMontage(int32 ComboNum);
	UFUNCTION()
	void AnimNotify_AttackStart();
	UFUNCTION()
	void AnimNotify_AttackEnd();
};
