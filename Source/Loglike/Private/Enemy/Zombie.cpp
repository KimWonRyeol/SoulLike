// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Zombie.h"
#include "Enemy/MonsterAnimBase.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MeleeMonsterAIController.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"


AZombie::AZombie()
{
	//Set Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/Zombie/Mesh/SK_Zombie.SK_Zombie'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/UndeadPack/Zombie/Animations/AnimBP_Zombie.AnimBP_Zombie_C'"));
	if (ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	
	//BoxComponent
	LeftBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistBox"));
	RightBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistBox"));
	
	LeftBox->SetCollisionProfileName(TEXT("OverlapAll"));
	RightBox->SetCollisionProfileName(TEXT("OverlapAll"));


	//init
	HealthPoint = 10.f;
	Damage = 5.f;
	MovementSpeed = 300.f;
	AttackSpeed = 4.43f;

	//AI
	AIControllerClass = AMeleeMonsterAIController::StaticClass();
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftBox->AttachToComponent(GetMesh(), AttachmentRules, "LeftFist");
	RightBox->AttachToComponent(GetMesh(), AttachmentRules, "RightFist");

	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	RightBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	AttackEnd();
}

void AZombie::AttackStart()
{
	LeftBox->SetGenerateOverlapEvents(true);
	RightBox->SetGenerateOverlapEvents(true);
}

void AZombie::AttackEnd()
{
	LeftBox->SetGenerateOverlapEvents(false);
	RightBox->SetGenerateOverlapEvents(false);
}


