// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller_Player.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../Public/Interface_AI.h"
#include "../Public/Interface_General.h"
#include "../Public/Interface_BaseCharacter.h"
#include "PWOGameInstance.h"


AController_Player::AController_Player()
{

}

/// <summary>
/// 팔로우 카메라를 생성해서 viewtarget 대체
/// </summary>
void AController_Player::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController() == false)
		return;
	PlayerCameraManager->SetManualCameraFade(1, FLinearColor::Black, true);
	FTimerHandle timer_handle;
	GetWorld()->GetTimerManager().SetTimer(timer_handle, FTimerDelegate::CreateLambda([&]() {
		PlayerCameraManager->StartCameraFade(1, 0, 2, FLinearColor::Black, true);
		follow_cam = Cast<AFollowCam_Base>(GetWorld()->SpawnActor(AFollowCam_Base::StaticClass()));
		follow_cam->follow_target = GetPawn();
		SetViewTarget(follow_cam);
		}), 2, false);
}

/// <summary>
/// 락온 쿨타임을 매 프레임 감소시킴
/// 매 프레임 카메라 전방으로 linetrace 해서 interactable 객체를 찾아서 등록
/// </summary>
/// <param name="DeltaTime"></param>
void AController_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	lock_on_cooltime -= DeltaTime;

	/* 라인트레이스로 인터랙션 가능 물체 탐색 */
	if (IsLocalPlayerController() && follow_cam->IsValidLowLevel()) {
		FHitResult hit_result;
		FVector trace_start = GetPawn()->GetActorLocation();
		FVector trace_end = trace_start + follow_cam->camera->GetForwardVector()*(100);
		TArray<AActor*> dummy;
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), trace_start, trace_end, ETraceTypeQuery::TraceTypeQuery6, false, dummy, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit && hit_result.GetActor()->ActorHasTag("interactable")) {
			if (interaction_target != hit_result.GetActor()) {
				if (interaction_target != nullptr && interaction_target->IsValidLowLevel()) {
					if (interaction_target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
						IInterface_General::Execute_removeInteractionInfo(interaction_target, GetPawn());
					}
				}
				interaction_target = hit_result.GetActor();
				if (interaction_target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
					IInterface_General::Execute_popInteractionInfo(interaction_target, GetPawn());
				}
			}
		}
		else {
			if (interaction_target != nullptr && interaction_target->IsValidLowLevel()) {
				if (interaction_target->GetClass()->ImplementsInterface(UInterface_General::StaticClass())) {
					IInterface_General::Execute_removeInteractionInfo(interaction_target, GetPawn());
				}
			}
			interaction_target = nullptr;
		}
	}
}


/// <summary>
/// 키매핑
/// </summary>
void AController_Player::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent) {
		InputComponent->BindAction("LockOn", IE_Pressed, this, &AController_Player::lockOnEvent);
		InputComponent->BindAxis("ChangeLockOn", this, &AController_Player::changeLockOnAxisEvent);
	}
}

/// <summary>
/// 락온타게팅 실행 ( 액션 매핑 이벤트 )
/// </summary>
void AController_Player::lockOnEvent()
{
	if (follow_cam->IsValidLowLevel() == false)
		return;
	if (follow_cam->is_lock_on) {
		releaseLock_ON();
		return;
	}
	AActor* target = findLockOnTarget();
	if (target != nullptr && target->IsValidLowLevel()) {
		if (target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setStateBarVisibility(target, true);
			IInterface_AI::Execute_setLockOnMarker(target, true);
		}
		follow_cam->is_lock_on = true;
		is_lock_on = true;
		follow_cam->look_target = target;
	}
}


/// <summary>
/// 타게팅 전환 (축매핑 이벤트)
/// changeLockOnTarget 메소드로 타게팅이 전환될 액터를 구하고 이전 타겟의 마커를 화면에서 지우고 새로운 타겟의 마커를 띄움
/// </summary>
/// <param name="__axis_value"></param>
void AController_Player::changeLockOnAxisEvent(float __axis_value)
{
	if (__axis_value == 0 || lock_on_cooltime > 0 || follow_cam->IsValidLowLevel() == false || is_lock_on == false)
		return;
	AActor* target = changeLockOnTarget(__axis_value);
	if (target != nullptr && target->IsValidLowLevel()) {
		if (follow_cam->look_target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setLockOnMarker(follow_cam->look_target, false);
		}
		if (target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setStateBarVisibility(target, true);
			IInterface_AI::Execute_setLockOnMarker(target, true);
		}
		follow_cam->look_target = target;
		lock_on_cooltime = 0.2f;
	}
}

/// <summary>
/// 타게팅 해제
/// </summary>
void AController_Player::releaseLock_ON_Implementation()
{
	if (follow_cam->IsValidLowLevel() == false)
		return;
	follow_cam->is_lock_on = false;
	is_lock_on = false;
	if (follow_cam->look_target != nullptr && follow_cam->look_target->IsValidLowLevel()) {
		if (follow_cam->look_target->GetClass()->ImplementsInterface(UInterface_AI::StaticClass())) {
			IInterface_AI::Execute_setLockOnMarker(follow_cam->look_target, false);
		}
	}
	SetControlRotation(follow_cam->camera->GetComponentRotation());
}



/// <summary>
/// 타게팅 대상을 선정
/// ABaseEnemy를 상속하는 대상 중 카메라 각과 최소각을 이루는 대상을 선택
/// </summary>
/// <returns> 타게팅 대상 </returns>
AActor* AController_Player::findLockOnTarget()
{
	TArray<AActor*> founded;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), founded);
	float min_angle = -1;
	AActor* result = nullptr;
	for (AActor* i : founded) {
		/* 거리 1000 이상이면 타게팅 후보에서 제외 */
		if (GetPawn()->GetDistanceTo(i) > 1000)
			continue;
		/* 라인트레이스 해서 히트가 발생하면 후보에서 제외*/
		FHitResult hit_result;
		FVector cam_location = follow_cam->camera->GetComponentLocation();
		FVector target_location = i->GetActorLocation();
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), cam_location, target_location, ETraceTypeQuery::TraceTypeQuery1, false, founded, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit)
			continue;

		/* 액터간 lookat 벡터와 카메라 벡터를 내적한 결과가 가장 큰 ( 카메라와 이루는 각이 가장 작은 ) 액터를 선택 */
		FVector lookat_vector = UKismetMathLibrary::GetDirectionUnitVector(cam_location, target_location);
		FVector camera_vector = UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(follow_cam->camera->GetComponentRotation()), 0.0001f);
		float angle = UKismetMathLibrary::Dot_VectorVector(lookat_vector, camera_vector);
		if (angle > min_angle) {
			min_angle = angle;
			result = i;
		}
	}
	return result;
}


/// <summary>
/// 타게팅 대상을 변경
/// __direction이 + 이면 이전 타겟의 우측, -이면 좌측 대상을 출력
/// </summary>
/// <param name="__direction"> 마우스 휠 축 방향 </param>
/// <returns> 다음 타게팅 대상 </returns>
AActor* AController_Player::changeLockOnTarget(float __direction)
{
	FVector camera_location = follow_cam->camera->GetComponentLocation();
	FVector prev_look_vector = UKismetMathLibrary::GetDirectionUnitVector(camera_location, follow_cam->look_target->GetActorLocation());
	FRotator camera_rotation = follow_cam->camera->GetComponentRotation();
	FVector camera_forward_vector = UKismetMathLibrary::GetForwardVector(camera_rotation);
	TArray<AActor*> founded;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), founded);
	float min_angle = -1;
	AActor* result = nullptr;
	for (AActor* i : founded) {
		if (follow_cam->look_target == i)
			continue;
		FHitResult hit_result;
		FVector target_location = i->GetActorLocation();
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), camera_location, target_location, ETraceTypeQuery::TraceTypeQuery1, false, founded, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit)
			continue;
		FVector target_look_vector = UKismetMathLibrary::GetDirectionUnitVector(camera_location, target_location);
		float angle = UKismetMathLibrary::Dot_VectorVector(target_look_vector, camera_forward_vector);
		/* angle 이 0.6 이하라면 화면에 적이 없다고 판단하고 후보에서 제외, min_angle 과 비교하여 최소각을 가진 타겟을 찾음 */
		if (angle > 0.6f && angle > min_angle) {
			FVector rotated_target_prev_lookat_vector = UKismetMathLibrary::Quat_UnrotateVector(camera_rotation.Quaternion(), target_look_vector - camera_forward_vector);
			float prev_target_angle;
			float dummy;
			UKismetMathLibrary::GetYawPitchFromVector(rotated_target_prev_lookat_vector, prev_target_angle, dummy);
			/* 현재 타겟과 이전 타겟과의 좌우 방향 관계가 입력된 __direction 과 일치 하는지 검사 */
			if (prev_target_angle * __direction > 10) {
				min_angle = angle;
				result = i;
			}
		}
	}
	return result;
}

/// <summary>
/// stackable한 아이템인지 결과 반환
/// </summary>
/// <param name="__item_type"></param>
/// <returns></returns>
bool AController_Player::isStackable(EItemType __item_type)
{
	return __item_type == EItemType::Consumables || __item_type == EItemType::Raw;
}


/// <summary>
/// 서버에서 applyDamage를 실행
/// </summary>
/// <param name="__damaged_actor"></param>
/// <param name="__damage_id"></param>
/// <param name="__damage_causer"></param>
/// <param name="__hit_bone_name"></param>
void AController_Player::CtoS_applyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name)
{
	Server_ApplyDamage(__damaged_actor, __damage_id, __damage_causer, __hit_bone_name);
}

/// <summary>
/// 클라이언트 소유 액터의 applyDamage를 실행
/// </summary>
/// <param name="__damaged_actor"></param>
/// <param name="__damage_id"></param>
/// <param name="__damage_causer"></param>
/// <param name="__hit_bone_name"></param>
void AController_Player::Server_ApplyDamage_Implementation(AActor* __damaged_actor, FName __damage_id, AActor* __damage_causer, FName __hit_bone_name)
{
	if (__damaged_actor->GetClass()->ImplementsInterface(UInterface_BaseCharacter::StaticClass())) {
		IInterface_BaseCharacter::Execute_applyDamage(__damaged_actor, __damage_id, __damage_causer, __hit_bone_name);
	}
}

/// <summary>
/// 액터가 타게팅 중인 액터인지 반환
/// </summary>
/// <param name="actor"></param>
/// <param name="result"></param>
void AController_Player::isLock_On_Target_Implementation(AActor* actor, bool& result)
{
	result = false;
	if (follow_cam->IsValidLowLevel())
		if (actor == follow_cam->look_target)
			result = true;
}


/// <summary>
/// 현재 타겟팅 중인 적 액터 포인터 반환
/// </summary>
/// <returns></returns>
AActor* AController_Player::getLockOnTargetActor_Implementation()
{
	if (follow_cam->IsValidLowLevel())
		return follow_cam->look_target;
	else
		return nullptr;
}

/// <summary>
/// 빈 index 반환
/// </summary>
/// <returns></returns>
int32 AController_Player::findInventoryEmptyIndex_Implementation()
{
	TArray<int32> keys;
	inventory_list.GetKeys(keys);
	keys.Sort();
	int32 min = 0;
	for (auto i : keys) {
		if (i != min)
			return min;
		else
			min += 1;
	}
	return min;
}


/// <summary>
/// 동일한 __item_id를 가진 슬롯의 index 반환
/// </summary>
/// <param name="__item_id"></param>
/// <returns></returns>
int32 AController_Player::findSameItem_Implementation(FName __item_id)
{
	TArray<FInventoryData> values;
	TArray<int32> keys;
	inventory_list.GenerateValueArray(values);
	inventory_list.GetKeys(keys);
	int nums = values.Num();
	for (int i = 0; i < nums; i++) {
		if (values[i].item_id == __item_id) {
			return keys[i];
		}
	}
	return -1;
}

/// <summary>
/// 아이템 획득
/// </summary>
/// <param name="__item_id"></param>
/// <param name="__num"></param>
void AController_Player::getItem_Implementation(FName __item_id, int32 __num)
{
	if (inventory_list.Num() >= max_slot_size)
		return;
	FItemData itemdata;
	bool add_in_empty_slot = false;
	itemdata = Cast<UPWOGameInstance>(GetGameInstance())->findItemData(__item_id);
	/* stackable 한 아이템일 때 findSameItem 결과가 있으면 그 슬롯의 count만 증가시킴*/
	if (isStackable(itemdata.item_type)) {
		int32 same_item_index = findSameItem(__item_id);
		if (same_item_index >= 0) {
			inventory_list[same_item_index].count += __num;
			if (reverse_quickslot_list.Contains(same_item_index))
				refreshQuickSlot(reverse_quickslot_list[same_item_index]);
			return;
		}
		else {
			add_in_empty_slot = true;
		}
	}
	else {
		add_in_empty_slot = true;
	}
	/* empty index에 아이템 add */
	if (add_in_empty_slot) {
		int32 empty_index = findInventoryEmptyIndex();
		FInventoryData data;
		data.item_id = __item_id;
		data.count = __num;
		inventory_list.Add(TTuple<int32, FInventoryData>(empty_index, data));
	}
}

bool AController_Player::getIsLockOn_Implementation()
{
	return is_lock_on;
}

AFollowCam_Base* AController_Player::getFollowCam_Implementation()
{
	return follow_cam;
}

void AController_Player::showInteractionText_Implementation(const FText& __text)
{

}

void AController_Player::removeInteractionText_Implementation()
{

}

void AController_Player::swapInvenSlot_Implementation(int32 __from, int32 __to)
{
	FInventoryData fromdata = inventory_list[__from];
	if (inventory_list.Contains(__to)) {
		if (fromdata.item_id == inventory_list[__to].item_id && isStackable(Cast<UPWOGameInstance>(GetGameInstance())->findItemData(fromdata.item_id).item_type)) {
			inventory_list[__to].count += fromdata.count;
			inventory_list.Remove(__from);
		}
		else {
			inventory_list[__from] = inventory_list[__to];
			inventory_list[__to] = fromdata;
		}
	}
	else {
		inventory_list.Add(TTuple<int32, FInventoryData>(__to, fromdata));
		inventory_list.Remove(__from);
	}
	updateQuickSlotData(__from, __to);
}


void AController_Player::swapQuickSlot_Implementation(FKey __from, FKey __to)
{
	int32 fromdata = quickslot_list[__from];
	if (quickslot_list.Contains(__to)) {
		quickslot_list[__from] = quickslot_list[__to];
		quickslot_list[__to] = fromdata;
		reverse_quickslot_list[fromdata] = __to;
		reverse_quickslot_list[quickslot_list[__from]] = __from;
	}
	else {
		quickslot_list.Add(TTuple<FKey, int32>(__to, fromdata));
		quickslot_list.Remove(__from);
		reverse_quickslot_list[fromdata] = __to;
	}
	refreshQuickSlot(__from);
	refreshQuickSlot(__to);
}

void AController_Player::registerInventoQuick_Implementation(int32 __from, FKey __to)
{
	if (reverse_quickslot_list.Contains(__from)) {
		FKey prev_key = reverse_quickslot_list[__from];
		if (__to == prev_key)
			return;
		if (quickslot_list.Contains(__to)) {
			quickslot_list.Remove(prev_key);
			reverse_quickslot_list[__from] = __to;
			reverse_quickslot_list.Remove(quickslot_list[__to]);
			quickslot_list[__to] = __from;
		}
		else {
			quickslot_list.Remove(prev_key);
			reverse_quickslot_list[__from] = __to;
			quickslot_list.Add(TTuple<FKey, int32>(__to, __from));
		}
		refreshQuickSlot(prev_key);
		refreshQuickSlot(__to);
		return;
	}
	if (quickslot_list.Contains(__to)) {
		reverse_quickslot_list.Remove(quickslot_list[__to]);
		quickslot_list[__to] = __from;
		reverse_quickslot_list.Add(TTuple<int32, FKey>(__from, __to));
	}
	else {
		quickslot_list.Add(TTuple<FKey, int32>(__to, __from));
		reverse_quickslot_list.Add(TTuple<int32, FKey>(__from, __to));
	}
	refreshQuickSlot(__to);
}

void AController_Player::refreshQuickSlot(FKey __key)
{
	UPWOGameInstance* gameinstance = Cast<UPWOGameInstance>(GetGameInstance());
	UWidget_ItemSlot* prev_quick_slot = gameinstance->quickslot_references[__key];
	if (quickslot_list.Contains(__key) && inventory_list.Contains(quickslot_list[__key])) {
		prev_quick_slot->my_index = quickslot_list[__key];
		FInventoryData data = inventory_list[prev_quick_slot->my_index];
		prev_quick_slot->item_id = data.item_id;
		prev_quick_slot->count = data.count;
	}
	else {
		prev_quick_slot->item_id = "None";
		prev_quick_slot->my_index = -1;
		prev_quick_slot->count = 0;
	}
	prev_quick_slot->initSlot();
}

void AController_Player::updateQuickSlotData(int32 __from, int32 __to) {
	FKey prevfromkey;
	FKey prevtokey;
	if (reverse_quickslot_list.Contains(__from)) {
		prevfromkey = reverse_quickslot_list[__from];
	}
	if (reverse_quickslot_list.Contains(__to)) {
		prevtokey = reverse_quickslot_list[__to];
	}
	bool fromflag = false;
	bool toflag = false;
	if (prevfromkey.IsValid()) {
		reverse_quickslot_list.Remove(__from);
		quickslot_list[prevfromkey] = __to;
		fromflag = true;
	}
	if (prevtokey.IsValid()) {
		if (inventory_list.Contains(__from)) {
			reverse_quickslot_list.Remove(__to);
			quickslot_list[prevtokey] = __from;
			toflag = true;
		}
		else if(prevfromkey.IsValid()){
			quickslot_list.Remove(prevfromkey);
			fromflag = false;
		}
	}
	if (prevfromkey.IsValid()) {
		if(fromflag)
			reverse_quickslot_list.Add(TTuple<int32, FKey>(__to, prevfromkey));
		refreshQuickSlot(prevfromkey);
	}
	if (prevtokey.IsValid()) {
		if(toflag)
			reverse_quickslot_list.Add(TTuple<int32, FKey>(__from, prevtokey));
		refreshQuickSlot(prevtokey);
	}
}

void AController_Player::removeQuickSlot_Implementation(FKey __key)
{
	reverse_quickslot_list.Remove(quickslot_list[__key]);
	quickslot_list.Remove(__key);
	refreshQuickSlot(__key);
}


FInventoryData AController_Player::getInventoryData_Implementation(int32 __index)
{
	if(inventory_list.Contains(__index))
		return inventory_list[__index];
	else {
		FInventoryData dummy;
		dummy.item_id = "None";
		return dummy;
	}
}

bool AController_Player::isInteractionTarget_Implementation(AActor* __actor)
{
	return interaction_target == __actor;
}

bool AController_Player::isRegisteredQuickSlotKey_Implementation(FKey __key)
{
	return quickslot_list.Contains(__key);
}

int32 AController_Player::getRegisteredQuickSlotInvenIndex_Implementation(FKey __key)
{
	if (quickslot_list.Contains(__key)) {
		return quickslot_list[__key];
	}
	else {
		return -1;
	}
}


void AController_Player::decreseItem_Implementation(int32 __index, int32 __decrease_num)
{
	if (inventory_list.Contains(__index) == false)
		return;
	if (inventory_list[__index].count - __decrease_num <= 0) {
		inventory_list.Remove(__index);
		if (reverse_quickslot_list.Contains(__index)) {
			FKey tmpkey = reverse_quickslot_list[__index];
			reverse_quickslot_list.Remove(__index);
			quickslot_list.Remove(tmpkey);
			refreshQuickSlot(tmpkey);
		}
	}
	else {
		inventory_list[__index].count -= __decrease_num;
		if (reverse_quickslot_list.Contains(__index)) {
			refreshQuickSlot(reverse_quickslot_list[__index]);
		}
	}
}