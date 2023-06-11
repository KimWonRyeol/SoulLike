// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "UI/CPPSlot.h"
#include "UI/Weapon.h"
#include "UI/Artifact.h"
#include "UI/WeaponEffect.h"
#include "UI/ArtifactEffect.h"
#include "LoglikeGameInstance.generated.h"




USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterData() : Name(1), DefaultHealth(100.0f), DefaultDamage(7.0f), DefaultLuck(0.0f), DefaultAttackSpeed(0.0f), Token(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DefaultHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DefaultDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DefaultLuck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float DefaultAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Token;

};
/**
 * 
 */
enum class EStageType : uint8;

UCLASS()
class LOGLIKE_API ULoglikeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	ULoglikeGameInstance();
	virtual void Init() override;

	TArray<int32> GetSelectedNodeArr();
	void AddSelectedNode(int32 NodeNum);
	/**Execute to initialize the stage contents in the lobby*/
	void InitDungeonMode();

	//FABCharacterData* GetABCharacterData(int32 Name);

	void SetMaxHealth(float NewMaxHealth);
	void SetCurrentHealth(float NewCurrentHealth);
	void SetAddHealth(float NewAddHealth);
	void SetAddDamage(float NewAddDamage);
	void SetAddLuck(float NewAddLuck);
	void SetAddAttackSpeed(float NewAddAttackSpeed);
	void SetAddToken(int32 NewAddToken);
	void SetTotalAddToken(int32 NewTotalAddToken);

	float GetAddHealth();
	float GetCurrentHealth();
	float GetAddDamage();
	float GetAddLuck();
	float GetAddAttackSpeed();
	int32 GetAddToken();
	int32 GetTotalAddToken();
	FWeapon* GetWeapon();
	TArray<FArtifact*> GetArtifact();

	float GetWeaponDamage();

	void SetTrueWeaponKey();
	void SetTrueArtifactKey();
	//Inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory Data")
		FCPPSlot GetWeaponData();

	UFUNCTION(BlueprintCallable, Category = "Inventory Data")
		void SetWeaponData(FCPPSlot CurWeaponData);

	void ApplyWeaponEffect(FString weaponName);

	FWeaponEffect GetWeaponEffect();

	FArtifactEffect GetArtifactEffect();

	void ResetArtifactEffect();

	UFUNCTION(BlueprintCallable, Category = "Inventory Data")
		TArray<FCPPSlot> GetArtifactData();

	UFUNCTION(BlueprintCallable, Category = "Inventory Data")
		void SetArtifactData(TArray<FCPPSlot> CurWeaponData);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EStageType CurrentStageType;
	

	bool WeaponKey = false;
	bool ArtifactKey = false;
	int32 WeaponKeyRemain = 1;
	int32 ArtifactKeyRemain = 1;
	int32 HpStatRemain = 10;
	int32 DamageStatRemain = 10;
	int32 LuckStatRemain = 10;

	float ArtifactEffect_MaxHealth = 0.0;
	float ArtifactEffect_Damage = 0.0;
	float ArtifactEffect_Stat_HP = 0.0;
	float ArtifactEffect_Stat_Damage = 0.0;
	float ArtifactEffect_Stat_Luck = 0.0;
	float ArtifactEffect_Recovery = 0.0;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHealth;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddHealth;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddLuck;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddAttackSpeed;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 AddToken; //�� �������� ������ ���� ��ū��

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 TotalAddToken; //�̹� �ǿ� ���� �� ��ū ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		TArray<FCPPSlot> ArtifactSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Data")
		FCPPSlot WeaponSlot;


private:
	FWeapon* WeaponData;
	TArray<FArtifact*> ArtifactDatas;

	TArray<int32> SelectedNodeArr;
	
	UPROPERTY()
	class UDataTable* ABCharacterTable;

	UPROPERTY(VIsibleAnywhere, Category = "Stat")
	class UABCharacterStatComponent* CharacterStat;

public:
	TArray<TPair<int32,int32>> ClearStageArray;
	//무기 공격력
	float WeaponDamage;
	struct FWeaponEffect WeaponEffect;
	struct FArtifactEffect ArtifactEffect;
};
