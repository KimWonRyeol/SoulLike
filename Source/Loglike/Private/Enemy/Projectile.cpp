// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Projectile.h"
#include "Character/LoglikeCharacter.h"

#include "Engine/DamageEvents.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/SparseDelegate.h"

// Sets default values
AProjectile::AProjectile()
{ 
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	InitialLifeSpan = 10.0f;
	Damage = 5.f;

	// UE_LOG(LogTemp, Warning, TEXT("Projectile Init"));

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Cast<ALoglikeCharacter>(OtherActor) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Projectile Hit"), *(OtherActor->GetName()));
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, GetController(), this);
		Destroy();
	}
	else
	{
		Destroy();
	}
	return;
}

