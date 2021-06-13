// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface_ItemEffect.h"


#include "BaseItemEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class CYPERARENA_API UBaseItemEffect : public UObject, public IInterface_ItemEffect
{
	GENERATED_BODY()
public :
	UBaseItemEffect();

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemEffect")
		float value;

};
