#include "Props/Stat_Statue.h"
#include "GameFramework/LoglikeGameInstance.h"
#include "Engine/GameInstance.h"
#include "Character/LoglikeCharacter.h"
#include "Character/ABCharacterStatComponent.h"
#include <Kismet/GameplayStatics.h>
#include <DungeonGameMode.h>
//#include <LoglikeGameMode.cpp>


// Sets default values
AStat_Statue::AStat_Statue()
{
	
}

void AStat_Statue::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(false);
	ULoglikeGameInstance* GameInstance = Cast<ULoglikeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (int8(GameInstance->CurrentStageType) != 3)
	{
		Destroy();
		return;
	}


	ADungeonGameMode* GameMode = Cast<ADungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->OnStageClear.AddDynamic(this, &AStat_Statue::SetCollisionOn);
}

void AStat_Statue::SetCollisionOn()
{
	SetActorEnableCollision(true);
}


void AStat_Statue::Stat_UP()
{
	auto ABGameInstance = Cast<ULoglikeGameInstance>(GetGameInstance());
	ALoglikeCharacter* PlayerCharacter = Cast<ALoglikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	int num = FMath::RandRange(0, 2);
	switch (num)
	{
	case 0:
		ABGameInstance->SetAddHealth(3);
		break;
	case 1:
		ABGameInstance->SetAddDamage(3);
		break;
	case 2:
		ABGameInstance->SetAddLuck(3);
		break;
	}
	
	PlayerCharacter->SetUpdateStat();
}



