// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Detail.h"
#include "../CustomData.h"
#include "../Public/PWOGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Public/Interface_ItemEffect.h"
#include "Kismet/KismetSystemLibrary.h"

void UWidget_Detail::initDetail(FName __item_id) {
	FItemData itemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	name_text->SetText(itemdata.name);
	image->SetBrushFromTexture(itemdata.icon);
	info_text->SetText(itemdata.item_info_text);
	if (itemdata.item_effect_list.Num() > 0) {
		FString final_effect_text;
		for (FItemEffect i : itemdata.item_effect_list) {
			auto item_effect_obj = i.item_effect.GetDefaultObject();
			item_effect_obj->value = i.value;
			if (i.item_effect->ImplementsInterface(UInterface_ItemEffect::StaticClass())) {
				final_effect_text += TEXT("\n");
				final_effect_text += IInterface_ItemEffect::Execute_describeItemEffect(item_effect_obj).ToString();
			}
		}
		effect_text->SetText(FText::FromString(final_effect_text.TrimStartAndEnd()));
		effect_text->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		effect_text->SetVisibility(ESlateVisibility::Collapsed);
	}
}