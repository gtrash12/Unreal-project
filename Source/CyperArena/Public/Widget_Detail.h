// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Detail.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UWidget_Detail : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
		class UImage* image;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
		class UTextBlock* name_text;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
		class UTextBlock* info_text;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
		class UTextBlock* effect_text;

	UFUNCTION(BlueprintCallable, Category = "itemslot")
		void initDetail(FName __item_id);
	UFUNCTION(BlueprintCallable, Category = "itemslot")
		void onViewPortCheck();
};
