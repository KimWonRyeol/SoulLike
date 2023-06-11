// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeSkeleton.h"
#include "Enemy/MonsterAnimBase.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MeleeMonsterAIController.h"
#include "Components/BoxComponent.h"

AMeleeSkeleton::AMeleeSkeleton()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/SK_Skeleton.SK_Skeleton'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/UndeadPack/SkeletonEnemy/Animations/Melee/AnimBP_MeleeSkeleton.AnimBP_MeleeSkeleton_C'"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	//Sword
	Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SWORD(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/Weapon/Sword/SK_Sword.SK_Sword_SM_Sword'"));
	if (SK_SWORD.Succeeded())
	{
		Sword->SetSkeletalMesh(SK_SWORD.Object);
	}

	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetCollisionProfileName(TEXT("OverlapAll"));

	//init
	HealthPoint = 30.f;
	Damage = 5.f;
	MovementSpeed = 400.f;
	AttackSpeed = 3.52f/2.f;

	//AI
	AIControllerClass = AMeleeMonsterAIController::StaticClass();
}

void AMeleeSkeleton::BeginPlay()
{
	Super::BeginPlay();
	
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	Sword->AttachToComponent(GetMesh(), AttachmentRules, "SKT_Sword");
	OpacitySkeletalMesh.Add(Sword); //Sword를 OpacitySkeletalMesh에 추가
	SwordCollision->AttachToComponent(Sword, AttachmentRules, "SwordCol");

	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	AttackEnd();
}

void AMeleeSkeleton::AttackStart()
{
	SwordCollision->SetGenerateOverlapEvents(true);
}

void AMeleeSkeleton::AttackEnd()
{
	SwordCollision->SetGenerateOverlapEvents(false);
}
