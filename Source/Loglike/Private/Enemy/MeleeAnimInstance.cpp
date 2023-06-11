// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeAnimInstance.h"
#include "Enemy/MonsterBase.h"
UMeleeAnimInstance::UMeleeAnimInstance()
{

}

void UMeleeAnimInstance::PlayComboAttackMontage(int32 ComboNum)
{
	this->PlayAttackMontage();
	if (Montage_IsPlaying(this->AttackMontage))
	{
		Montage_JumpToSection(FName(FString::FromInt(ComboNum)), this->AttackMontage);
	}
}

void UMeleeAnimInstance::AnimNotify_AttackStart()
{
	AMonsterBase* Monster = Cast<AMonsterBase>(TryGetPawnOwner());
	if (::IsValid(Monster))
	{
		Monster->AttackStart();
	}
}

void UMeleeAnimInstance::AnimNotify_AttackEnd()
{
	AMonsterBase* Monster = Cast<AMonsterBase>(TryGetPawnOwner());
	if (::IsValid(Monster))
	{
		Monster->AttackEnd();
	}
}
