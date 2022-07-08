// Fill out your copyright notice in the Description page of Project Settings.


#include "NetActor.h"
#include "Components/LineBatchComponent.h"

// Sets default values
ANetActor::ANetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Net"));
	SetRootComponent(InstancedComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (CubeVisualAsset.Succeeded())
	{
		InstancedComponent->SetStaticMesh(CubeVisualAsset.Object);
		InstancedComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		bbox = CubeVisualAsset.Object->GetBoundingBox().GetSize();
	}
}

ANetActor::~ANetActor() {
	if (Generator != nullptr)
		delete Generator;
}

void ANetActor::generate() {
	if (Generator != nullptr)
		delete Generator;
	Generator = new NetGenerator(NetWidth, NetHeight, NetWidthPointCount, NetHeightPointCount, MaxOffsetX, MaxOffsetY, MaxOffsetZ, AdditionLines, CrossWays);
}

void ANetActor::createWalkers() {
	NetPoints = Generator->getPoints();

	AWalkerActor* walk;
	FVector point(0, 0, 0);
	int point_ind;
	for (int i = 0; i < NumberOfWalkers; i++) {
		point_ind = FMath::RandRange(0, NetPoints.size() - 1);
		point = NetPoints[point_ind];

		FActorSpawnParameters params;
		params.Owner = this;
		walk = GetWorld()->SpawnActor<AWalkerActor>(params);

		walk->SetActorLocation(point);
		walk->setStartPoint(point_ind);
		walk->setLineColor(FLinearColor::MakeFromHSV8((int)(255 * (float)(NumberOfWalkers - i) / (float)(NumberOfWalkers + 1)), 255, 255));
		walk->setVelosity(Velosity);
		walk->setGenerator(Generator);
		Walkers.push_back(walk);
	}
}

void ANetActor::draw() {
	InstancedComponent->ClearInstances();
	std::vector<NetLine> LinesList = Generator->getLines();
	for (auto it : LinesList) {
		FVector vec = it.End - it.Start;
		float len = vec.Size();
		FTransform t_origin, t_scale, t_rotate, t_translate, tr;
		t_origin.SetTranslation(FVector(bbox.X / 2, 0, 0));
		t_scale.SetScale3D(FVector(len / bbox.X, LineWidth / bbox.Y, LineHeight / bbox.Z));
		t_rotate.SetRotation(FQuat::FindBetweenVectors(FVector(1, 0, 0), vec));
		t_translate.SetTranslation(it.Start);

		tr = t_origin * t_scale * t_rotate * t_translate;
		InstancedComponent->AddInstance(tr);
	}
	/*LineBatch->Flush();
	std::vector<NetLine> LinesList = Generator->getLines();
	for (auto it : LinesList) {
		LineBatch->DrawLine(it.Start, it.End, FLinearColor(1, 0, 0), 0);
	}*/
}

void ANetActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

	generate();
	draw();
}

// Called when the game starts or when spawned
void ANetActor::BeginPlay()
{
	Super::BeginPlay();

	generate();
	createWalkers();
	draw();
}

// Called every frame
void ANetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

