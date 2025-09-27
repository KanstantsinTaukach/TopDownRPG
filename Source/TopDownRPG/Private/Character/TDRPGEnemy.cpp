// Copyright K.Taukach

#include "Character/TDRPGEnemy.h"

#include "TopDownRPG/TopDownRPG.h"

ATDRPGEnemy::ATDRPGEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void ATDRPGEnemy::HighlightActor()
{
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

    WeaponSkeletalMesh->SetRenderCustomDepth(true);
    WeaponSkeletalMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATDRPGEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);

    WeaponSkeletalMesh->SetRenderCustomDepth(false);
}