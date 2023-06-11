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
	//캐릭터의 애니메이션을 계속 업데이트하는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//공격 몽타주 섹션을 이동하는 함수
	void JumpToAttackMontageSection(int32 NewSection);

	//공격 몽타주 실행 함수
	void PlayAttackMontage();
	//닷지 몽타주 실행 함수
	void PlayDodgeMontage();
	//패링 몽타주 실행 함수
	void PlayParryingMontage(float);
	//공격 받는 몽타주 실행 함수
	void PlayHitMontage();
	//죽었을 때 애니메이션 실행 함수
	void SetDeadAnim() { IsDead = true; }
	

public:
	//몽타주 노티파이 체크 델리게이트
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;


private:
	//현재 폰 스피드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	//공중에 있는지 체크
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
	//현재 걷는 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentWalkSpeed;
	//공격 몽타주 클래스 객체
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	//닷지 몽타주 클래스 객체
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage;
	//패링 몽타주 클래스 객체
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Parrying", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingMontage;
	//공격 받는 몽타주 클래스 객체
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hit", Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;
	
	/*UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dead", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;*/

	//죽었다는 것을 체크
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;


private:

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

};
