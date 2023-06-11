// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/MonsterBase.h"
#include "RangedMonsterBase.generated.h"

/**
 * 
 */

 // ���Ÿ� ���� ���� �ִϸ��̼��� ������ ���� ������ϵ��� ��������Ʈ ȣ��
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
	// Montage Anim => ���� ���� ���� ���� �˸���
	UFUNCTION()
	void OnRangedMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// Delegate
	FOnShootingEndDelegate OnShootingEnd;

	// IsShooting�� true�� ������ ���� ��� ��� x
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsShooting;

//protected:
	//class URangedAnimInstance* RangedAnim;

};
