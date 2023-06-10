// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/LoglikeGameInstance.h"
#include "UI/StageNodeWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CPPSlot.h"
#include "UI/Weapon.h"
#include "UI/WeaponEffect.h"
#include "UI/ArtifactEffect.h"
#include "UI/Artifact.h"
#include "Character/ABCharacterStatComponent.h"
#include "Character/LoglikeCharacter.h"
#include "Sound/SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/ABSaveGame.h"

ULoglikeGameInstance::ULoglikeGameInstance()
{
    CurrentStageType = EStageType::E_None;
    
    /*FString CharacterDataPath = TEXT("/Script/Engine.DataTable'/Game/Source/CharacterData/ABCharacterData123.ABCharacterData123'");
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
    ABCharacterTable = DT_ABCHARACTER.Object;*/

    CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));
    MaxHealth = 100.0f + (CharacterStat->GetMaxHealth() * 5);
    AddHealth = 0.0f;
    CurrentHealth = 100.0f + (CharacterStat->GetMaxHealth() * 5);
    AddDamage = 0.0f;
    AddLuck = 0.0f;
    AddAttackSpeed = 0.0f;
    AddToken = 0;

    //UDataTable* WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Weapon/Weapons.Weapons'"));
    WeaponSlot.Item.DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Weapon/Weapons.Weapons'"));
    WeaponSlot.Item.RowName = FName(TEXT("Common"));

    WeaponData = WeaponSlot.Item.DataTable->FindRow<FWeapon>(WeaponSlot.Item.RowName, FString("LookUpWeaponData"));

    WeaponDamage = float(WeaponData->AtkPower);

    memset(&WeaponEffect, 0, sizeof(FWeaponEffect));
    memset(&ArtifactEffect, 0, sizeof(FArtifactEffect));
}

void ULoglikeGameInstance::Init()
{
    Super::Init();
    SelectedNodeArr.Empty();
}

TArray<int32> ULoglikeGameInstance::GetSelectedNodeArr()
{
    return SelectedNodeArr;
}

void ULoglikeGameInstance::AddSelectedNode(int32 NodeNum)
{
    SelectedNodeArr.AddUnique(NodeNum);
}

void ULoglikeGameInstance::InitDungeonMode()
{
    UABSaveGame* ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::CreateSaveGameObject(UABSaveGame::StaticClass()));
    ABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot("Player1", 0));

    CurrentStageType = EStageType::E_None;
    SelectedNodeArr.Empty();    //Init SelectNodeArr
    ClearStageArray.Empty();    //Init ClearStageArr

    if (ABSaveGame != nullptr)
    {
        MaxHealth = 100.0f + (ABSaveGame->MaxHealth * 5);
        CurrentHealth = 100.0f + (ABSaveGame->MaxHealth * 5);
    }
    else
    {
        MaxHealth = 100.0f + (CharacterStat->GetMaxHealth() * 5);
        CurrentHealth = 100.0f + (CharacterStat->GetMaxHealth() * 5);
    }
    AddHealth = 0.0f;
    AddDamage = 0.0f;
    AddLuck = 0.0f;
    AddAttackSpeed = 0.0f;
    AddToken = 0;

    WeaponSlot.Item.DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Weapon/Weapons.Weapons'"));
    WeaponSlot.Item.RowName = FName(TEXT("Common"));

    //무기 효과 초기화
    memset(&WeaponEffect, 0, sizeof(FWeaponEffect));
    WeaponData = WeaponSlot.Item.DataTable->FindRow<FWeapon>(WeaponSlot.Item.RowName, FString("LookUpWeaponData"));
    WeaponDamage = float(WeaponData->AtkPower);

    ArtifactSlots.Empty();
    ArtifactDatas.Empty();

    CurrentHealth = MaxHealth;
    ResetArtifactEffect();

    ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if(PlayerCharacter != nullptr)
        PlayerCharacter->SetUpdateStat();
}

//FABCharacterData* ULoglikeGameInstance::GetABCharacterData(int32 Name)
//{
//    return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Name), TEXT(""));
//}

void ULoglikeGameInstance::SetMaxHealth(float NewMaxHealth)
{
    MaxHealth = NewMaxHealth;
}

void ULoglikeGameInstance::SetCurrentHealth(float NewCurrentHealth)
{
    if (NewCurrentHealth > 0 && NewCurrentHealth < 1) //매개변수로 퍼센트가 들어올때(석상 회복)
    {
        CurrentHealth = CurrentHealth + MaxHealth * NewCurrentHealth;
    }
    else
    {
        CurrentHealth += NewCurrentHealth;
    }
    
    if (CurrentHealth < 0) 
    {
        CurrentHealth = 0;
    }
    else if (CurrentHealth >= MaxHealth)
    {
        CurrentHealth = MaxHealth;
    }
}

void ULoglikeGameInstance::SetAddHealth(float NewAddHealth)
{
    AddHealth += NewAddHealth;
    MaxHealth = 100.0f + AddHealth * 5;
}

void ULoglikeGameInstance::SetAddDamage(float NewAddDamage)
{
    AddDamage += NewAddDamage;
}

void ULoglikeGameInstance::SetAddLuck(float NewAddLuck)
{
    AddLuck += NewAddLuck;
}

void ULoglikeGameInstance::SetAddAttackSpeed(float NewAddAttackSpeed)
{
    AddAttackSpeed += NewAddAttackSpeed;
}

void ULoglikeGameInstance::SetAddToken(int32 NewAddToken)
{
    AddToken += NewAddToken;
}

void ULoglikeGameInstance::SetTotalAddToken(int32 NewTotalAddToken)
{
    TotalAddToken += NewTotalAddToken;
}

float ULoglikeGameInstance::GetAddHealth()
{
    return AddHealth;
}

float ULoglikeGameInstance::GetCurrentHealth()
{
    return CurrentHealth;
}

float ULoglikeGameInstance::GetAddDamage()
{
    return AddDamage;
}

float ULoglikeGameInstance::GetAddLuck()
{
    return AddLuck;
}

float ULoglikeGameInstance::GetAddAttackSpeed()
{
    return AddAttackSpeed;
}

int32 ULoglikeGameInstance::GetAddToken()
{
    return AddToken;
}

int32 ULoglikeGameInstance::GetTotalAddToken()
{
    return TotalAddToken;
}

FWeapon* ULoglikeGameInstance::GetWeapon()
{
    return WeaponData;
}

TArray<FArtifact*> ULoglikeGameInstance::GetArtifact()
{
    return ArtifactDatas;
}

float ULoglikeGameInstance::GetWeaponDamage()
{
    return WeaponDamage;
}

void ULoglikeGameInstance::SetTrueWeaponKey()
{
    WeaponKey = true;
}

void ULoglikeGameInstance::SetTrueArtifactKey()
{
    ArtifactKey = true;
}

FCPPSlot ULoglikeGameInstance::GetWeaponData()
{
    return WeaponSlot;
}

void ULoglikeGameInstance::SetWeaponData(FCPPSlot CurWeapon)
{
    WeaponSlot = CurWeapon;
    WeaponData = WeaponSlot.Item.DataTable->FindRow<FWeapon>(WeaponSlot.Item.RowName, FString("LookUpWeaponData"));
    
    WeaponDamage = float(WeaponData->AtkPower);

    /* 고유효과를 가진 무기 효과 적용 */
    //효과 적용 전 전의 무기 효과 초기화
    memset(&WeaponEffect, 0, sizeof(FWeaponEffect));
    if(WeaponData->Effect != "No effect")
        ApplyWeaponEffect(WeaponData->Name_);

    ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    PlayerCharacter->SetUpdateStat();

    UE_LOG(LogTemp, Warning, TEXT("Get Weapon!!!!!! %s"), *WeaponData->Name_);
}

void ULoglikeGameInstance::ApplyWeaponEffect(FString weaponName)
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *weaponName);
    if (weaponName == "Ebonshard") WeaponEffect.ParryingAtk = true;
    else if (weaponName == "Drakenfang") WeaponEffect.FinalCombo = true;
    else if (weaponName == "Thunderforgod") WeaponEffect.ParryingDuration = true;
    else if (weaponName == "Bloodripper") WeaponEffect.Leech = true;
    else if (weaponName == "FatalFlour") WeaponEffect.GlassCannon = true;
}

struct FWeaponEffect ULoglikeGameInstance::GetWeaponEffect()
{
    return WeaponEffect;
}

FArtifactEffect ULoglikeGameInstance::GetArtifactEffect()
{
    return ArtifactEffect;
}

TArray<FCPPSlot> ULoglikeGameInstance::GetArtifactData()
{
    return ArtifactSlots;
}

void ULoglikeGameInstance::SetArtifactData(TArray<FCPPSlot> CurArtifacts)
{
    int index = 0;
    ArtifactSlots.Empty();
    ArtifactDatas.Empty();
    ResetArtifactEffect();
    ArtifactEffect_MaxHealth = 0.0f;
    for (FCPPSlot ArtifactSlot : CurArtifacts)
    {
        ArtifactSlots.Add(ArtifactSlot);
        ArtifactDatas.Add(ArtifactSlot.Item.DataTable->FindRow<FArtifact>(ArtifactSlot.Item.RowName, FString("LookUpArtifactData")));
    }

    for (FArtifact* ArtifactData : ArtifactDatas)
    {
        if (ArtifactData->Name_ == "Ruby Amulet ")
        {
            if (ArtifactData->Grade == "Common") ArtifactEffect_MaxHealth += 10.0f;
            else if (ArtifactData->Grade == "Rare") ArtifactEffect_MaxHealth += 20.0f;
            else if (ArtifactData->Grade == "Epic") ArtifactEffect_MaxHealth += 30.0f;
            else if (ArtifactData->Grade == "Unique") ArtifactEffect_MaxHealth += 40.0f;
            ArtifactEffect.HP_UP = true;
        }
        else if (ArtifactData->Name_ == "Rage Helm")
        {
            if (ArtifactData->Grade == "Common") ArtifactEffect_Damage += 0.05f;
            else if (ArtifactData->Grade == "Rare") ArtifactEffect_Damage += 0.1f;
            else if (ArtifactData->Grade == "Epic") ArtifactEffect_Damage += 0.15f;
            else if (ArtifactData->Grade == "Unique") ArtifactEffect_Damage += 0.2f;
            ArtifactEffect.Damage_UP = true;
        }
        else if (ArtifactData->Name_ == "Gem Of ATK")
        {
            if (ArtifactData->Grade == "Common") ArtifactEffect_Stat_Damage += 1.0f;
            else if (ArtifactData->Grade == "Rare") ArtifactEffect_Stat_Damage += 2.0f;
            else if (ArtifactData->Grade == "Epic") ArtifactEffect_Stat_Damage += 3.0f;
            else if (ArtifactData->Grade == "Unique") ArtifactEffect_Stat_Damage += 4.0f;
            ArtifactEffect.Stat_UP_Atk = true;
        }
        else if (ArtifactData->Name_ == "Gem Of HP")
        {
            if (ArtifactData->Grade == "Common") ArtifactEffect_Stat_HP += 1.0f;
            else if (ArtifactData->Grade == "Rare") ArtifactEffect_Stat_HP += 2.0f;
            else if (ArtifactData->Grade == "Epic") ArtifactEffect_Stat_HP += 3.0f;
            else if (ArtifactData->Grade == "Unique") ArtifactEffect_Stat_HP += 4.0f;
            ArtifactEffect.Stat_UP_HP = true;
        }
        else if (ArtifactData->Name_ == "Gem Of LUK")
        {
            if (ArtifactData->Grade == "Common") ArtifactEffect_Stat_Luck += 1.0f;
            else if (ArtifactData->Grade == "Rare") ArtifactEffect_Stat_Luck += 2.0f;
            else if (ArtifactData->Grade == "Epic") ArtifactEffect_Stat_Luck += 3.0f;
            else if (ArtifactData->Grade == "Unique") ArtifactEffect_Stat_Luck += 4.0f;
            ArtifactEffect.Stat_UP_Luck = true;
        }
        else if (ArtifactData->Name_ == "Ruined Crown")
        {
            if (ArtifactData->Grade == "Common") ArtifactEffect_Recovery += 5.0;
            else if (ArtifactData->Grade == "Rare") ArtifactEffect_Recovery += 10.0;
            else if (ArtifactData->Grade == "Epic") ArtifactEffect_Recovery += 15.0;
            else if (ArtifactData->Grade == "Unique") ArtifactEffect_Recovery += 20.0;
            ArtifactEffect.Recovery = true;
        }
        UE_LOG(LogTemp, Warning, TEXT("Get Artifact!!!!!!  %s"), *ArtifactData->Name_);
    }

    ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    PlayerCharacter->SetUpdateStat();
}

void ULoglikeGameInstance::ResetArtifactEffect()
{
    ArtifactEffect.HP_UP = false;
    ArtifactEffect.Damage_UP = false;
    ArtifactEffect.Stat_UP_Atk = false;
    ArtifactEffect.Stat_UP_HP = false;
    ArtifactEffect.Stat_UP_Luck = false;
    ArtifactEffect.Recovery = false;

    ArtifactEffect_MaxHealth = 0.0;
    ArtifactEffect_Damage = 0.0;
    ArtifactEffect_Stat_HP = 0.0;
    ArtifactEffect_Stat_Damage = 0.0;
    ArtifactEffect_Stat_Luck = 0.0;
    ArtifactEffect_Recovery = 0.0;
}