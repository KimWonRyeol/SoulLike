// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterAnimBase.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UBossAnimInstance : public UMonsterAnimBase
{
	GENERATED_BODY()

public:
	UBossAnimInstance();

	void PlayComboAttackMontage(int32 ComboNum);

	void PlayChargedAttackMontage();
	void PlayJumpAttackMontage();
	void PlayRangedAttackMontage();

	void PlayLevelStartMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ChargedAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* RangedAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* LevelStartMontage;

private:
	UFUNCTION()
	void AnimNotify_AttackEnd();
private:
	// ���� �������� ��Ƽ���� �ٸ��� ����
	UFUNCTION()
	void AnimNotify_ChragedAtk();
	UFUNCTION()
		void AnimNotify_JumpHit();

	UFUNCTION()
	void AnimNotify_JumpAtk();

	UFUNCTION()
	void AnimNotify_RangedAtk();


};
