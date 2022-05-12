// Fill out your copyright notice in the Description page of Project Settings.


#include "NetActor.h"
#include "Components/LineBatchComponent.h"

// Sets default values
ANetActor::ANetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LineBatch = CreateDefaultSubobject<ULineBatchComponent>(TEXT("Net"));
	LineBatch->SetupAttachment(RootComponent);
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
	LineBatch->Flush();
	std::vector<NetLine> LinesList = Generator->getLines();
	for (auto it : LinesList) {
		LineBatch->DrawLine(it.Start, it.End, FLinearColor(1, 0, 0), 0);
	}
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

