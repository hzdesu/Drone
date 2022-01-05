// Fill out your copyright notice in the Description page of Project Settings.


#include "Missle.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include "myProject_1GameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values  初始化
AMissle::AMissle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	//启用穿插事件
	Mesh->SetGenerateOverlapEvents(true);
	//声明一个委托并为它绑定事件
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMissle::OverlapHandler);


	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	
	//给导弹初始速度
	MovementComp->InitialSpeed = 3000.0f;

	InitialLifeSpan = 5.0f;

}

void AMissle::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//判断导弹击中的对方是否继承于AEnemy
	if (OtherActor->IsA<AEnemy>())
	{
		OtherActor->Destroy();
		//要获取gamemode，先要获取场景
		AmyProject_1GameModeBase*gm=Cast<AmyProject_1GameModeBase>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->points++;
		}

		this->Destroy();
	}
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();
	//给导弹生命周期  == InitialLifeSpan
	//this->SetLifeSpan(5.0f);

	//子弹发射出去后的音效
	UGameplayStatics::PlaySound2D(this, MissleSound,0.1f);

}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

