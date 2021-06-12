// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemEffect.h"
#include "ItemEffect_Heal.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UItemEffect_Heal : public UBaseItemEffect
{
	GENERATED_BODY()
public :

	virtual void applyItemEffect_Implementation(AActor* causer) override;
	virtual void describeItemEffect_Implementation(FText& __desc_text) override;
};
