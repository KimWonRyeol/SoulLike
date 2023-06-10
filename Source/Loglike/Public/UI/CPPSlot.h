// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include <Engine/DataTable.h>
#include "CPPSlot.generated.h"

/**
 * 
 * */




/** Please add a struct description */
USTRUCT(BlueprintType)
struct FCPPSlot
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Item"))
		FDataTableRowHandle Item;
};
