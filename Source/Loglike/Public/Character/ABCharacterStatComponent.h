// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOGLIKE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//void SetNewLevel(int NewLevel);
	//void SetStartName(int32 NewName);
	void SetMaxHealth(float NewMaxHealth);
	void SetCurrentHealth(float NewCurrentHealth);
	void SetAddHealth(float NewAddHealth);
	void SetDefaultDamage(float NewDefaultDamage);
	void SetAddDamage(float NewAddDamage);
	void SetDefaultLuck(float NewDefaultLuck);
	void SetAddLuck(float NewAddLuck);
	void SetDefaultAttackSpeed(float NewDefaultAttackSpeed);
	void SetAddAttackSpeed(float NewAddAttackSpeed);
	void SetDefaultToken(int32 NewDefaultToken);
	void SetAddToken(int32 NewAddToken);
	void AddLuck_(int32 amount);
	void AddDamage_(int32 amount);
	void AddHP_(int32 amount);
	void AddToken_(int32 NewAddToken);
	void SetWeaponKey(bool _WeaponKey);
	void SetArtifactKey(bool _ArtifactKey);
	bool GetWeaponKey();
	bool GetArtifactKey();
	float GetMaxHealth();
	float GetCurrentHealth();
	float GetDefaultDamage();
	float GetDefaultLuck();
	float GetDefaultAttackSpeed();
	int32 GetToken();
	void SetWeaponKeyRemain(int32 WeaponKeyRemain_);
	void SetArtifactKeyRemain(int32 ArtifactKeyRemain_);
	void SetHpStatRemain(int32 HpStatRemain_);
	void SetDamageStatRemain(int32 DamageStatRemain_);
	void SetLuckStatRemain(int32 LuckStatRemain_);
	int32 GetWeaponKeyRemain();
	int32 GetArtifactKeyRemain();
	int32 GetHpStatRemain();
	int32 GetDamageStatRemain();
	int32 GetLuckStatRemain();
	FOnHPIsZeroDelegate OnHPIsZero;
		
private:
	struct FABCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Name;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHealth; // == DefaultHealth

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddHealth;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefaultDamage;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddDamage; //삭제할지도...

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefaultLuck;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddLuck; //삭제할지도...

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float DefaultAttackSpeed;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AddAttackSpeed; //삭제할지도...

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Token;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		bool ArtifactKey;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		bool WeaponKey;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 WeaponKeyRemain;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 ArtifactKeyRemain;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 HpStatRemain;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 DamageStatRemain;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 LuckStatRemain;

};
