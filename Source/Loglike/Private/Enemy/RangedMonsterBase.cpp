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
	
	// 기절시 그냥 반환 eg(if(stun 몽타주가 발동중이 아니라면))
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