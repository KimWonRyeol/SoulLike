// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "WeaponEffect.generated.h"

/**
 * 
 */
USTRUCT()
struct FWeaponEffect
{
	GENERATED_BODY()

public:
	bool ParryingAtk;
	bool FinalCombo;
	bool ParryingDuration;
	bool Leech;
	bool GlassCannon;
};
