// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Detail.h"
#include "../CustomData.h"
#include "../Public/PWOGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Public/Interface_ItemEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"

void UWidget_Detail::initDetail(FName __item_id) {
	FItemData itemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	name_text->SetText(itemdata.name);
	//UE_LOG(LogTemp, Warning, FString::Printf(TEXT("%d"),itemdata.item_rank));
	switch (itemdata.item_rank)
	{
	case EItemRank::Common:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		break;
	case EItemRank::Rare:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));
		break;
	case EItemRank::Precious:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::Purple)));
		break;
	case EItemRank::Unique:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		break;
	case EItemRank::Legendary:
		name_text->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::Orange)));
		break;
	default:
		break;
	}
	image->SetBrushFromTexture(itemdata.icon);
	info_text->SetText(itemdata.item_info_text);
	if (itemdata.item_effect_list.Num() > 0) {
		FString final_effect_text;
		for (FItemEffect i : itemdata.item_effect_list) {
			auto item_effect_obj = i.item_effect.GetDefaultObject();
			item_effect_obj->value = i.value;
			if (i.item_effect->ImplementsInterface(UInterface_ItemEffect::StaticClass())) {
				FString cur_text = IInterface_ItemEffect::Execute_describeItemEffect(item_effect_obj).ToString();
				if (cur_text == "")
					continue;
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
	//onViewPortCheck();
}

void UWidget_Detail::onViewPortCheck()
{
	FVector2D viewport_size;
	GetWorld()->GetGameViewport()->GetViewportSize(viewport_size);
	FGeometry geometry = GetTickSpaceGeometry();
	FVector2D abs_to_local_vector = geometry.GetAbsoluteSize() / geometry.GetLocalSize();
	FVector2D target_position = geometry.Position * abs_to_local_vector;
	bool flag = false;
	if (geometry.GetAbsoluteSize().X + target_position.X > viewport_size.X) {
		target_position.X -= 85 * abs_to_local_vector.X + geometry.GetAbsoluteSize().X;
		flag = true;
	}
	if (geometry.GetAbsoluteSize().Y + target_position.Y > viewport_size.Y) {
		target_position.Y -= 85 * abs_to_local_vector.Y + geometry.GetAbsoluteSize().Y;
		flag = true;
	}
	if (flag)
		SetPositionInViewport(target_position);
	//if (IsValid(this) == false && GetWorld() == nullptr)
	//	return;
	//FTimerManager asd;
	//asd.SetTimerForNextTick(FTimerDelegate::CreateLambda([&]() {
	//	//지오메트리가 업데이트 될 때 까지 busy wait
	//	if (GetTickSpaceGeometry().GetLocalSize().X == 0) { 
	//		onViewPortCheck();
	//	}
	//	else {
	//		FVector2D viewport_size;
	//		GetWorld()->GetGameViewport()->GetViewportSize(viewport_size);
	//		FGeometry geometry = GetTickSpaceGeometry();
	//		FVector2D abs_to_local_vector = geometry.GetAbsoluteSize() / geometry.GetLocalSize();
	//		FVector2D target_position = geometry.Position * abs_to_local_vector;
	//		bool flag = false;
	//		if (geometry.GetAbsoluteSize().X + target_position.X > viewport_size.X) {
	//			target_position.X -= 85*abs_to_local_vector.X +  geometry.GetAbsoluteSize().X;
	//			flag = true;
	//		}
	//		if (geometry.GetAbsoluteSize().Y + target_position.Y > viewport_size.Y) {
	//			target_position.Y -= 85 * abs_to_local_vector.Y + geometry.GetAbsoluteSize().Y;
	//			flag = true;
	//		}
	//		if(flag)
	//			SetPositionInViewport(target_position);
	//		
	//	}
	//	}));
}