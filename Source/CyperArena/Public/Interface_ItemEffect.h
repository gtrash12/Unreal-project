// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_ItemEffect.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_ItemEffect : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYPERARENA_API IInterface_ItemEffect
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void applyItemEffect(AActor* causer);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		FText describeItemEffect();
};
