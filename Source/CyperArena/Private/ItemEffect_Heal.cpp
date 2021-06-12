// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEffect_Heal.h"

void UItemEffect_Heal::applyItemEffect_Implementation(AActor* causer) {
	return;
}

void UItemEffect_Heal::describeItemEffect_Implementation(FText& __desc_text) {
	//__desc_text = FString::Printf(TEXT("체력을 160 회복합니다."));
	FText txt = FText::FromString("체력을 %d 회복합니다.");
	__desc_text = FText::Format(txt, value);
}