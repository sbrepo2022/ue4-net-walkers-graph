// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "GameFramework/Actor.h"
#include "NetGenerator.h"
#include "WalkerActor.h"
#include "NetActor.generated.h"

UCLASS()
class LAB2_API ANetActor : public AActor
{
    GENERATED_BODY()


private:
    UInstancedStaticMeshComponent* InstancedComponent;
    FVector bbox;
    NetGenerator* Generator;
    std::vector<AWalkerActor*> Walkers;

    std::vector<FVector> NetPoints;

    void generate();
    void createWalkers();
    void draw();
    
public:	
    // Sets default values for this actor's properties
    ANetActor();
    ~ANetActor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        float NetWidth = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        float NetHeight = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        int NetWidthPointCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        int NetHeightPointCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        float MaxOffsetX = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        float MaxOffsetY = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        float MaxOffsetZ = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        float AdditionLines = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetSettings")
        bool CrossWays = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetRenderSettings")
        float LineWidth = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetRenderSettings")
        float LineHeight = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walkers")
        int NumberOfWalkers = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walkers")
        int Velosity = 100.0f;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void PostRegisterAllComponents() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
