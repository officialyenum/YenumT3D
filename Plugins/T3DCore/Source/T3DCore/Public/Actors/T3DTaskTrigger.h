// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/T3DTaskData.h"
#include "GameFramework/Actor.h"
#include "T3DTaskTrigger.generated.h"

UCLASS()
class T3DCORE_API AT3DTaskTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AT3DTaskTrigger();

	UPROPERTY(EditAnywhere)
	UT3DTaskData* TaskToStart;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
