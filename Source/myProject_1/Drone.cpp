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

	//定义OutCollision
	OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	//关联OutCollision到RootComponent上
	RootComponent = OutCollision;
	//设置大小
	OutCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	//设置碰撞
	OutCollision->SetCollisionProfileName(TEXT("Pawn"));
	//启用物理
	OutCollision->SetSimulatePhysics(true);
	//锁定水平
	OutCollision->BodyInstance.bLockXRotation = true;
	OutCollision->BodyInstance.bLockYRotation = true;

	//定义Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh设置到OutCollision上
	Mesh->SetupAttachment(OutCollision);

	//定义Paddle
	Paddle1=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));

	//把Paddle关联到Mesh上
	Paddle1->SetupAttachment(Mesh, TEXT("Paddle1"));
	Paddle2->SetupAttachment(Mesh, TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh, TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh, TEXT("Paddle4"));

	//把paddle装进数组里
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);

	//定义向上的推力
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThurster"));
	//关联到RootComponent上
	UpThruster->SetupAttachment(RootComponent);
	//设置初始的力
	UpThruster->ThrustStrength = 980.0f;
	//自动开启
	UpThruster->SetAutoActivate(true);
	//设置力的方向
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));

	//定义向前的推力
	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThurster"));
	//关联到RootComponent上
	ForwardThruster->SetupAttachment(RootComponent);
	//设置初始的力
	ForwardThruster->ThrustStrength = 0.0f;
	//自动开启
	ForwardThruster->SetAutoActivate(true);
	//设置力的方向
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
	//检测键盘输入
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
	//调用飞桨旋转
	RotatePaddle(DeltaTime);

}

// Called to bind functionality to input
//设置输入绑定
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//轴向性用Axis  一次性用Action
	PlayerInputComponent->BindAxis(TEXT("Lift"), this, &ADrone::Lift);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADrone::Turn);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed,this, &ADrone::DoFire);
}

//升降
void ADrone::Lift(float val)
{
	UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;   	//需要考虑到帧率
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
	//生成子弹
	FTransform firingPoint=Mesh->GetSocketTransform(TEXT("FiringSocket"));
		GetWorld()->SpawnActor<AMissle>(Bullet,firingPoint );
}

void ADrone::RotatePaddle(float Delta)
{
	//对数组进行循环操作
	for (auto paddle : Paddles)
	{
		paddle->AddRelativeRotation(FRotator(.0f, Delta * PaddleRotateSpeed, .0f));
	}
}

