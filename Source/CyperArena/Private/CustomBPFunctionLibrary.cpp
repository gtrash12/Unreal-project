// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBPFunctionLibrary.h"

int32 UCustomBPFunctionLibrary::findEmptyIndex(TMap<int32, FInventoryData> __inven) {
	TArray<int32> keys;
	__inven.GetKeys(keys);
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

int32 UCustomBPFunctionLibrary::findSameItem(TMap<int32, FInventoryData> __inven, FName __item_id){
	TArray<FInventoryData> values;
	TArray<int32> keys;
	__inven.GenerateValueArray(values);
	__inven.GetKeys(keys);
	int nums = values.Num();
	for (int i = 0; i < nums; i++) {
		if (values[i].item_id == __item_id) {
			return keys[i];
		}
	}
	return -1;
	//__controller->GetGameInstance()->
	
}