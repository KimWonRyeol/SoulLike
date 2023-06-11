// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterBase.h"
#include "RangedMonsterBase.generated.h"

/**
 * 
 */

 // 원거리 몬스터 공격 애니메이션이 끝나면 공격 재수행하도록 델리게이트 호출
DECLARE_MULTICAST_DELEGATE(FOnShootingEndDelegate);

UCLASS()
class LOGLIKE_API ARangedMonsterBase : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	ARangedMonsterBase();

	// Actor Set/Initialize
	// virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	virtual void Attack() override;
	// Montage Anim => 몬스터 공격 종료 시점 알림용
	UFUNCTION()
	void OnRangedMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Delegate
	FOnShootingEndDelegate OnShootingEnd;

	// IsShooting이 true일 동안은 공격 모션 재생 x
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsShooting;

//protected:
	//class URangedAnimInstance* RangedAnim;

};
