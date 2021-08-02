// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowCam_Base.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFollowCam_Base::AFollowCam_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    spring_arm->SetupAttachment(RootComponent);
    spring_arm->SetRelativeLocationAndRotation(FVector(0.0f, 20.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
    spring_arm->TargetArmLength = 350;
    spring_arm->SocketOffset = FVector(0, 20, -30);
    spring_arm->TargetOffset = FVector(0, 0, 80);
    spring_arm->ProbeSize = 20;
    //spring_arm->bEnableCameraLag = true;
    spring_arm->bEnableCameraRotationLag = true;
    spring_arm->CameraLagMaxDistance = 100;
    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    camera->SetupAttachment(spring_arm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AFollowCam_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFollowCam_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (follow_target->IsValidLowLevel() == false)
        return;
    FVector trace_start = follow_target->GetActorLocation();
    trace_start.Z += 50;
    FVector trace_end = spring_arm->GetRightVector() * 60 + trace_start;

    const TArray<AActor*> ignore;
    FHitResult hit_result;
    /* 캐릭터의 카메라 기준 우측으로 LineTraceSingle 을 수행해 벽이 감지되면 카메라를 충돌 거리만큼 좌측으로 옮김 */
    UKismetSystemLibrary::LineTraceSingle(this, trace_start, trace_end, ETraceTypeQuery::TraceTypeQuery2, false, ignore, EDrawDebugTrace::Type::None, hit_result, true);
    if (hit_result.bBlockingHit) {
        location_offset.Y = hit_result.Distance - 60;
    }
    else {
        location_offset.Y = 0;
    }

    FVector target_location = follow_target->GetActorLocation() + follow_target->GetActorRightVector() * location_offset.Y;
    FRotator target_rotation;
    /* 락온 타게팅 상태라면 락온 타겟의 look at 방향으로 controlRotation 을 대체 */
    if (is_lock_on) {
        target_rotation = UKismetMathLibrary::FindLookAtRotation(camera->GetComponentLocation(), look_target->GetActorLocation());
        GetWorld()->GetFirstPlayerController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(follow_target->GetActorLocation(), look_target->GetActorLocation()));
    }
    else {
        target_rotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
    }
    SetActorLocationAndRotation(target_location, target_rotation);
}

