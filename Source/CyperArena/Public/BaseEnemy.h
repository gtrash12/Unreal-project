// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter.h"
//#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"
#include "Interface_AI.h"

#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CYPERARENA_API ABaseEnemy : public ABaseCharacter, public IInterface_AI
{
	GENERATED_BODY()

public :
	ABaseEnemy();

	//캐릭터 결정부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		FName name;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "BaseEnemy")
		class UBehaviorTree* behavior_tree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		TArray<UAnimMontage*> attack_montages;

	// 내부동작 필요변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		UWidgetComponent* state_bar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		FVector look_location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseEnemy")
		float down_time;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "BaseEnemy")
		AActor* target_actor;


protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void setLookRotation_Implementation() override;

	virtual void applyDamage_Multicast_Exec_Implementation(FName __target_damage_id, AActor* damage_causer, FName __hit_bone_name) override;
	virtual void getTargetRotation_Implementation(FRotator& __target_rotation) override;
	virtual void getAttackTraceChannel_Implementation(TEnumAsByte<ETraceTypeQuery>& __attack_trace_channel) override;
	virtual void setCharacterState_Implementation(ECharacterState target_character_state)override;

	// 인터페이스 메소드 선언
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		void setLookLocation(FVector __target_look_location);
		virtual void setLookLocation_Implementation(FVector __target_look_location);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		void setTarget(AActor* __target);
		virtual void setTarget_Implementation(AActor* __target);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		AActor* getTarget();
		virtual AActor* getTarget_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		void setLockOnMarker(bool __is_lock_on);
		virtual void setLockOnMarker_Implementation(bool __is_lock_on);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		void setStateBarVisibility(bool __visibility);
		virtual void setStateBarVisibility_Implementation(bool __visibility);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		void setSprint(bool __is_sprint);
		virtual void setSprint_Implementation(bool __is_sprint);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		void attack();
		virtual void attack_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface-Enemy")
		UBehaviorTree* getBehaviorTree();
		virtual UBehaviorTree* getBehaviorTree_Implementation();

	// 클래스 메소드 선언
	UFUNCTION(BlueprintCallable, NetMulticast, UnReliable)
		void Multicast_setSprint(bool __is_sprint);
		virtual void Multicast_setSprint_Implementation(bool __is_sprint);

	UFUNCTION(BlueprintCallable, NetMulticast, UnReliable)
		void Multicast_attack(FVector __last_movement_input, UAnimMontage* __animation);
		virtual void Multicast_attack_Implementation(FVector __last_movement_input, UAnimMontage* __animation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UAnimMontage* selectAttackMontage();
		virtual UAnimMontage* selectAttackMontage_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void death();
		virtual void death_Implementation();
};
