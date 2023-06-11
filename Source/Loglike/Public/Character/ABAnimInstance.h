// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOGLIKE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UABAnimInstance();
	//ĳ������ �ִϸ��̼��� ��� ������Ʈ�ϴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//���� ��Ÿ�� ������ �̵��ϴ� �Լ�
	void JumpToAttackMontageSection(int32 NewSection);

	//���� ��Ÿ�� ���� �Լ�
	void PlayAttackMontage();
	//���� ��Ÿ�� ���� �Լ�
	void PlayDodgeMontage();
	//�и� ��Ÿ�� ���� �Լ�
	void PlayParryingMontage(float);
	//���� �޴� ��Ÿ�� ���� �Լ�
	void PlayHitMontage();
	//�׾��� �� �ִϸ��̼� ���� �Լ�
	void SetDeadAnim() { IsDead = true; }
	

public:
	//��Ÿ�� ��Ƽ���� üũ ��������Ʈ
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;


private:
	//���� �� ���ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	//���߿� �ִ��� üũ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
	//���� �ȴ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentWalkSpeed;
	//���� ��Ÿ�� Ŭ���� ��ü
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	//���� ��Ÿ�� Ŭ���� ��ü
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage;
	//�и� ��Ÿ�� Ŭ���� ��ü
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Parrying", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingMontage;
	//���� �޴� ��Ÿ�� Ŭ���� ��ü
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hit", Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;
	
	/*UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;*/

	//�׾��ٴ� ���� üũ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;


private:

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

};
