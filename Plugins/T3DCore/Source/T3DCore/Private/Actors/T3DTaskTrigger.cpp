// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/T3DTaskTrigger.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/T3DTaskSubsystem.h"


// Sets default values
AT3DTaskTrigger::AT3DTaskTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	UBoxComponent* Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetBoxExtent(FVector(200));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AT3DTaskTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AT3DTaskTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AT3DTaskTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// simple player check
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor != Pawn) return;

	if (UT3DTaskSubsystem* TS = GetGameInstance()->GetSubsystem<UT3DTaskSubsystem>())
	{
		TS->StartTask(TaskToStart);
	}
}


