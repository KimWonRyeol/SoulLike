// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABItemBox.h"
#include "Character/LoglikeCharacter.h"
#include "Character/ABWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

    RootComponent = Trigger;
    Box->SetupAttachment(RootComponent);

    Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Script/Engine.StaticMesh'/Game/Source/AncientTreasures/Meshes/SM_Chest_01e.SM_Chest_01e'"));
    if (SM_BOX.Succeeded())
    {
        Box->SetStaticMesh(SM_BOX.Object);
    }
    Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

    Trigger->SetCollisionProfileName(TEXT("ItemBox"));
    Box->SetCollisionProfileName(TEXT("NoCollision"));

    WeaponItemClass = AABWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    // AddDynamic 매크로를 통해 OnCharacterOverlap 함수를 OnComponentBeginOverlap 델리게이트에 추가
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}


// Called every frame
void AABItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("asdasd"));
    auto LogLikeCharacter = Cast<ALoglikeCharacter>(OtherActor);
    if (LogLikeCharacter != nullptr && WeaponItemClass != nullptr)
    {
        auto NewWeapon = GetWorld()->SpawnActor<AABWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
        LogLikeCharacter->SetWeapon(NewWeapon);
        Box->SetHiddenInGame(true, true);
        SetActorEnableCollision(false);
    }
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
}

