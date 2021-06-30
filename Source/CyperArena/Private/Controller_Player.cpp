// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller_Player.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void AController_Player::BeginPlay()
{
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
}