// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/RangedMonsterBase.h"
#include "Enemy/MonsterAnimBase.h"

ARangedMonsterBase::ARangedMonsterBase()
{
	IsShooting = false;
}

void ARangedMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterAnim->OnMontageEnded.AddDynamic(this, &ARangedMonsterBase::OnRangedMontageEnded);
}

void ARangedMonsterBase::Attack()
{
	if (nullptr == MonsterAnim) return;
	if (IsShooting) return;
	
	// ������ �׳� ��ȯ eg(if(stun ��Ÿ�ְ� �ߵ����� �ƴ϶��))
	// UE_LOG(LogTemp, Warning, TEXT("ARangedMonsterBase Attack Func Is Run"));
	MonsterAnim->PlayAttackMontage();
	
	IsShooting = true;
}

void ARangedMonsterBase::OnRangedMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Warning, TEXT("Now Playing %s"), Montage->GetName());

	if (Montage->GetName().Contains("Pain"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Attack Mon"));
		return;
	}
	IsShooting = false;
	UE_LOG(LogTemp, Warning, TEXT("OnRangedMontageEnded"));

	OnShootingEnd.Broadcast();
}