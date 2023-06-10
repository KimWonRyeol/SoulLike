// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterGenerator.generated.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	E_Zombie = 0 UMETA(DisplayName = "Zombie"),
	E_Goblin = 1 UMETA(DisplayName = "Goblin"),
	E_Skeleton = 2 UMETA(DisplayName = "Skeleton"),
	E_Lich = 3 UMETA(DisplayName = "Lich"),
	E_Ghoul = 4 UMETA(DisplayName = "Ghoul"),
};

USTRUCT(Atomic, BlueprintType)
struct FMonsterGroup
{
    GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMonsterType MonsterType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MonsterNum;
};

USTRUCT(Atomic, BlueprintType)
struct FMonsterGroups
{
    GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMonsterGroup> MonsterGruop;
};

UCLASS()
class LOGLIKE_API AMonsterGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterGenerator();

protected:
	virtual void BeginPlay() override;
public:
	int32 GenMonsters(int8 MonsterGroupIndex);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Monster", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* FocusStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Monster", Meta = (AllowPrivateAccess = true))
	TArray<class UStaticMeshComponent*> MonsterLocationArr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Monster", Meta = (AllowPrivateAccess = true))
	TArray<FMonsterGroups> MonsterGroups;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Monster", Meta = (AllowPrivateAccess = true))
	TArray<UBlueprint*> MonsterBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "MonsterClass", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AGhoul> GhoulClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "MonsterClass", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class ALich> LichClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "MonsterClass", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AGoblin> GoblinClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "MonsterClass", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AZombie> ZombieClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "MonsterClass", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AMeleeSkeleton> MeleeSkeletonClass;

};
