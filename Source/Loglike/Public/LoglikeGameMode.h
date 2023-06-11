// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoglikeGameMode.generated.h"

UCLASS(minimalapi)
class ALoglikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoglikeGameMode();
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	class AABGameState* ABGameState;
};



