// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ItemSlot.h"
#include "../Public/Interface_PlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Public/PWOGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

//FEventReply UWidget_ItemSlot::OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
//{
//	
//}
//
//void UWidget_ItemSlot::OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
//{
//
//}

void UWidget_ItemSlot::initSlot() {
	FInventoryData invendata;
	if(GetWorld()->GetFirstPlayerController() == NULL)
		return;
	if (GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass())) {
		invendata = IInterface_PlayerController::Execute_getInventoryData(GetWorld()->GetFirstPlayerController(), my_index);
	}
	item_id = invendata.item_id;
	count = invendata.count;
	if (invendata.item_id == "None") {
		slot_image->SetBrushFromTexture(Cast<UPWOGameInstance>(GetGameInstance())->empty_slot_image);
		count_text->SetVisibility(ESlateVisibility::Collapsed);
		//
	}
	else {
		FItemData itemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(item_id);
		slot_image->SetBrushFromTexture(itemdata.icon);
		if (Cast<UPWOGameInstance>(GetGameInstance())->isStackable(itemdata.item_type)) {
			count_text->SetText(FText::FromString(FString::FromInt(count)));
			count_text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else {
			count_text->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}

void UWidget_ItemSlot::dropFromItemSlot(UWidget_ItemSlot* from) {
	if (from->my_index == my_index)
		return;
	if (GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UInterface_PlayerController::StaticClass())) {
		IInterface_PlayerController::Execute_swapInvenSlot(GetWorld()->GetFirstPlayerController(), from->my_index, my_index);
	}
	//from->init

}