// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	//해당 몽타주와 연결할 변수 선언
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

//캐릭터의 애니메이션을 계속 업데이트하는 함수(이동속도, 떨어지는 유무 등등)
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

//공격 몽타주 실행 함수
void UABAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

//공격 몽타주 섹션을 이동하는 함수
void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

//공격 몽타주에서 색션의 이름 가져오는 함수
FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	//여기서 색션의 이름을 맞춰야 노티파이 실행
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

//몽타주 실행 중 해당 노티파이를 만나면 실행되는 함수
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	//델리게이트에 바인딩된 함수를 호출
	OnAttackHitCheck.Broadcast();
}

//몽타주 실행 중 해당 노티파이를 만나면 실행되는 함수
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	//델리게이트에 바인딩된 함수를 호출
	OnNextAttackCheck.Broadcast();
}

//닷지 몽타주 실행 함수
void UABAnimInstance::PlayDodgeMontage()
{
	if (!Montage_IsPlaying(DodgeMontage))
	{
		Montage_Play(DodgeMontage, 1.0f);
	}
}

//패링 몽타주 실행 함수
void UABAnimInstance::PlayParryingMontage(float duration)
{
	if (!Montage_IsPlaying(ParryingMontage))
	{
		Montage_Play(ParryingMontage, 1.0f);
	}
}

//공격 받는 몽타주 실행 함수
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



