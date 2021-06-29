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
		FVector pawn_location = GetPawn()->GetActorLocation();
		bool is_hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), cam_location, pawn_location, ETraceTypeQuery::TraceTypeQuery1, false, founded, EDrawDebugTrace::Type::None, hit_result, true);
		if (is_hit)
			continue;

		/* 액터간 lookat 벡터와 카메라 벡터를 내적한 결과가 가장 큰 ( 카메라와 이루는 각이 가장 작은 ) 액터를 선택 */
		FVector lookat_vector = UKismetMathLibrary::GetDirectionUnitVector(cam_location, pawn_location);
		FVector camera_vector = UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(follow_cam->camera->GetComponentRotation()), 0.0001f);
		float angle = UKismetMathLibrary::Dot_VectorVector(lookat_vector, camera_vector);
		if (angle > min_angle) {
			min_angle = angle;
			result = i;
		}
	}
	return result;
}