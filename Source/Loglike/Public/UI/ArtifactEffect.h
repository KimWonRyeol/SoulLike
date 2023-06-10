// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "ArtifactEffect.generated.h"

/**
 * 
 */
USTRUCT()
struct FArtifactEffect
{
	GENERATED_BODY()

public:
	bool HP_UP;
	bool Damage_UP;
	bool Stat_UP_Atk;
	bool Stat_UP_HP;
	bool Stat_UP_Luck;
	bool Recovery;
};