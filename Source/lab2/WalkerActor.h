// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "NetGenerator.h"
#include "GameFramework/Actor.h"
#include "WalkerActor.generated.h"

UCLASS()
class LAB2_API AWalkerActor : public AActor
{
	GENERATED_BODY()


private:
	NetGenerator* Generator;
	ULineBatchComponent* LineBatch;
	std::vector<NetLine> LinesList;
	int start_point, end_point;

	int path_created;
	int cur_line_index;
	FVector cur_pos, next_pos;
	float velosity;

	FLinearColor line_color;

	void createPath();
	
public:	
	// Sets default values for this actor's properties
	AWalkerActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WalkerSetings")
	UStaticMeshComponent* VisualMesh;

	void setGenerator(NetGenerator* gen);
	void setStartPoint(int point_ind) { start_point = point_ind; }
	void setEndPoint(int point_ind) { end_point = point_ind; }
	void setLineColor(const FLinearColor &lc) { line_color = lc; }
	void setVelosity(int vel) { velosity = vel; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
