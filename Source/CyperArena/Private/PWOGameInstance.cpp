// Fill out your copyright notice in the Description page of Project Settings.


#include "PWOGameInstance.h"
#include "../Public//Interface_General.h"

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

void UPWOGameInstance::findItemData_Implementation(FName __item_id, FItemData& __output_item_data){
	FItemData* res = item_data_table->FindRow<FItemData>(__item_id, TEXT(""));
	if(res)
		__output_item_data = *res;
}

void UPWOGameInstance::Interact_Implementation(AActor* __target, AActor* __causer) {
	//__target->
	if (__target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
		IInterface_General::Execute_Interact(__target, __causer);
	}
}