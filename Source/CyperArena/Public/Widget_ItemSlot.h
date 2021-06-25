// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UWidget_ItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UPROPERTY(Meta = (BindWidget))
		class UImage* slot_image;
	UPROPERTY(Meta = (BindWidget))
		class UTextBlock* count_text;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
		FName item_id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
		int32 my_index;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemData")
		int32 count;


	UFUNCTION(BlueprintCallable, Category = "itemslot")
		void dropFromItemSlot(UWidget_ItemSlot* from);
};
