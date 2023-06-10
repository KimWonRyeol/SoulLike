// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Lich.h"
#include "Enemy/MonsterAnimBase.h"
#include "Enemy/Projectile.h"
#include "Enemy/RangedMonsterAIController.h"
#include "Character/LoglikeCharacter.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"

ALich::ALich()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_LICH(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/Lich/Mesh/SK_Lich.SK_Lich'"));
	if (SK_LICH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_LICH.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> LICH_ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/UndeadPack/Lich/Animations/AnimBP_Lich.AnimBP_Lich_C'"));
	if (LICH_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(LICH_ANIM.Class);
	}

	HealthPoint = 20.f;
	MovementSpeed = 400.f;
	AttackSpeed = 2.f;

	AIControllerClass = ARangedMonsterAIController::StaticClass();
}

void ALich::AttackStart()
{
	if (IsShooting)
	{
		if (Projectile)
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(0.f, 0.f, 88.f));
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation);
			}
		}
	}
}

