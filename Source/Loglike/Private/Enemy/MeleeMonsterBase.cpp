// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeMonsterBase.h"
#include "Enemy/MeleeAnimInstance.h"
#include "Enemy/MeleeMonsterAIController.h"

#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AMeleeMonsterBase::AMeleeMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ComboNum = 0;
	IsAttacking = false;

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);
	TextRenderComponent->Text = FText::FromString("!");
	TextRenderComponent->SetTextRenderColor(FColor(255, 0, 0));
	TextRenderComponent->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	FVector TextPos = FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TextRenderComponent->SetRelativeLocation(TextPos);
	
}

void AMeleeMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MonsterAnim->OnMontageEnded.AddDynamic(this, &AMeleeMonsterBase::OnMontageEnded);
	TextRenderComponent->SetVisibility(false);
}
void  AMeleeMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AMeleeMonsterBase::ComboAttack()
{
	//Cast<AMeleeMonsterAIController>(GetController())->SetStun(true, 10.f);
	IsAttacking = true;
	Cast<UMeleeAnimInstance>(MonsterAnim)->PlayComboAttackMontage(ComboNum);
	ComboNum++;
	ComboNum %= 2;
}
void AMeleeMonsterBase::PushBack(AActor* PivotActor)
{
	FVector DamageDirection = PivotActor->GetActorLocation() - GetActorLocation();
	DamageDirection.Z = 0.f;
	DamageDirection.Normalize();
	DamageDirection *= -1;

	LaunchCharacter(DamageDirection * 1200.f, true, true);
}
void AMeleeMonsterBase::Stun()
{
	TextRenderComponent->SetVisibility(true);
	PushBack(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Cast<AMeleeMonsterAIController>(GetController())->SetStun(true, 10.f);
	Cast<UMeleeAnimInstance>(MonsterAnim)->PlayStunMontage();
}

void AMeleeMonsterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//어택 이외의 몽타주가 끝나면 Combo를 초기화

	if (!Montage->GetName().Contains("Attack"))
	{
		ComboNum = 0;
	}

	if (Montage->GetName().Contains("Pain"))  // Pain Montage 후 BT실행가능
	{
		Cast<AMeleeMonsterAIController>(GetController())->SetPain(false, 0.f);
	}
	else if (Montage->GetName().Contains("Attack") )// Attack Montage 후 BT실행가능
	{
		IsAttacking = false;
	}
	else if (Montage->GetName().Contains("Stun"))
	{
		TextRenderComponent->SetVisibility(false);
		Cast<AMeleeMonsterAIController>(GetController())->SetStun(false, 0.f);
	}
	AttackEnd();
}
