// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include <NiagaraEffectType.h>
#include "Engine/DataTable.h"
#include "Weapon.generated.h"


/** Please add a struct description */
USTRUCT(BlueprintType)
struct FWeapon:public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Name_"))
		FString Name_;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Image", MakeStructureDefaultValue = "None"))
		TObjectPtr<UTexture2D> Image;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Grade"))
		FString Grade;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Static Mesh", MakeStructureDefaultValue = "None"))
		TObjectPtr<UStaticMesh> StaticMesh;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Effect"))
		FString Effect;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Color", MakeStructureDefaultValue = "(SpecifiedColor=(R=1.000000,G=0.000000,B=1.000000,A=1.000000),ColorUseRule=UseColor_Specified)"))
		FSlateColor Color;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FX", MakeStructureDefaultValue = "None"))
		TObjectPtr<UNiagaraComponent> FX;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AtkPower", MakeStructureDefaultValue = "0"))
		int32 AtkPower;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AtkSpeed"))
		int32 AtkSpeed;
};
