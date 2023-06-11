// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MonsterGenerator.h"
#include "Enemy/Zombie.h"
#include "Enemy/Goblin.h"
#include "Enemy/Ghoul.h"
#include "Enemy/Lich.h"
#include "Enemy/MeleeSkeleton.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AMonsterGenerator::AMonsterGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FocusStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FocusMesh"));
	RootComponent = FocusStaticMesh;
	for (int8 i = 1; i <= 8; i++)
	{
		FString ComponentName = FString::Printf(TEXT("MonsterLocation %d"), i);
		MonsterLocationArr.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*ComponentName)));
		MonsterLocationArr[MonsterLocationArr.Num() - 1]->SetupAttachment(RootComponent);
		MonsterLocationArr[MonsterLocationArr.Num() - 1]->SetWorldRotation(MonsterLocationArr[0]->GetComponentRotation());
	}

	FMonsterGroups Groups1;
	FMonsterGroup Group;
	Group.MonsterNum = 8;
	Group.MonsterType = EMonsterType::E_Zombie;
	Groups1.MonsterGruop.Add(Group);
	MonsterGroups.Add(Groups1);
	
	FMonsterGroups Groups2;

	Group.MonsterNum = 4;
	Group.MonsterType = EMonsterType::E_Goblin;
	Groups2.MonsterGruop.Add(Group);
	Group.MonsterNum = 2;
	Group.MonsterType = EMonsterType::E_Skeleton;
	Groups2.MonsterGruop.Add(Group);
	MonsterGroups.Add(Groups2);

	FMonsterGroups Groups3;
	Group.MonsterNum = 4;
	Group.MonsterType = EMonsterType::E_Zombie;
	Groups3.MonsterGruop.Add(Group);
	Group.MonsterNum = 1;
	Group.MonsterType = EMonsterType::E_Lich;
	Groups3.MonsterGruop.Add(Group);
	MonsterGroups.Add(Groups3);

	FMonsterGroups Groups4;
	Group.MonsterNum = 2;
	Group.MonsterType = EMonsterType::E_Zombie;
	Groups4.MonsterGruop.Add(Group);
	Group.MonsterNum = 1;
	Group.MonsterType = EMonsterType::E_Ghoul;
	Groups4.MonsterGruop.Add(Group);
	Group.MonsterNum = 2;
	Group.MonsterType = EMonsterType::E_Lich;
	Groups4.MonsterGruop.Add(Group);
	MonsterGroups.Add(Groups4);

	FMonsterGroups Groups5;
	Group.MonsterNum = 3;
	Group.MonsterType = EMonsterType::E_Lich;
	Groups5.MonsterGruop.Add(Group);
	MonsterGroups.Add(Groups5);
}

void AMonsterGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (auto MonLoc : MonsterLocationArr)
	{
		MonLoc->SetVisibility(false);
		FRotator FocusRot = UKismetMathLibrary::FindLookAtRotation(MonLoc->GetComponentLocation(), FocusStaticMesh->GetComponentLocation());
		MonLoc->SetRelativeRotation(FocusRot);
	}
	FocusStaticMesh->DestroyComponent();
}

int32 AMonsterGenerator::GenMonsters(int8 MonsterGroupIndex)
{
	int8 MonsterNum = 0;
	for (auto Group : MonsterGroups[MonsterGroupIndex-1].MonsterGruop)
	{
		for (int8 n = MonsterNum; n < MonsterNum + Group.MonsterNum; n++)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			FRotator SpawnRotator = MonsterLocationArr[n]->GetComponentRotation();
			FVector  SpawnLocation = MonsterLocationArr[n]->GetComponentLocation();

			/*switch (Group.MonsterType)
			{
			case EMonsterType::E_Ghoul:
				GetWorld()->SpawnActor<AGhoul>(MonsterBlueprint[0]->GeneratedClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Goblin:
				GetWorld()->SpawnActor<AGoblin>(MonsterBlueprint[1]->GeneratedClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Lich:
				GetWorld()->SpawnActor<ALich>(MonsterBlueprint[2]->GeneratedClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Skeleton:
				GetWorld()->SpawnActor<AMeleeSkeleton>(MonsterBlueprint[3]->GeneratedClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Zombie:
				GetWorld()->SpawnActor<AZombie>(MonsterBlueprint[4]->GeneratedClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			}*/
			
			switch (Group.MonsterType)
			{
			case EMonsterType::E_Ghoul:
				GetWorld()->SpawnActor<AGhoul>(GhoulClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Goblin:
				GetWorld()->SpawnActor<AGoblin>(GoblinClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Lich:
				GetWorld()->SpawnActor<ALich>(LichClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Skeleton:
				GetWorld()->SpawnActor<AMeleeSkeleton>(MeleeSkeletonClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			case EMonsterType::E_Zombie:
				GetWorld()->SpawnActor<AZombie>(ZombieClass, SpawnLocation, SpawnRotator, SpawnParams);
				break;
			}

		}
		MonsterNum += Group.MonsterNum;
	}
	for (auto GenLoc : MonsterLocationArr)
	{
		GenLoc->DestroyComponent();
	}
	return MonsterNum;
}
