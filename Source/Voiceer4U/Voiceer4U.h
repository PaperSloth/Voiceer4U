// Copyright(c) 2020 PaperSloth

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class USoundCue;
class USoundWave;

enum class ESoundCategory : uint8
{
	CompileStart,
	CompileSuccess,
	CompileFailed,
};

class FVoiceer4UModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override { }

private:
	USoundCue* compileStartSound;
	USoundCue* compileSuccessSound;
	USoundCue* compileFailSound;

	USoundWave* engineStartSuccessWave;
	USoundWave* engineFailedWave;

	void OnResetSound();
	void InitializeRandomSound(USoundCue* cue, const ESoundCategory Category, const int SoundNum, const FString& Root);
	void ResetSoundCue(USoundCue* cue, USoundWave* wave, const float Volume, const float Pitch);
	bool OnSaveConfig();
};
