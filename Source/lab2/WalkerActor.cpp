// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkerActor.h"
#include "Components/LineBatchComponent.h"

// Sets default values
AWalkerActor::AWalkerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LineBatch = CreateDefaultSubobject<ULineBatchComponent>(TEXT("Path"));
	LineBatch->SetupAttachment(RootComponent);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(VisualMesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

    if (CubeVisualAsset.Succeeded())
    {
        VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
        VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }

	path_created = false;
	velosity = 100.0f;
	Generator = nullptr;
}

// Called when the game starts or when spawned
void AWalkerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWalkerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Generator != nullptr) {
		if (! path_created) {
			start_point = end_point;
			createPath();
		}
	}

	if (path_created) {
		FVector dir = next_pos - cur_pos;
		dir.Normalize();
		FVector new_loc = GetActorLocation() + dir * velosity * DeltaTime;
		SetActorLocation(new_loc);

		if (FVector::DistSquared(new_loc, cur_pos) > FVector::DistSquared(next_pos, cur_pos)) {
			SetActorLocation(next_pos);
			cur_line_index++;
			if (cur_line_index >= LinesList.size()) {
				path_created = false;
			}
			else {
				cur_pos = LinesList[cur_line_index].Start;
				next_pos = LinesList[cur_line_index].End;
			}
		}
	}
}

void AWalkerActor::setGenerator(NetGenerator* gen) {
	Generator = gen;
	createPath();
}

void AWalkerActor::createPath() {
	path_created = true;

	std::vector<FVector> NetPoints = Generator->getPoints();
	do {
		end_point = FMath::RandRange(0, NetPoints.size() - 1);
	} while (end_point == start_point);

	LineBatch->Flush();
	LinesList = Generator->getPath(start_point, end_point);
	cur_line_index = 0;
	//UE_LOG(LogTemp, Warning, TEXT("LinesList size is: %d"), LinesList.size());
	cur_pos = LinesList[cur_line_index].Start;
	next_pos = LinesList[cur_line_index].End;
	for (auto it : LinesList) {
		LineBatch->DrawLine(it.Start, it.End, line_color, 1);
	}
}

