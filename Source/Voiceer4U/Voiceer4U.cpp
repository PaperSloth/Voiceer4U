// Copyright(c) 2020 PaperSloth

#include "Voiceer4U.h"
#include "Voiceer4UConfig.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "Sound/SoundNodeRandom.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "FileHelpers.h"
#endif // WITH_EDITOR

#define LOCTEXT_NAMESPACE "FVoiceer4UModule"

void FVoiceer4UModule::StartupModule()
{
#if WITH_EDITOR
	if (!GEngine) return;
	ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (settingsModule != nullptr)
	{
		ISettingsSectionPtr settingsSection = settingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"Voiceer4U",
			LOCTEXT("Voiceer4USettings", "Voiceer4U"),
			LOCTEXT("Voiceer4USettingsDescription", "Configure settings for the use of Voiceer4U."),
			GetMutableDefault<UVoiceer4UConfig>()
		);
		if (settingsSection.IsValid())
		{
			settingsSection->OnModified().BindRaw(this, &FVoiceer4UModule::OnSaveConfig);
		}
	}

	compileStartSound = LoadObject<USoundCue>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileStart_Cue.CompileStart_Cue"));
	compileSuccessSound = LoadObject<USoundCue>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
	compileFailSound = LoadObject<USoundCue>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
	if (!compileStartSound || !compileSuccessSound || !compileFailSound) return;

	engineStartSuccessWave = LoadObject<USoundWave>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileSuccess.CompileSuccess"));
	engineFailedWave = LoadObject<USoundWave>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileFailed.CompileFailed"));
	if (!engineStartSuccessWave || !engineFailedWave) return;

	const UVoiceer4UConfig* config = GetDefault<UVoiceer4UConfig>();
	if (!config) return;
	if (config->UseEngineDefaultSound)
	{
		// エンジン標準のSoundに初期化
		ResetSoundCue(compileStartSound, engineStartSuccessWave, 0.4f, 1.2f);
		ResetSoundCue(compileSuccessSound, engineStartSuccessWave, 0.6f, 1.0f);
		ResetSoundCue(compileFailSound, engineFailedWave, 0.4f, 1.0f);
	}
	else
	{
		// ユーザーが設定したSoundをRandom再生するように設定
		const int32 MaxSound = config->MaxRegistrySound;
		const FString& Root = config->RootDirectory;
		InitializeRandomSound(compileStartSound, ESoundCategory::CompileStart, MaxSound, Root);
		InitializeRandomSound(compileSuccessSound, ESoundCategory::CompileSuccess, MaxSound, Root);
		InitializeRandomSound(compileFailSound, ESoundCategory::CompileFailed, MaxSound, Root);
	}

	// 変更の保存
	TArray<UPackage*> Packages;
	Packages.Emplace(compileStartSound->GetOutermost());
	Packages.Emplace(compileSuccessSound->GetOutermost());
	Packages.Emplace(compileFailSound->GetOutermost());
	UEditorLoadingAndSavingUtils::SavePackages(Packages, true);
	UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);

	FEditorDelegates::OnShutdownPostPackagesSaved.AddRaw(this, &FVoiceer4UModule::OnResetSound);
#endif // WITH_EDITOR
}

// Editor終了時に元のサウンドデータに差し替え
void FVoiceer4UModule::OnResetSound()
{
#if WITH_EDITOR
	if (!GEngine) return;
	ResetSoundCue(compileStartSound, engineStartSuccessWave, 0.4f, 1.2f);
	ResetSoundCue(compileSuccessSound, engineStartSuccessWave, 0.6f, 1.0f);
	ResetSoundCue(compileFailSound, engineFailedWave, 0.4f, 1.0f);

	// 変更の保存
	TArray<UPackage*> Packages;
	Packages.Emplace(compileStartSound->GetOutermost());
	Packages.Emplace(compileSuccessSound->GetOutermost());
	Packages.Emplace(compileFailSound->GetOutermost());
	UEditorLoadingAndSavingUtils::SavePackages(Packages, true);
	UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
#endif // WITH_EDITOR
}

// ProjectSettingsで設定したDirectory名に沿ってFileの検索 / Random再生用のNode設定
void FVoiceer4UModule::InitializeRandomSound(USoundCue* cue, const ESoundCategory Category, const int SoundNum, const FString& Root)
{
	cue->ResetGraph();

	FString directory;
	FString file;
	switch (Category)
	{
	case ESoundCategory::CompileStart:
		directory = TEXT("OnCompileStart");
		file = TEXT("CompileStart");
		break;
	case ESoundCategory::CompileSuccess:
		directory = TEXT("OnCompileSuccess");
		file = TEXT("CompileSuccess");
		break;
	case ESoundCategory::CompileFailed:
		directory = TEXT("OnCompileFailed");
		file = TEXT("CompileFailed");
		break;
	default:
		return;
	}

	TArray<USoundWave*> waveArray;
	TArray<USoundNode*> soundNodeArray;
	USoundNodeRandom* randomNode = cue->ConstructSoundNode<USoundNodeRandom>();
	for (int i = 0; i < SoundNum; i++)
	{
		FString path = FString::Format(TEXT("/Voiceer4U/{0}/{1}/{2}{3}.{2}{3}"), { Root, directory, file, i });
		USoundWave* wave = LoadObject<USoundWave>(nullptr, *path);
		if (!wave)
		{
			continue;
		}
		waveArray.Emplace(wave);

		USoundNodeWavePlayer* player = cue->ConstructSoundNode<USoundNodeWavePlayer>();
		player->SetSoundWave(waveArray[i]);
		player->GraphNode->NodePosX = -515;
		player->GraphNode->NodePosY = i * 75;
		soundNodeArray.Emplace(player);
		randomNode->InsertChildNode(i);
	}
	// サウンドファイルを取得できない場合はエンジンの標準のサウンドにする
	if (waveArray.Num() <= 0)
	{
		switch (Category)
		{
		case ESoundCategory::CompileStart:
			ResetSoundCue(compileStartSound, engineStartSuccessWave, 0.4f, 1.2f);
			return;
		case ESoundCategory::CompileSuccess:
			ResetSoundCue(compileSuccessSound, engineStartSuccessWave, 0.6f, 1.0f);
			return;
		case ESoundCategory::CompileFailed:
			ResetSoundCue(compileFailSound, engineFailedWave, 0.4f, 1.0f);
			return;
		default:
			return;
		}
	}

	randomNode->SetChildNodes(soundNodeArray);
	randomNode->GraphNode->NodePosX = -260;

	cue->FirstNode = randomNode;
	cue->LinkGraphNodesFromSoundNodes();
	cue->VolumeMultiplier = 1.0f;
	cue->PitchMultiplier = 1.0f;
}

// Engine標準のSoundに初期化
void FVoiceer4UModule::ResetSoundCue(USoundCue* cue, USoundWave* wave, const float Volume, const float Pitch)
{
	cue->ResetGraph();

	USoundNodeWavePlayer* player = cue->ConstructSoundNode<USoundNodeWavePlayer>();
	player->SetSoundWave(wave);
	
	cue->FirstNode = player;
	cue->LinkGraphNodesFromSoundNodes();
	cue->VolumeMultiplier = Volume;
	cue->PitchMultiplier = Pitch;
}

// configの保存用Callback
bool FVoiceer4UModule::OnSaveConfig()
{
	UVoiceer4UConfig* config = GetMutableDefault<UVoiceer4UConfig>();
	if (!config)
	{
		return false;
	}
	config->SaveConfig();
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVoiceer4UModule, Voiceer4U)
