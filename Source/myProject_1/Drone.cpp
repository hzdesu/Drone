// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����OutCollision
	OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	//����OutCollision��RootComponent��
	RootComponent = OutCollision;
	//���ô�С
	OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	//������ײ
	OutCollision->SetCollisionProfileName(TEXT("Pawn"));
	//��������
	OutCollision->SetSimulatePhysics(true);
	//����ˮƽ
	OutCollision->BodyInstance.bLockXRotation = true;
	OutCollision->BodyInstance.bLockYRotation = true;

	//����Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh���õ�OutCollision��
	Mesh->SetupAttachment(OutCollision);

	//����Paddle
	Paddle1=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));

	//��Paddle������Mesh��
	Paddle1->SetupAttachment(Mesh, TEXT("Paddle1"));
	Paddle2->SetupAttachment(Mesh, TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh, TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh, TEXT("Paddle4"));

	//��paddleװ��������
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);

	//�������ϵ�����
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThurster"));
	//������RootComponent��
	UpThruster->SetupAttachment(RootComponent);
	//���ó�ʼ����
	UpThruster->ThrustStrength = 980.0f;
	//�Զ�����
	UpThruster->SetAutoActivate(true);
	//�������ķ���
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));

	//������ǰ������
	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThurster"));
	//������RootComponent��
	ForwardThruster->SetupAttachment(RootComponent);
	//���ó�ʼ����
	ForwardThruster->ThrustStrength = 0.0f;
	//�Զ�����
	ForwardThruster->SetAutoActivate(true);
	//�������ķ���
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));

}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//����������
	if (GetInputAxisValue(TEXT("Lift")) == 0.0f)
	{
		UpThruster->ThrustStrength = 980.0f;
	}
	if (GetInputAxisValue(TEXT("Forward")) == 0.0f)
	{
		ForwardThruster->ThrustStrength = 0.0f;

		if (Mesh->GetRelativeRotation().Pitch != 0.0f)
		{
			float currentPitch = Mesh->GetRelativeRotation().Pitch;
			Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime*2.0f, 0.0f, 0.0f));
			if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
			{
				Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
			}
		}
	}
	//���÷ɽ���ת
	RotatePaddle(DeltaTime);

}

// Called to bind functionality to input
//���������
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//��������Axis  һ������Action
	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed,this, &ADrone::DoFire);
}

//����
void ADrone::Lift(float val)
{
	UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;   	//��Ҫ���ǵ�֡��
	UpThruster->ThrustStrength=FMath::Clamp(UpThruster->ThrustStrength, -LiftThrustMax, LiftThrustMax);
}

//
void ADrone::Forward(float val)
{
	ForwardThruster->ThrustStrength += val * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrustMax, ForwardThrustMax);

	/*float pitch = Mesh->GetRelativeRotation().Pitch;
	if (FMath::Abs(pitch) < 30.0f)
	{
		Mesh->AddRelativeRotation(FRotator(100.0f * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
	}*/
	float pitch = Mesh->GetRelativeRotation().Pitch;
	float valpitch = -val * pitch;
	if (valpitch < 30.0f)
	{
		Mesh->AddRelativeRotation(FRotator(-val * 50.0f * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
	}


}

void ADrone::Turn(float val)
{
	OutCollision->AddTorqueInDegrees(this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::DoFire()
{
	//�����ӵ�
	FTransform firingPoint=Mesh->GetSocketTransform(TEXT("FiringSocket"));
		GetWorld()->SpawnActor<AMissle>(Bullet,firingPoint );
}

void ADrone::RotatePaddle(float Delta)
{
	//���������ѭ������
	for (auto paddle : Paddles)
	{
		paddle->AddRelativeRotation(FRotator(.0f, Delta * PaddleRotateSpeed, .0f));
	}
}

