// Copyright K.Taukach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDRPGGameModeBase.generated.h"

class UCharacterClassInfo;

UCLASS()
class TOPDOWNRPG_API ATDRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
    TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
