// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_General.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_General : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYPERARENA_API IInterface_General
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void interaction(AActor* causor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void popInteractionInfo(AActor* causor);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-Controller")
		void removeInteractionInfo(AActor* causor);
};
