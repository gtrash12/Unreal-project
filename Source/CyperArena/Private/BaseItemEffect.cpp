// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItemEffect.h"
//#include "Kismet/KismetSystemLibrary.h"

UBaseItemEffect::UBaseItemEffect() {
	UE_LOG(LogTemp, Warning, TEXT("생성"));
}

UBaseItemEffect::~UBaseItemEffect() {
	UE_LOG(LogTemp, Warning, TEXT("삭제"));
}

//FText UBaseItemEffect::describeItemEffect_Implementation() {}
void UBaseItemEffect::applyItemEffect_Implementation(ACharacter* causor, int32 __inven_index) {}
void UBaseItemEffect::onRegistration_Implementation(ACharacter* causor, int32 __inven_index) {}
void UBaseItemEffect::onRemoveRegistration_Implementation(ACharacter* causor, int32 __inven_index) {}
void UBaseItemEffect::onActivate_Implementation(ACharacter* causor, int32 __inven_index) {}