// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Goblin.h"
#include "Enemy/MonsterAnimBase.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MeleeMonsterAIController.h"
#include "Components/BoxComponent.h"

AGoblin::AGoblin()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/EnemyGoblin/Mesh/SM_EnemyGoblin.SM_EnemyGoblin'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/UndeadPack/EnemyGoblin/Animations/BP_Goblin.BP_Goblin_C'"));
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
	HealthPoint = 20.f;
	Damage = 3.f;
	MovementSpeed = 400.f;
	AttackSpeed = 2.53f;

	//AI
	AIControllerClass = AMeleeMonsterAIController::StaticClass();
}

void AGoblin::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftBox->AttachToComponent(GetMesh(), AttachmentRules, "LeftFist");
	RightBox->AttachToComponent(GetMesh(), AttachmentRules, "RightFist");

	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	RightBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	AttackEnd();
}

void AGoblin::AttackStart()
{
	LeftBox->SetGenerateOverlapEvents(true);
	RightBox->SetGenerateOverlapEvents(true);
}

void AGoblin::AttackEnd()
{
	LeftBox->SetGenerateOverlapEvents(false);
	RightBox->SetGenerateOverlapEvents(false);
}



