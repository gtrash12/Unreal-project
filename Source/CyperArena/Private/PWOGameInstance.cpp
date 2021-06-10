// Fill out your copyright notice in the Description page of Project Settings.


#include "PWOGameInstance.h"

UPWOGameInstance::UPWOGameInstance() {
	static ConstructorHelpers::FObjectFinder<UDataTable> damage_data_table_cst(TEXT("DataTable'/Game/DataTable/DamageDataTable.DamageDataTable'"));
	if (damage_data_table_cst.Succeeded())
		damage_data_table = damage_data_table_cst.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> action_data_table_cst(TEXT("DataTable'/Game/DataTable/ActionDataTable.ActionDataTable'"));
	if (action_data_table_cst.Succeeded())
		action_data_table = action_data_table_cst.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> item_data_table_cst(TEXT("DataTable'/Game/DataTable/ItemDataTable.ItemDataTable'"));
	if (item_data_table_cst.Succeeded())
		item_data_table = item_data_table_cst.Object;
}


void UPWOGameInstance::findDamageData_Implementation(FName __damage_id, FdamageData& __output_damage_data) {
	__output_damage_data = *damage_data_table->FindRow<FdamageData>(__damage_id, TEXT(""));
}

void UPWOGameInstance::findActionData_Implementation(FName __action_id, FActionData& __output_action_data) {
	__output_action_data = *action_data_table->FindRow<FActionData>(__action_id, TEXT(""));
}