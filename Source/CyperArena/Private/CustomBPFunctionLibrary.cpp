// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBPFunctionLibrary.h"

int32 UCustomBPFunctionLibrary::findEmptyIndex(TArray<int32> keys) {
	keys.Sort();
	int32 min = 0;
	for (auto i : keys) {
		/*if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%d"), min));*/
		if (i != min)
			return min;
		else
			min += 1;
	}
	return min;
}