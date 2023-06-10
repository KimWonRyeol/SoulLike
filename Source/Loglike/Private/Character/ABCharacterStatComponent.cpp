// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ABCharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ABPlayerState.h"
#include "GameFramework/ABSaveGame.h"
#include "GameFramework/LoglikeGameInstance.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	Name = 1; //삭제할지도...
	
	UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));
	ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot("Player1", 0));
	if (nullptr == ABSaveGame)
	{
		//만약 세이브 파일이 없다면 스탯을 기본값으로 설정
		SetMaxHealth(0.0f);
		SetDefaultDamage(0.0);
		SetDefaultLuck(0.0);
		SetDefaultAttackSpeed(0.0f);
		SetDefaultToken(10000);
		SetWeaponKey(false);
		SetArtifactKey(false);
		SetWeaponKeyRemain(1);
		SetArtifactKeyRemain(1);
		SetHpStatRemain(10);
		SetDamageStatRemain(10);
		SetLuckStatRemain(10);
	}
	else
	{
		SetMaxHealth(ABSaveGame->MaxHealth);
		SetDefaultDamage(ABSaveGame->DefaultDamage);
		SetDefaultLuck(ABSaveGame->DefaultLuck);
		SetDefaultAttackSpeed(ABSaveGame->DefaultAttackSpeed);
		SetDefaultToken(ABSaveGame->Token);
		SetArtifactKey(ABSaveGame->ArtifactKey);
		SetWeaponKey(ABSaveGame->WeaponKey);
		SetWeaponKeyRemain(ABSaveGame->WeaponKeyRemain);
		SetArtifactKeyRemain(ABSaveGame->ArtifactKeyRemain);
		SetHpStatRemain(ABSaveGame->HpStatRemain);
		SetDamageStatRemain(ABSaveGame->DamageStatRemain);
		SetLuckStatRemain(ABSaveGame->LuckStatRemain);
	}
}

// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("StatComponent!!!!!"));
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	/*UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));

	ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot("Player1", 0));
	if (nullptr == ABSaveGame)
		SetStartName(Name);
	else
	{
		SetMaxHealth(ABSaveGame->MaxHealth);
		SetDefaultDamage(ABSaveGame->DefaultDamage);
		SetDefaultLuck(ABSaveGame->DefaultLuck);
		SetDefaultAttackSpeed(ABSaveGame->DefaultAttackSpeed);
		SetToken(ABSaveGame->Token);
	}*/
}

//// Called every frame
//void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

//void UABCharacterStatComponent::SetStartName(int32 NewName) //삭제할지도...
//{
//	auto LoglikeGameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (LoglikeGameInstance != nullptr)
//	{
//		CurrentStatData = LoglikeGameInstance->GetABCharacterData(NewName);
//		if (nullptr != CurrentStatData)
//		{
//			Name = NewName;
//			SetMaxHealth(CurrentStatData->DefaultHealth);
//			SetDefaultDamage(CurrentStatData->DefaultDamage);
//			SetDefaultLuck(CurrentStatData->DefaultLuck);
//			SetDefaultAttackSpeed(CurrentStatData->DefaultAttackSpeed);
//			SetDefaultToken(CurrentStatData->Token);
//		}
//		else
//			UE_LOG(LogTemp, Error, TEXT("Name (%d) data doesn't exist"), NewName);
//	}
//	else
//		UE_LOG(LogTemp, Warning, TEXT("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"));
//}

void UABCharacterStatComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

void UABCharacterStatComponent::SetCurrentHealth(float NewCurrentHealth) //삭제할지도...
{
	CurrentHealth = NewCurrentHealth;
}

void UABCharacterStatComponent::SetAddHealth(float NewAddHealth) //삭제할지도...
{
	AddHealth += NewAddHealth;
}

void UABCharacterStatComponent::SetDefaultDamage(float NewDefaultDamage)
{
	DefaultDamage = NewDefaultDamage;
}

void UABCharacterStatComponent::SetAddDamage(float NewAddDamage) //삭제할지도...
{
	AddDamage += NewAddDamage;
}

void UABCharacterStatComponent::SetDefaultLuck(float NewDefaultLuck)
{
	DefaultLuck = NewDefaultLuck;
}

void UABCharacterStatComponent::SetAddLuck(float NewAddLuck) //삭제할지도...
{
	AddLuck += NewAddLuck;
}

void UABCharacterStatComponent::SetDefaultAttackSpeed(float NewDefaultAttackSpeed)
{
	DefaultAttackSpeed = NewDefaultAttackSpeed;
}

void UABCharacterStatComponent::SetAddAttackSpeed(float NewAddAttackSpeed) //삭제할지도...
{
	AddAttackSpeed += NewAddAttackSpeed;
}

void UABCharacterStatComponent::SetDefaultToken(int32 NewDefaultToken) 
{
	Token = NewDefaultToken;
}

void UABCharacterStatComponent::SetAddToken(int32 NewAddToken) //삭제할지도...
{
	Token += NewAddToken;
}

void UABCharacterStatComponent::AddLuck_(int32 amount)
{
	UE_LOG(LogTemp, Log, TEXT("Add Luck"));
	DefaultLuck += amount;
}

void UABCharacterStatComponent::AddDamage_(int32 amount)
{
	DefaultDamage += amount;
}

void UABCharacterStatComponent::AddHP_(int32 amount)
{
	MaxHealth += amount;
}

void UABCharacterStatComponent::AddToken_(int32 NewAddToken)
{
	Token += NewAddToken;
}

void UABCharacterStatComponent::SetWeaponKey(bool _WeaponKey)
{
	WeaponKey = _WeaponKey;
}

void UABCharacterStatComponent::SetArtifactKey(bool _ArtifactKey)
{
	ArtifactKey = _ArtifactKey;
}

bool UABCharacterStatComponent::GetWeaponKey()
{
	return WeaponKey;
}

bool UABCharacterStatComponent::GetArtifactKey()
{
	return ArtifactKey;
}

float UABCharacterStatComponent::GetMaxHealth()
{
	return MaxHealth;
}

float UABCharacterStatComponent::GetCurrentHealth() //삭제할지도...
{
	return CurrentHealth;
}

float UABCharacterStatComponent::GetDefaultDamage()
{
	return DefaultDamage;
}

float UABCharacterStatComponent::GetDefaultLuck()
{
	return DefaultLuck;
}

float UABCharacterStatComponent::GetDefaultAttackSpeed()
{
	return DefaultAttackSpeed;
}

int32 UABCharacterStatComponent::GetToken()
{
	return Token;
}

void UABCharacterStatComponent::SetWeaponKeyRemain(int32 WeaponKeyRemain_)
{
	WeaponKeyRemain = WeaponKeyRemain_;
}

void UABCharacterStatComponent::SetArtifactKeyRemain(int32 ArtifactKeyRemain_)
{
	ArtifactKeyRemain = ArtifactKeyRemain_;
}

void UABCharacterStatComponent::SetHpStatRemain(int32 HpStatRemain_)
{
	HpStatRemain = HpStatRemain_;
}

void UABCharacterStatComponent::SetDamageStatRemain(int32 DamageStatRemain_)
{
	DamageStatRemain = DamageStatRemain_;
}

void UABCharacterStatComponent::SetLuckStatRemain(int32 LuckStatRemain_)
{
	LuckStatRemain = LuckStatRemain_;
}

int32 UABCharacterStatComponent::GetWeaponKeyRemain()
{
	return WeaponKeyRemain;
}


int32 UABCharacterStatComponent::GetArtifactKeyRemain()
{
	return ArtifactKeyRemain;
}

int32 UABCharacterStatComponent::GetHpStatRemain()
{
	return HpStatRemain;
}

int32 UABCharacterStatComponent::GetDamageStatRemain()
{
	return DamageStatRemain;
}

int32 UABCharacterStatComponent::GetLuckStatRemain()
{
	return LuckStatRemain;
}