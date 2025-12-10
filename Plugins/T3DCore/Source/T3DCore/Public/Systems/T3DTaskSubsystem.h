// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/T3DTaskData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "T3DTaskSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class T3DCORE_API UT3DTaskSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
public:
	// Start a mission (by data asset)
	void StartTask(UT3DTaskData* Task);

	// Called when an event happens (kill/collect/location)
	UFUNCTION()
	void NotifyEnemyKilled(AActor* Enemy);
	UFUNCTION()
	void NotifyItemCollected(FName ItemID);
	UFUNCTION()
	void NotifyReachedLocation(); // location trigger simply calls this

	
	bool IsTaskActive() const { return ActiveTask != nullptr; }
	FName GetActiveTaskID() const;
	
	// Save/Load
	void SaveTaskProgress();
	void LoadTaskProgress();
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY()
	UT3DTaskData* ActiveTask{ nullptr };
	
	// For linear tasks we only need current objective index and current count
	int32 CurrentObjectiveIndex = 0;
	int32 CurrentObjectiveCount = 0;
	
	void AdvanceObjectiveIfComplete();
	void CompleteTask();

	// helper to persist (slot name)
	FString SaveSlotName = TEXT("PlayerSaveSlot");
	uint32 UserIndex = 0;
};
