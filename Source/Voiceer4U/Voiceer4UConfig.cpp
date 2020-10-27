// Copyright(c) 2020 PaperSloth

#include "Voiceer4UConfig.h"

UVoiceer4UConfig::UVoiceer4UConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	UseEngineDefaultSound(false),
	MaxRegistrySound(10),
	RootDirectory(TEXT("MakinoMonaka"))
{
}
