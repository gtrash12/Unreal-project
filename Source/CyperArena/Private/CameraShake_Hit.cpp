// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake_Hit.h"

UCameraShake_Hit::UCameraShake_Hit()
{
    OscillationDuration = 0.25f;
    OscillationBlendInTime = 0.0f;
    OscillationBlendOutTime = 0.05f;

    RotOscillation.Pitch.Amplitude = FMath::RandRange(2, 4);
    RotOscillation.Pitch.Frequency = FMath::RandRange(10, 15);

    RotOscillation.Pitch.Amplitude = FMath::RandRange(2, 4);
    RotOscillation.Pitch.Frequency = FMath::RandRange(10, 15);
}