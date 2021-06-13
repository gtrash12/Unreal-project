// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UCustomBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable, Category = "Array")
		static int32 findEmptyIndex(TArray<int32> keys);
};
