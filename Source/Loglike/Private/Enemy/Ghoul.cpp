// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ghoul.h"
#include "Enemy/MonsterAnimBase.h"
#include "Character/LoglikeCharacter.h"
#include "Enemy/MeleeMonsterAIController.h"
#include "Components/BoxComponent.h"
AGhoul::AGhoul()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/Ghoul/Mesh/SK_Ghoul_Full.SK_Ghoul_Full'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/UndeadPack/Ghoul/Animations/AnimBP_Ghoul.AnimBP_Ghoul_C'"));
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
	HealthPoint = 50.f;
	Damage = 3.f;
	MovementSpeed = 400.f;
	AttackSpeed = 2.33f;

	//AI
	AIControllerClass = AMeleeMonsterAIController::StaticClass();
}

void AGhoul::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	LeftBox->AttachToComponent(GetMesh(), AttachmentRules, "LeftFist");
	RightBox->AttachToComponent(GetMesh(), AttachmentRules, "RightFist");

	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	RightBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBase::OnOverlapBegin);
	AttackEnd();
}

void AGhoul::AttackStart()
{
	LeftBox->SetGenerateOverlapEvents(true);
	RightBox->SetGenerateOverlapEvents(true);
}

void AGhoul::AttackEnd()
{
	LeftBox->SetGenerateOverlapEvents(false);
	RightBox->SetGenerateOverlapEvents(false);
}
