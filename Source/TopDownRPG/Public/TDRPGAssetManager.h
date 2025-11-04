// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TDRPGAssetManager.generated.h"


UCLASS()
class TOPDOWNRPG_API UTDRPGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
    static UTDRPGAssetManager& Get();

protected:
    virtual void StartInitialLoading() override;
};
