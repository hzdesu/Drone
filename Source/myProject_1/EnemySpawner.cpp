// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "Enemy.h"
#include <Engine/EngineTypes.h>
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
		RootComponent = SpawnArea;
		SpawnArea->SetBoxExtent(FVector(10000.0f, 10000.0f, 10000.0f));

}

void AEnemySpawner::SpawnEnemy()
{
	//������ɵ���λ��
	FVector Origin;
	FVector Extends;
	GetActorBounds(false, Origin, Extends);
	FVector SpawnPoint= UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extends);
	GetWorld()->SpawnActor<AEnemy>(EnemyType, SpawnPoint, FRotator::ZeroRotator);
}



// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	//����ʱ�����ɵ���
	FTimerHandle SpawnHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle,this,&AEnemySpawner::SpawnEnemy,spawnSpan,true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

