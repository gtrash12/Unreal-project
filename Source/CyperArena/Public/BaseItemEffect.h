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
	~UBaseItemEffect();
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemEffect")
		float value;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemEffect")
		FName item_id;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-ItemEffect")
	//	void applyItemEffect(ACharacter* causor, int32 __inven_index);
	//	virtual void applyItemEffect_Implementation(ACharacter* causor, int32 __inven_index);
	///*UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-ItemEffect")
	//	FText describeItemEffect();
	//	virtual FText describeItemEffect_Implementation();*/
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-ItemEffect")
	//	void onRegistration(ACharacter* causor, int32 __inven_index);
	//	virtual void onRegistration_Implementation(ACharacter* causor, int32 __inven_index);
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-ItemEffect")
	//	void onRemoveRegistration(ACharacter* causor, int32 __inven_index);
	//	virtual void onRemoveRegistration_Implementation(ACharacter* causor, int32 __inven_index);
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-ItemEffect")
	//	void onActivate(ACharacter* causor, int32 __inven_index);
	//	virtual void onActivate_Implementation(ACharacter* causor, int32 __inven_index);
};
