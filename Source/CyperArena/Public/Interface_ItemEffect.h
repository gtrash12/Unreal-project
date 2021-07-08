// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CustomEnums.h"

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
	/* 아이템 효과 적용시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void applyItemEffect(ACharacter* causor, int32 __inven_index);
	/* 아이템 설명 텍스트 요구시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		FText describeItemEffect();
	/* 아이템을 퀵슬롯이나 장비 슬롯에 등록시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onRegistration(ACharacter* causor, int32 __inven_index);
	/* 아이템을 퀵슬롯이나 장비 슬롯에서 해제시 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onRemoveRegistration(ACharacter* causor, int32 __inven_index);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onEquip(ACharacter* causor, EEquipmentType __key);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onUnequip(ACharacter* causor, EEquipmentType __key);
	/* 아이템 사용시 (인벤토리에서 우클릭하거나 퀵슬롯에 등록 후 퀵슬롯 키 입력시 ) */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface-ItemEffect")
		void onActivate(ACharacter* causor, int32 __inven_index);
};
