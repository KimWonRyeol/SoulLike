// Fill out your copyright notice in the Description page of Project Settings. 


#include "Enemy/Boss.h"
#include "Enemy/BossAnimInstance.h"
#include "Enemy/BossAIController.h"
#include "Enemy/Projectile.h"

#include "UI/BossHPBar.h"
#include "Blueprint/UserWidget.h"

#include "Character/LoglikeCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "Engine/DamageEvents.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ABoss::ABoss()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage.Rampage'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AB_BOSS(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonRampage/Characters/Heroes/Rampage/Animations/UsedAsset/AnimBP_Boss.AnimBP_Boss_C'"));
	if (AB_BOSS.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AB_BOSS.Class);
	}

	/*static ConstructorHelpers::FClassFinder<UUserWidget> HPBAR(TEXT("/Game/ParagonRampage/Blueprints/UI_BossHPBar.UI_BossHPBar_C"));
	if (HPBAR.Succeeded())
	{
		BossHpWidgetClass = HPBAR.Class;
	}*/

	//BoxComponent
	LeftBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistBox"));
	RightBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistBox"));
	
	LeftBox->SetCollisionProfileName(TEXT("OverlapAll"));
	RightBox->SetCollisionProfileName(TEXT("OverlapAll"));

	// init
	HealthPoint = 500.f;  // temp stat (400.f)
	MovementSpeed = 400.f;
	AttackSpeed = 2.f;

	//Combo Attack
	ComboNum = 0;
	//Damage
	ChargedAttackDamage = 10;
	JumpAttackDamage = 10;
	AttackDamageMin = 3;
	AttackDamageMax = 5;
	IsAttacking = false;


	// AI
	AIControllerClass = ABossAIController::StaticClass();
}

void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Anim
	MonsterAnim->OnMontageEnded.AddDynamic(this, &ABoss::OnMontageEnded);


}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftBox->AttachToComponent(GetMesh(), AttachmentRules, "LeftFist");
	RightBox->AttachToComponent(GetMesh(), AttachmentRules, "RightFist");

	CheckHP = HealthPoint;

	//MeleeAttackInit
	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	RightBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	AttackEnd();

	// Attack Test
	// JumpAttack();
	// RangedAttack();
	// Attack();
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE_LOG(LogTemp, Warning, TEXT("Boss Actor : %s HP : %f"), *GetName(), HealthPoint);

	if (HealthPoint <= 0)
	{
		if (BossHpWidget != nullptr)
		{
			BossHpWidget->RemoveFromViewport();
		}
	}
	else if (HealthPoint < CheckHP)
	{
		CheckHitPoint();
		BossHpWidget->UpdateHPWidget(HealthPoint);
		CheckHP = HealthPoint;
	}
}

void ABoss::SetHPbar()
{
	if (BossHpWidgetClass != nullptr)
	{
		BossHpWidget = Cast<UBossHPBar>(CreateWidget(GetWorld(), BossHpWidgetClass));

		if (BossHpWidget != nullptr)
		{
			BossHpWidget->AddToViewport();
		}
	}
}

void ABoss::Stun()
{
	PushBack(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Cast<ABossAIController>(GetController())->SetStun(true, 10.f);
	Cast<UBossAnimInstance>(MonsterAnim)->PlayStunMontage();
}

void ABoss::PushBack(AActor* PivotActor)
{
	FVector DamageDirection = PivotActor->GetActorLocation() - GetActorLocation();
	DamageDirection.Z = 0.f;
	DamageDirection.Normalize();
	DamageDirection *= -1;

	LaunchCharacter(DamageDirection * 1200.f, true, true);
}

void ABoss::AttackStart()
{
	LeftBox->SetGenerateOverlapEvents(true);
	RightBox->SetGenerateOverlapEvents(true);
}

void ABoss::AttackEnd()
{
	LeftBox->SetGenerateOverlapEvents(false);
	RightBox->SetGenerateOverlapEvents(false);
}

void ABoss::Attack()
{
	IsAttacking = true;

	Damage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	Cast<UBossAnimInstance>(MonsterAnim)->PlayComboAttackMontage(ComboNum);
	Cast<ABossAIController>(GetController())->CountAttack();
	ComboNum++;
	ComboNum %= 3;
}

void ABoss::ChargedAttack()
{
	Cast<UBossAnimInstance>(MonsterAnim)->PlayChargedAttackMontage();
}

void ABoss::JumpAttack()
{
	Cast<UBossAnimInstance>(MonsterAnim)->PlayJumpAttackMontage();
	Jump();
}

void ABoss::RangedAttack()
{
	Cast<UBossAnimInstance>(MonsterAnim)->PlayRangedAttackMontage();
}

void ABoss::ChargedAttackStart(int32 FistIndex)
{
	FVector FistLoc;
	switch (FistIndex)
	{
	case 1:
		FistLoc = GetMesh()->GetSocketLocation(FName("LeftFist"));
		break;
	case 0:
		FistLoc = GetMesh()->GetSocketLocation(FName("RightFist"));
		break;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		FistLoc,
		FistLoc + 120.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(120.f),
		Params
	);

	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(
		GetWorld(),
		FistLoc,
		120.f,
		120.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		3.f);
	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(ChargedAttackDamage, DamageEvent, GetController(), this);
		}
	}
}

void ABoss::JumpAttackStart()
{
	APawn* Target = UGameplayStatics::GetPlayerPawn(this, 0);
	FVector TargetLoc = Target->GetActorLocation();

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(
		GetCapsuleComponent(),
		TargetLoc,
		(Target->GetActorLocation() - GetActorLocation()).Rotation(),
		true,
		true,
		0.35f,
		false,
		EMoveComponentAction::Type::Move,
		Info);
}

void ABoss::JumpAttackEnd()
{
	FVector TargetLoc = GetMesh()->GetSocketLocation(FName("LeftFist")) + GetActorRightVector() * 75.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		TargetLoc,
		TargetLoc + 100.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(100.f),
		Params
	);

	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(
		GetWorld(),
		TargetLoc,
		100.f,
		100.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		3.f);
	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(ChargedAttackDamage, DamageEvent, GetController(), this);
		}
	}
}

void ABoss::SpawnEnemies()
{
	TArray<TSubclassOf<AMonsterBase>> SelectedEnemy;

	while (SelectedEnemy.Num() < 3)
	{
		TSubclassOf<AMonsterBase> RandomEnemyClass = Enemies[FMath::RandRange(0, Enemies.Num() - 1)];
		if (!SelectedEnemy.Contains(RandomEnemyClass))
		{
			SelectedEnemy.Add(RandomEnemyClass);
			AMonsterBase* SpawnedEnemy = GetWorld()->SpawnActor<AMonsterBase>(RandomEnemyClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (SpawnedEnemy)
			{
				SpawnedEnemies.Add(SpawnedEnemy);
			}
		}
	}
}

void ABoss::RangedAttackStart()
{
	TSubclassOf<AProjectile> Rock_0 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_1 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_2 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_3 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));
	TSubclassOf<AProjectile> Rock_4 = StaticLoadClass(AProjectile::StaticClass(), NULL, TEXT("/Script/Engine.Blueprint'/Game/ParagonRampage/Blueprints/BP_BossProjectile.BP_BossProjectile_C'"));

	TArray<TSubclassOf<AProjectile>> Rocks;

	Rocks.Emplace(Rock_0);
	Rocks.Emplace(Rock_1);
	Rocks.Emplace(Rock_2);
	Rocks.Emplace(Rock_3);
	Rocks.Emplace(Rock_4);

	if (Projectile)
	{
		const FRotator ActorRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + ActorRotation.RotateVector(FVector(0.f, 0.f, 88.f));
		const FRotator SpawnRotation = FRotator(0.f, ActorRotation.Yaw + 80.f, 0.f);
		float dist = 33.f;
		int index = 1;

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (ensure(Rocks.Num() > 0))
			{
				for (auto& Rock : Rocks)
				{
					World->SpawnActor<AProjectile>(Rock, SpawnLocation, FRotator(0.f, SpawnRotation.Yaw - (dist * index++), 0.f));
				}

			}
		}
	}
}

void ABoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//Attack test
	if (!Montage->GetName().Contains("_Attack"))
	{
		ComboNum = 0;
	}

	if (Montage->GetName().Contains("Hit"))
	{
		Cast<ABossAIController>(GetController())->SetPain(false, 0.f);
	}
	else if (Montage->GetName().Contains("Stun"))
	{
		Cast<ABossAIController>(GetController())->SetStun(false, 0.f);
	}

	if (Montage->GetName().Contains("_Attack"))// Attack Montage 후 BT실행가능
	{
		IsAttacking = false;
	}
	else if (Montage->GetName().Contains("Charged"))
	{
		ChargedAttackIsEnded.Broadcast();
	}
	else if (Montage->GetName().Contains("Jump"))
	{
		JumpAttackIsEnded.Broadcast();
	}
	else if (Montage->GetName().Contains("Ranged"))
	{
		RangedAttackIsEnded.Broadcast();
	}

	AttackEnd();
}

void ABoss::CheckHitPoint()
{
	Cast<ABossAIController>(GetController())->BossHitPoint(HealthPoint);
}

void ABoss::ReadyToSkill(bool enable)
{
	Cast<ABossAIController>(GetController())->SetSkill(enable);
}

void ABoss::AttackCount()
{
	Cast<ABossAIController>(GetController())->CountAttack();
}