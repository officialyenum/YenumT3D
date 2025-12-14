// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SettingsSaveGame.h"

#include "Data/PendingSettings.h"
#include "Subsystems/SettingsSubsystem.h"

void USettingsSaveGame::SaveData(const FPendingSettings& PendingSettingsData)
{
	
	Data.GraphicsQuality = PendingSettingsData.GraphicsQuality;
	Data.Brightness = PendingSettingsData.Brightness;
	Data.MasterVolume = PendingSettingsData.MasterVolume;
	Data.SFXVolume = PendingSettingsData.SFXVolume;
	Data.DialogueVolume = PendingSettingsData.DialogueVolume;
	Data.WindowMode = PendingSettingsData.WindowMode;
}


