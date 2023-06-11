// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossAnimInstance.h"
#include "Enemy/Boss.h"

UBossAnimInstance::UBossAnimInstance()
{

}

void UBossAnimInstance::PlayComboAttackMontage(int32 ComboNum)
{
	this->PlayAttackMontage();
	if (Montage_IsPlaying(this->AttackMontage))
	{
		Montage_JumpToSection(FName(FString::FromInt(ComboNum)), this->AttackMontage);
	}
}

void UBossAnimInstance::PlayChargedAttackMontage()
{
	Montage_Play(ChargedAttackMontage);
}

void UBossAnimInstance::PlayJumpAttackMontage()
{
	Montage_Play(JumpAttackMontage);
}

void UBossAnimInstance::PlayRangedAttackMontage()
{
	Montage_Play(RangedAttackMontage);
}

void UBossAnimInstance::PlayLevelStartMontage()
{
	Montage_Play(LevelStartMontage);
}

void UBossAnimInstance::AnimNotify_AttackEnd()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	if (::IsValid(Boss))
	{
		Boss->AttackEnd();
	}
}

void UBossAnimInstance::AnimNotify_ChragedAtk()
{
	
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	if (::IsValid(Boss))
	{
		
		int32 MonIndex = FCString::Atof(*(Montage_GetCurrentSection().ToString()));
		Boss->ChargedAttackStart(MonIndex);
	}
}

 void UBossAnimInstance::AnimNotify_JumpHit()
{
	 ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	 if (::IsValid(Boss))
	 {
		 Boss->JumpAttackEnd();
	 }
}

void UBossAnimInstance::AnimNotify_JumpAtk()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	if (::IsValid(Boss))
	{
		Boss->JumpAttackStart();
	}
}

void UBossAnimInstance::AnimNotify_RangedAtk()
{
	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());
	if (::IsValid(Boss))
	{
		Boss->RangedAttackStart();
	}
}