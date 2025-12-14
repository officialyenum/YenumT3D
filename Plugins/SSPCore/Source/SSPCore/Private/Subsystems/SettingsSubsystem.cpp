// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SettingsSubsystem.h"

#include "Data/PendingSettings.h"
#include "Data/SettingsSaveGame.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"

static const FName SoundMixName = TEXT("SoundMix");
static const FName MasterSoundClassName = TEXT("Master");
static const FName SFXSoundClassName = TEXT("SFX");
static const FName DialogueSoundClassName = TEXT("Dialogue");
static TArray<FIntPoint> CachedResolutions;

void USettingsSubsystem::ApplySettings()
{
	auto* GS = GEngine->GetGameUserSettings();
	if (PendingSettings.GraphicsQuality >= 0)
	{
		SaveGame->Data.GraphicsQuality = PendingSettings.GraphicsQuality;
		GS->SetOverallScalabilityLevel(PendingSettings.GraphicsQuality);
	}
	if (PendingSettings.Brightness >= 0)
	{
		SaveGame->Data.Brightness = PendingSettings.Brightness;
		float Gamma = FMath::Clamp(PendingSettings.Brightness, 0.5f, 3.0f);

		if (UWorld* World = GetWorld())
		{
			FString Cmd = FString::Printf(TEXT("r.ToneMapperGamma %.2f"), Gamma);
			World->Exec(World, *Cmd);
		}
	}
	if (PendingSettings.ResolutionIndex >= 0 && CachedResolutions.IsValidIndex(PendingSettings.ResolutionIndex))
	{
		SaveGame->Data.ResolutionIndex = PendingSettings.ResolutionIndex;
		GS->SetScreenResolution(CachedResolutions[PendingSettings.ResolutionIndex]);
		GS->ApplyResolutionSettings(false);
	}
	if (PendingSettings.WindowMode >= 0)
	{
		EWindowMode::Type Mode =
			PendingSettings.WindowMode == 0 ? EWindowMode::Fullscreen :
			PendingSettings.WindowMode == 1 ? EWindowMode::WindowedFullscreen : EWindowMode::Windowed;
		GS->SetFullscreenMode(Mode);
		int32 CurrentIndex = CachedResolutions.Find(GS->GetScreenResolution());
		switch (Mode)
		{
		case EWindowMode::Fullscreen:
			break;
		case EWindowMode::WindowedFullscreen:
			if (CurrentIndex >= 0 && CachedResolutions.IsValidIndex(CurrentIndex))
			{
				PendingSettings.ResolutionIndex = CurrentIndex;
				SaveGame->Data.ResolutionIndex = CurrentIndex;
				GS->SetScreenResolution(CachedResolutions[PendingSettings.ResolutionIndex]);
				GS->ApplyResolutionSettings(false);
			}
			GS->ApplyResolutionSettings(false);
			break;
		case EWindowMode::Windowed:
			break;
		case EWindowMode::NumWindowModes:
			break;
		}
		SaveGame->Data.WindowMode = PendingSettings.WindowMode;
	}

	GS->ApplySettings(false);
	GS->SaveSettings();
	SaveGame->SaveData(PendingSettings);
	UGameplayStatics::SaveGameToSlot(SaveGame, "YenumUserSettings", 0);
	OnSettingsApplied.Broadcast(SaveGame);
	PendingSettings.Reset();
}

void USettingsSubsystem::ApplySoundSettings(USoundMix* SoundMix, USoundClass* SoundClass, EVolumeType SoundType) const
{
	if (!SoundMix || !SoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplySoundSettings: Invalid SoundMix or SoundClass"));
		return;
	}
	float Volume = -1.0f;
	switch (SoundType)
	{
	case EVolumeType::EVT_Master:
        Volume = PendingSettings.MasterVolume;
		SaveGame->Data.MasterVolume = Volume;
		break;
	case EVolumeType::EVT_SFX:
        SaveGame->Data.MasterVolume = PendingSettings.SFXVolume;
		SaveGame->Data.SFXVolume = Volume;
		break;
	case EVolumeType::EVT_Dialogue:
        Volume = PendingSettings.DialogueVolume;
		SaveGame->Data.DialogueVolume = Volume;
		break;
	default:
		return;
	}
	if (Volume < 0.0f)
	{
		return; // not queued
	}
    Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
	// Apply sound class override
	UGameplayStatics::SetSoundMixClassOverride(
		this,
		SoundMix,
		SoundClass,
		Volume,
		1.0f    // Pitch
	);
	SaveGame->SaveData(PendingSettings);
	UGameplayStatics::SaveGameToSlot(SaveGame, "YenumUserSettings", 0);
	//Activate Mix
	UGameplayStatics::PushSoundMixModifier(this, SoundMix);
	OnSoundSettingsApplied.Broadcast(SoundType);
}

void USettingsSubsystem::RevertPendingSettings()
{
	PendingSettings = {};
    OnSettingsReverted.Broadcast(PendingSettings);
}

void USettingsSubsystem::ResetToDefaults()
{
	auto* GS = GEngine->GetGameUserSettings();
	PendingSettings = DefaultSettings.GetAsPendingSettings();
	PendingSettings.ResolutionIndex = CachedResolutions.Find(GS->GetScreenResolution());
	OnSettingsResetToDefaults.Broadcast(PendingSettings);
	ApplySettings();
}

void USettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UGameplayStatics::DoesSaveGameExist("YenumUserSettings", 0))
	{
		SaveGame = Cast<USettingsSaveGame>(
			UGameplayStatics::LoadGameFromSlot("YenumUserSettings", 0));
		PendingSettings = SaveGame->Data.GetAsPendingSettings();
			OnSettingsInitialized.Broadcast(PendingSettings);
			ApplySettings();
	}
	else
	{
		SaveGame = Cast<USettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingsSaveGame::StaticClass()));
		PendingSettings = SaveGame->Data.GetAsPendingSettings();
		OnSettingsInitialized.Broadcast(PendingSettings);
	}
}

void USettingsSubsystem::QueueGraphicsQuality(int32 Level)
{
	PendingSettings.GraphicsQuality = Level;
	bHasPendingChanges = true;
	OnSettingsQueued.Broadcast();
}

void USettingsSubsystem::QueueBrightness(float Value)
{
	PendingSettings.Brightness = Value;
	bHasPendingChanges = true;
	OnSettingsQueued.Broadcast();
}

void USettingsSubsystem::QueueVolume(float Value, EVolumeType VolumeType)
{
	switch (VolumeType)
	{
	case EVolumeType::EVT_Master:
		PendingSettings.MasterVolume = Value;
		break;
	case EVolumeType::EVT_SFX:
		PendingSettings.SFXVolume = Value;
		break;
	case EVolumeType::EVT_Dialogue:
		PendingSettings.DialogueVolume = Value;
		break;
	case EVolumeType::EVT_MAX:
		break;
	}
	bHasPendingChanges = true;
	OnSettingsQueued.Broadcast();
}

void USettingsSubsystem::QueueResolution(int32 Index)
{
	PendingSettings.ResolutionIndex = Index;
	bHasPendingChanges = true;
	OnSettingsQueued.Broadcast();
}

void USettingsSubsystem::QueueWindowMode(int32 Index)
{
	PendingSettings.WindowMode = Index;
	bHasPendingChanges = true;
	OnSettingsQueued.Broadcast();
}

TArray<FString> USettingsSubsystem::GetAvailableResolutions()
{
	if (CachedResolutions.Num() == 0)
	{
		FScreenResolutionArray Resolutions;
		RHIGetAvailableResolutions(Resolutions, false);
		for (const FScreenResolutionRHI& R : Resolutions)
		{
			CachedResolutions.AddUnique(FIntPoint(R.Width, R.Height));
		}
	}
	
	TArray<FString> Out;
	for (const auto& R : CachedResolutions)
	{
		Out.Add(FString::Printf(TEXT("%d x %d"), R.X, R.Y));
	}
	return Out;
}

TArray<FString> USettingsSubsystem::GetWindowModes() const
{
	return { TEXT("Fullscreen"), TEXT("Windowed"), TEXT("Borderless") };
}

int32 USettingsSubsystem::GetCurrentResolutionIndex() const
{
	auto* GS = GEngine->GetGameUserSettings();
	FString Current = FString::Printf(TEXT("%d x %d"), GS->GetScreenResolution().X, GS->GetScreenResolution().Y);

	auto StringList = GetAvailableResolutions();
	return StringList.IndexOfByKey(Current);
}

int32 USettingsSubsystem::GetCurrentGraphics()
{
	auto* GS = GEngine->GetGameUserSettings();
	return GS->GetOverallScalabilityLevel();
}

int32 USettingsSubsystem::GetCurrentWindowMode()
{
	switch (GEngine->GetGameUserSettings()->GetFullscreenMode()) {
	case EWindowMode::Fullscreen: return 0;
	case EWindowMode::WindowedFullscreen: return 1;
	case EWindowMode::Windowed: return 2;
	default: return 1;
	}
}

FPendingSettings USettingsSubsystem::GetCurrentSettingsData() const
{
	if (PendingSettings.IsValid())
	{
		return PendingSettings;
	}
	
	if (UGameplayStatics::DoesSaveGameExist("YenumUserSettings", 0))
	{
		if (const auto* Save = Cast<USettingsSaveGame>(
			UGameplayStatics::LoadGameFromSlot("YenumUserSettings", 0)))
		{
			return Save->Data.GetAsPendingSettings();
		}
	}
	
	return DefaultSettings.GetAsPendingSettings();
}
