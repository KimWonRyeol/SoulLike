// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABWeapon.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "UI/CPPSlot.h"
#include "UI/Weapon.h"
#include "Character/LoglikeCharacter.h"
#include "Character/ABCharacterStatComponent.h"
#include "UI/StageNodeWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMesh.h"

// Sets default values
AABWeapon::AABWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	//게임 인스턴스 객체 생성
	//auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	//ULoglikeGameInstance* ABGameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (ABGameInstance != nullptr) {
	//	if (!ABGameInstance->GetWeaponData().Item.IsNull())
	//	{
	//		WeaponSlot = ABGameInstance->GetWeaponData();
	//	}
	//	if (!WeaponSlot.Item.IsNull())
	//	{
	//		WeaponData = WeaponSlot.Item.DataTable->FindRow<FWeapon>(WeaponSlot.Item.RowName, FString("LookUpWeaponData"));
	//		UE_LOG(LogTemp, Warning, TEXT("Get Weapon@@@@@@@ %s"), *WeaponData->Name_);
	//		//웨폰의 이름에 따라서 각기 다른 레퍼런스 경로를 설정할 예정...?
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("@@@@@@@ "));
	//}

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Script/Engine.StaticMesh'/Game/Sword_Fx/Mesh/SM_Sword01.SM_Sword01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Script/Engine.StaticMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SM_Blade_HeroSword11.SM_Blade_HeroSword11'"));
	if (SK_WEAPON.Succeeded())
		Weapon->SetStaticMesh(SK_WEAPON.Object);

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AABWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABWeapon::ChangeWeapon(FString NewWeapon)
{
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	FWeapon* WeaponData = ABGameInstance->GetWeapon();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *WeaponData->Name_);

	UStaticMesh* MyStaticMesh = WeaponData->StaticMesh;
	//FString test = FString(TEXT("/Script/Engine.StaticMesh'%s'", *MyStaticMesh->GetPathName()));
	FString test = TEXT("/Script/Engine.StaticMesh'") + MyStaticMesh->GetPathName() + TEXT("'");
	UE_LOG(LogTemp, Warning, TEXT("/Script/Engine.StaticMesh'/Game/Sword_Fx/Mesh/SM_Rare.SM_Rare'"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *test);

	UStaticMesh* NewStaticMesh = nullptr;
	FStreamableManager StreamableManager;
	NewStaticMesh = StreamableManager.LoadSynchronous<UStaticMesh>(FSoftObjectPath(test));
	if(NewStaticMesh == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("qqqqqqqqqqq"));

	Weapon->SetStaticMesh(NewStaticMesh);

	/*if (WeaponData->Name_ == "Rare1")
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON1(TEXT("/Script/Engine.StaticMesh'/Game/Sword_Fx/Mesh/SM_Rare.SM_Rare'"));
		if (SK_WEAPON1.Succeeded())
			Weapon->SetStaticMesh(SK_WEAPON1.Object);
	}*/

	////static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON((TEXT("%s"), *NewWeapon));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Script/Engine.StaticMesh'/Game/Sword_Fx/Mesh/SM_Rare.SM_Rare'"));

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

