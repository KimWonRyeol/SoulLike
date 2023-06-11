// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MonsterAnimBase.h"
#include "Enemy/MonsterBase.h"

UMonsterAnimBase::UMonsterAnimBase()
{
	IsDead = false;
}

void UMonsterAnimBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UMonsterAnimBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!::IsValid(Character)) { return; }

	if (!IsDead)
	{
		Speed = Character->GetVelocity().Size();
	}
	
}

void UMonsterAnimBase::PlayAttackMontage()
{
	if (Montage_IsPlaying(PainMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("PainMontage"));
	}
	Montage_Play(AttackMontage);
	
}

void UMonsterAnimBase::PlayPainMontage()
{
	Montage_Play(PainMontage);
}

void UMonsterAnimBase::PlayStunMontage()
{
	if (Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(StunMontage,1.3f);
	}
}

void UMonsterAnimBase::AnimNotify_Attack()
{
	AMonsterBase* Monster = Cast<AMonsterBase>(TryGetPawnOwner());
	if (::IsValid(Monster))
	{
		Monster->AttackStart();
	}
}