// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItemEffect.h"
//#include "Kismet/KismetSystemLibrary.h"

UBaseItemEffect::UBaseItemEffect() {
	UE_LOG(LogTemp, Warning, TEXT("생성"));
}

UBaseItemEffect::~UBaseItemEffect() {
	UE_LOG(LogTemp, Warning, TEXT("삭제"));
}