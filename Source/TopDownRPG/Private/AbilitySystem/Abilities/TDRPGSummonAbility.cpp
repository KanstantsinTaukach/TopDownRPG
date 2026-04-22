// Copyright K.Taukach

#include "AbilitySystem/Abilities/TDRPGSummonAbility.h"

TArray<FVector> UTDRPGSummonAbility::GetSpawnLocations()
{
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    const float DeltaSpread = SpawnSpread / NumMinions;

    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.0, FVector::UpVector);
    TArray<FVector> SpawnLocations;
    for (int32 i = 0; i < NumMinions; ++i)
    {
        const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * (i + 0.5), FVector::UpVector);
        FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

        FHitResult Hit;
        GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.0, 0.0, 400.0), ChosenSpawnLocation - FVector(0.0, 0.0, 400.0), ECC_Visibility);
        if(Hit.bBlockingHit)
        {
            ChosenSpawnLocation = Hit.ImpactPoint;
        }
        SpawnLocations.Add(ChosenSpawnLocation);
    }
 
    return SpawnLocations;
}