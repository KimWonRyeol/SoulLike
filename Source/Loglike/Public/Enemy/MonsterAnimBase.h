// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimBase.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UMonsterAnimBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterAnimBase();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DaltaSeconds) override;

	void PlayAttackMontage();
	void PlayPainMontage();
	void PlayStunMontage();

	void SetDeadAnim() { IsDead = true; }

public:
	void SetDead() { IsDead = true; }

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess = true))
	bool IsDead;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
		UAnimMontage* PainMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
		UAnimMontage* StunMontage;

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess = true))
		float Speed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess = true))
		ACharacter* Character;

private:
	UFUNCTION()
	void AnimNotify_Attack();
};
