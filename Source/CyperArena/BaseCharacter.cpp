// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//리플리케이션 설정
	bReplicates = true;

	look_yaw = 0.0f;
	look_pitch = 0.0f;
	R_look_rotation = FRotator(0.000000, 0.000000, 0.000000);

	ragdoll_physics_handle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	ragdoll_physics_handle->CreationMethod = EComponentCreationMethod::Native;
	ragdoll_physics_handle->InterpolationSpeed = 10000000272564224.000000f;
	ragdoll_physics_handle->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Make sure the mesh and animbp update after the CharacterBP
	//ACharacter::GetMesh()->AddTickPrerequisiteActor(this);


}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UKismetSystemLibrary::PrintString(this,"SDF\n");
	//d_time = DeltaTime;
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 리플리케이션 가능하게 하기
void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, R_look_rotation);
}

// 캐릭터가 바라보는 방향을 결정하는 변수 설정
void ABaseCharacter::Look_Implementation()
{
	FRotator tmp_rotator1 = UKismetMathLibrary::MakeRotator(0, look_pitch, look_yaw);
	FRotator tmp_rotator2 = UKismetMathLibrary::NormalizedDeltaRotator(R_look_rotation, AActor::K2_GetActorRotation());
	FRotator interp_tmp = UKismetMathLibrary::RInterpTo(tmp_rotator1, tmp_rotator2, d_time, 15.000000);
	look_pitch = UKismetMathLibrary::ClampAngle(interp_tmp.Pitch, -90.000000, 90.000000);
	look_yaw = UKismetMathLibrary::ClampAngle(interp_tmp.Yaw, -90.000000, 90.000000);
	UKismetSystemLibrary::PrintString(this, "SDF\n");
	return;
}

//void ABaseCharacter::rotateActorTimeline(FRotator target_rotation, float time)
//{
//
//}