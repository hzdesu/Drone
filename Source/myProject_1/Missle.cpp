// Fill out your copyright notice in the Description page of Project Settings.


#include "Missle.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include "myProject_1GameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values  ��ʼ��
AMissle::AMissle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	//���ô����¼�
	Mesh->SetGenerateOverlapEvents(true);
	//����һ��ί�в�Ϊ�����¼�
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMissle::OverlapHandler);


	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	
	//��������ʼ�ٶ�
	MovementComp->InitialSpeed = 3000.0f;

	InitialLifeSpan = 5.0f;

}

void AMissle::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�жϵ������еĶԷ��Ƿ�̳���AEnemy
	if (OtherActor->IsA<AEnemy>())
	{
		OtherActor->Destroy();
		//Ҫ��ȡgamemode����Ҫ��ȡ����
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
	//��������������  == InitialLifeSpan
	//this->SetLifeSpan(5.0f);

	//�ӵ������ȥ�����Ч
	UGameplayStatics::PlaySound2D(this, MissleSound,0.1f);

}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

