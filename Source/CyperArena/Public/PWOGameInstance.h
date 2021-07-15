// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../CustomData.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Widget_Detail.h"
#include "Widget_ItemSlot.h"
#include "Engine/NiagaraSystem.h"

#include "PWOGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CYPERARENA_API UPWOGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPWOGameInstance();

	UPROPERTY(BlueprintReadOnly, Category = "DataTable")
		UDataTable* damage_data_table;
	UPROPERTY(BlueprintReadOnly, Category = "DataTable")
		UDataTable* action_data_table;
	UPROPERTY(BlueprintReadOnly, Category = "DataTable")
		UDataTable* item_data_table;
	UPROPERTY(BlueprintReadOnly, Category = "DataTable")
		UDataTable* item_mesh_data_table;
	UPROPERTY()
		UTexture2D* empty_slot_image;
	UPROPERTY(BlueprintReadWrite, Category = "Base-Look")
		UWidget_Detail* detail_widget_reference;
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
		TMap<FKey, UWidget_ItemSlot*> quickslot_references;
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
		TMap<int32, UWidget_ItemSlot*> inventory_slot_reference;
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite)
		TMap<EEquipmentType, UWidget_ItemSlot*> equipment_slot_reference;
	UPROPERTY()
		UNiagaraSystem* blood_effect;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameInstance-DataTableAccess")
		void findDamageData(FName __damage_id, FdamageData& __output_damage_data);
		virtual void findDamageData_Implementation(FName __damage_id, FdamageData& __output_damage_data);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameInstance-DataTableAccess")
		void findActionData(FName __action_id, FActionData& __output_action_data);
		virtual void findActionData_Implementation(FName __action_id, FActionData& __output_action_data);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameInstance-DataTableAccess")
		FItemData findItemData(FName __item_id);
		virtual FItemData findItemData_Implementation(FName __item_id);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GameInstance-Interaction")
		void Interact(AActor* __target, AActor* __causer);
		virtual void Interact_Implementation(AActor* __target, AActor* __causer);

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
		bool isStackable(EItemType __item_type);

	UFUNCTION(BlueprintCallable, Category = "Class")
		UBaseItemEffect* getClassDefaultObject(TSubclassOf<UBaseItemEffect> __class);
};

