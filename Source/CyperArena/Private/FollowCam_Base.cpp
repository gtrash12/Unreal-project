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
    spring_arm->TargetArmLength = 250;
    spring_arm->SocketOffset = FVector(0, 20, -30);
    spring_arm->TargetOffset = FVector(0, 0, 80);
    spring_arm->ProbeSize = 20;
    spring_arm->bEnableCameraLag = true;
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
    if (look_target->IsValidLowLevel() == false)
        return;
    FVector trace_start = GetActorLocation();
    trace_start.Z += 50;
    FVector trace_end = GetActorLocation();
    UKismetSystemLibrary::LineTraceSingle();
}

