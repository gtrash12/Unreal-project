// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../CustomData.h"
#include "BaseItemEffect.h"

#include "CustomBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UCustomBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable, Category = "Map")
		static int32 findEmptyIndex(TMap<int32, FInventoryData> __inven);
	UFUNCTION(BlueprintCallable, Category = "Map")
		static int32 findSameItem(TMap<int32, FInventoryData> __inven, FName __item_id);
	UFUNCTION(BlueprintCallable, Category = "Class")
		static UBaseItemEffect* getClassDefaultObject(TSubclassOf<UBaseItemEffect> __class);
};
