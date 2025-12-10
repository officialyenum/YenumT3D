// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/T3DTaskSubsystem.h"

#include "Events/T3DGameEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/TaskSave.h"


void UT3DTaskSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Bind to Global Events
	if (UT3DGameEvents* GE = GetGameInstance()->GetSubsystem<UT3DGameEvents>())
	{
		GE->OnEnemyKilled.AddDynamic(this, &UT3DTaskSubsystem::NotifyEnemyKilled);
		GE->OnItemCollected.AddDynamic(this, &UT3DTaskSubsystem::NotifyItemCollected);
	}

	LoadTaskProgress();
}

void UT3DTaskSubsystem::StartTask(UT3DTaskData* Task)
{
	if (!Task) return;
	ActiveTask = Task;
	CurrentObjectiveIndex = 0;
	CurrentObjectiveCount = 0;
	
	UE_LOG(LogTemp, Log, TEXT("Task started: %s"), *Task->TaskID.ToString());

	SaveTaskProgress();
}

void UT3DTaskSubsystem::NotifyEnemyKilled(AActor* Enemy)
{
	if (!ActiveTask) return;

	const FT3DTask& Obj = ActiveTask->Tasks[CurrentObjectiveIndex];
	if (Obj.TaskType != ET3DTaskType::KillEnemy) return;

	CurrentObjectiveCount++;
	UE_LOG(LogTemp, Log, TEXT("Kill objective ++ (%d/%d)"), CurrentObjectiveCount, Obj.TargetCount);
	AdvanceObjectiveIfComplete();
}

void UT3DTaskSubsystem::NotifyItemCollected(FName ItemID)
{
	if (!ActiveTask) return;

	const FT3DTask& Obj = ActiveTask->Tasks[CurrentObjectiveIndex];
	if (Obj.TaskType != ET3DTaskType::CollectItem) return;

	CurrentObjectiveCount++;
	UE_LOG(LogTemp, Log, TEXT("Collect objective ++ (%d/%d)"), CurrentObjectiveCount, Obj.TargetCount);
	AdvanceObjectiveIfComplete();
}

void UT3DTaskSubsystem::NotifyReachedLocation()
{
	if (!ActiveTask) return;

	const FT3DTask& Obj = ActiveTask->Tasks[CurrentObjectiveIndex];
	if (Obj.TaskType != ET3DTaskType::ReachLocation) return;

	// For reach-location objectives we assume trigger fires once
	CurrentObjectiveCount = Obj.TargetCount;
	UE_LOG(LogTemp, Log, TEXT("Reached location objective"));
	AdvanceObjectiveIfComplete();
}

FName UT3DTaskSubsystem::GetActiveTaskID() const
{
	return  ActiveTask ? ActiveTask->TaskID : NAME_None;
}

void UT3DTaskSubsystem::SaveTaskProgress()
{
	UTaskSave* TSG = Cast<UTaskSave>(UGameplayStatics::CreateSaveGameObject(UTaskSave::StaticClass()));
	if (!TSG) return;

	TSG->SavedTaskID = GetActiveTaskID();
	TSG->SavedCurrentObjectiveIndex = CurrentObjectiveIndex;
	TSG->SavedCurrentObjectiveCount = CurrentObjectiveCount;

	UGameplayStatics::SaveGameToSlot(TSG, SaveSlotName, UserIndex);
	UE_LOG(LogTemp, Log, TEXT("Task progress saved"));
}

void UT3DTaskSubsystem::LoadTaskProgress()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex)) return;

	
	USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex);
	UTaskSave* TSG = Cast<UTaskSave>(Loaded);
	if (!TSG) return;

	// You need to resolve SavedTaskID back to a UDataAsset (designer should ensure mission data assets are referenced somewhere).
	// For simplicity we'll search all loaded assets in memory (or you can maintain a registry).
	if (TSG->SavedTaskID != NAME_None)
	{
		// Simple resolver: iterate all UMissionData objects currently loaded (could be replaced with a registry)
		for (TObjectIterator<UT3DTaskData> It; It; ++It)
		{
			if (It->TaskID == TSG->SavedTaskID)
			{
				ActiveTask = *It;
				CurrentObjectiveIndex = TSG->SavedCurrentObjectiveIndex;
				CurrentObjectiveCount = TSG->SavedCurrentObjectiveCount;
				UE_LOG(LogTemp, Log, TEXT("Loaded task: %s idx:%d count:%d"),
					   *TSG->SavedTaskID.ToString(), CurrentObjectiveIndex, CurrentObjectiveCount);
				return;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Saved task id found but no matching TaskData loaded: %s"), *TSG->SavedTaskID.ToString());
	}
}


void UT3DTaskSubsystem::AdvanceObjectiveIfComplete()
{
	const FT3DTask& Obj = ActiveTask->Tasks[CurrentObjectiveIndex];
	if (CurrentObjectiveCount >= Obj.TargetCount)
	{
		UE_LOG(LogTemp, Log, TEXT("Task complete: %s"), *Obj.TaskName.ToString());

		// Move to next objective
		CurrentObjectiveIndex++;
		CurrentObjectiveCount = 0;

		if (CurrentObjectiveIndex >= ActiveTask->Tasks.Num())
		{
			CompleteTask();
		}
		else
		{
			// Next objective started
			UE_LOG(LogTemp, Log, TEXT("Next objective index: %d"), CurrentObjectiveIndex);
		}

		SaveTaskProgress();
	}
}

void UT3DTaskSubsystem::CompleteTask()
{
	UE_LOG(LogTemp, Log, TEXT("Mission Complete: %s"), *ActiveTask->TaskID.ToString());
	// handle reward, UI update, etc.
	ActiveTask = nullptr;
	CurrentObjectiveIndex = 0;
	CurrentObjectiveCount = 0;

	SaveTaskProgress();
}
