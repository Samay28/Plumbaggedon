// Fill out your copyright notice in the Description page of Project Settings.

#include "Valve.h"
// #include "Components/StaticMeshComponent.h"

// Sets default values
AValve::AValve()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AValve::BeginPlay()
{
	Super::BeginPlay();
	// ValveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ValveMesh"));

	// static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Content/Megascans/3D_Assets/Steering_Wheel_wcvodee/S_Steering_Wheel_wcvodee_lod3_Var1.uasset'"));
	// if (MeshAsset.Succeeded())
	// {
	// 	ValveMesh->SetStaticMesh(MeshAsset.Object);
	// }
	// ValveMesh->SetupAttachment(GetRootComponent());
}

// Called every frame
void AValve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
