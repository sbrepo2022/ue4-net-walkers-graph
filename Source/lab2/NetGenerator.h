// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>

#define MAX_WEIGHT 1000000.0f
/**
 * 
 */
class NetLine {
public:
	FVector Start;
	FVector End;
	NetLine(FVector Start, FVector End) : Start(Start), End(End) {}
};

class NetNode {
public:
	int num;
	int last_node;
	bool visited;
	float weight;

	NetNode() : num(-1), last_node(-1), visited(false), weight(MAX_WEIGHT) {}
	NetNode(int num) : num(num), last_node(num), visited(false), weight(MAX_WEIGHT) {}
};

class LAB2_API NetGenerator
{
	std::vector<FVector> NetPoints;
	std::vector<std::vector<int>> NetIndices;

	float distance(int p1, int p2) { return 1.0f; }

public:
	NetGenerator(float NetWidth, float NetHeight, int NetWidthPointCount, int NetHeightPointCount, float MaxOffsetX, float MaxOffsetY, float MaxOffsetZ, float AdditionLines, bool CrossWays);
	~NetGenerator();

	std::vector<NetLine> getLines();
	std::vector<FVector> getPoints();
	std::vector<NetLine> getPath(int start_ind, int end_ind);
};
