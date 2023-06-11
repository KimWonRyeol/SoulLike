// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	//�ش� ��Ÿ�ֿ� ������ ���� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Mixamo/KnightChar/Animation/ComboAttackMontage.ComboAttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>Dodge_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Mixamo/KnightChar/Animation/Dodge_F_Montage.Dodge_F_Montage'"));
	if (Dodge_MONTAGE.Succeeded())
	{
		DodgeMontage = Dodge_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>Parrying_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Mixamo/KnightChar/Animation/Defense_Montage.Defense_Montage'"));
	if (Parrying_MONTAGE.Succeeded())
	{
		ParryingMontage = Parrying_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage>Hit_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Mixamo/KnightChar/Animation/Defense_Montage.Defense_Montage'"));
	if (Hit_MONTAGE.Succeeded())
	{
		HitMontage = Hit_MONTAGE.Object;
	}
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>Dead_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Mixamo/KnightChar/Animation/Defense_Montage.Defense_Montage'"));
	if (Dead_MONTAGE.Succeeded())
	{
		DeadMontage = Dead_MONTAGE.Object;
	}*/
}

//ĳ������ �ִϸ��̼��� ��� ������Ʈ�ϴ� �Լ�(�̵��ӵ�, �������� ���� ���)
void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			CurrentWalkSpeed = Character->GetMovementComponent()->GetMaxSpeed();
		}
	}
}

//���� ��Ÿ�� ���� �Լ�
void UABAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

//���� ��Ÿ�� ������ �̵��ϴ� �Լ�
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

//���� ��Ÿ�ֿ��� ������ �̸� �������� �Լ�
FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	//���⼭ ������ �̸��� ����� ��Ƽ���� ����
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

//��Ÿ�� ���� �� �ش� ��Ƽ���̸� ������ ����Ǵ� �Լ�
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	//��������Ʈ�� ���ε��� �Լ��� ȣ��
	OnAttackHitCheck.Broadcast();
}

//��Ÿ�� ���� �� �ش� ��Ƽ���̸� ������ ����Ǵ� �Լ�
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	//��������Ʈ�� ���ε��� �Լ��� ȣ��
	OnNextAttackCheck.Broadcast();
}

//���� ��Ÿ�� ���� �Լ�
void UABAnimInstance::PlayDodgeMontage()
{
	if (!Montage_IsPlaying(DodgeMontage))
	{
		Montage_Play(DodgeMontage, 1.0f);
	}
}

//�и� ��Ÿ�� ���� �Լ�
void UABAnimInstance::PlayParryingMontage(float duration)
{
	if (!Montage_IsPlaying(ParryingMontage))
	{
		Montage_Play(ParryingMontage, 1.0f);
	}
}

//���� �޴� ��Ÿ�� ���� �Լ�
void UABAnimInstance::PlayHitMontage()
{
	if (!Montage_IsPlaying(HitMontage))
	{
		Montage_Play(HitMontage, 1.0f);
	}
}

//void UABAnimInstance::PlayDeadMontage()
//{
//	if (!Montage_IsPlaying(DeadMontage))
//	{
//		Montage_Play(DeadMontage, 1.0f);
//	}
//}



