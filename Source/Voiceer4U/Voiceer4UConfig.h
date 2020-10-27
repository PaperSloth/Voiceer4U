// Copyright(c) 2020 PaperSloth

#pragma once

#include "CoreMinimal.h"
#include "Settings/EditorSettings.h"
#include "Voiceer4UConfig.generated.h"

UCLASS(config=Game, defaultconfig)
class VOICEER4U_API UVoiceer4UConfig : public UObject
{
	GENERATED_BODY()

public:
	UVoiceer4UConfig(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, config, Category = "UVoiceer4U")
	bool UseEngineDefaultSound;
	UPROPERTY(EditAnywhere, config, Category = "UVoiceer4U")
	int32 MaxRegistrySound;
	UPROPERTY(EditAnywhere, config, Category = "UVoiceer4U")
	FString RootDirectory;
};
