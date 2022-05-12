// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGenerator.h"
#include "Math/UnrealMathUtility.h"

NetGenerator::NetGenerator(float NetWidth, float NetHeight, int NetWidthPointCount, int NetHeightPointCount, float MaxOffsetX, float MaxOffsetY, float MaxOffsetZ, float AdditionLines, bool CrossWays)
{
	// generate net points coordinates
	for (int i = 0; i < NetHeightPointCount; i++) {
		for (int j = 0; j < NetWidthPointCount; j++) {
			float PointX = FMath::FRandRange(-MaxOffsetX, MaxOffsetX) + NetWidth / NetWidthPointCount * j;
			float PointY = FMath::FRandRange(-MaxOffsetY, MaxOffsetY) + NetHeight / NetHeightPointCount * i;
			float PointZ = FMath::FRandRange(-MaxOffsetZ, MaxOffsetZ);
			NetPoints.push_back(FVector(PointX, PointY, PointZ));
		}
	}

	// generate indices table
	NetIndices.resize(NetWidthPointCount * NetHeightPointCount);
	int cycle_end = 0;
	bool first = true;
	while (! cycle_end) {
		cycle_end = 1;
		for (int i = 0; i < NetPoints.size(); i++) {
			if (!NetIndices[i].size() && !first) {
				cycle_end = 0;
			}
			else {
				first = false;
				int x = i % NetWidthPointCount;
				int y = (int)(i / NetWidthPointCount);
				std::vector<int> directions, using_directions;

				/* Choose direction */
				if (x < NetWidthPointCount - 1 && ! NetIndices[y * NetWidthPointCount + x + 1].size()) {
					directions.push_back(1);
				}	// right
				if (x > 0 && ! NetIndices[y * NetWidthPointCount + x - 1].size()) {
					directions.push_back(2);
				}	// left
				if (y < NetHeightPointCount - 1 && ! NetIndices[(y + 1) * NetWidthPointCount + x].size()) {
					directions.push_back(3);
				}	// top
				if (y > 0 && ! NetIndices[(y - 1) * NetWidthPointCount + x].size()) {
					directions.push_back(4);
				}	// bottom
				if (x < NetWidthPointCount - 1 && y < NetHeightPointCount - 1 && ! NetIndices[(y + 1) * NetWidthPointCount + x + 1].size()) {
					directions.push_back(5);
				}	// right-bottom

				if (CrossWays)
					if (x > 0 && y < NetHeightPointCount - 1 && !NetIndices[(y + 1) * NetWidthPointCount + x - 1].size()) {
						directions.push_back(6);
					}	// left-bottom

				if (x > 0 && y > 0 && !NetIndices[(y - 1) * NetWidthPointCount + x - 1].size()) {
					directions.push_back(7);
				}	// left-top

				if (CrossWays)
					if (x < NetWidthPointCount - 1 && y > 0 && !NetIndices[(y - 1) * NetWidthPointCount + x + 1].size()) {
						directions.push_back(8);
					}	// right-top

				int main_direction = 0;
				if (directions.size()) {
					main_direction = directions[FMath::RandRange(0, directions.size() - 1)];
					using_directions.push_back(main_direction);
					directions.clear();
				}

				/* Add other directions */
				if (x < NetWidthPointCount - 1 && std::find(NetIndices[i].begin(), NetIndices[i].end(), y * NetWidthPointCount + x + 1) == NetIndices[i].end()) {
					directions.push_back(1);
				}	// right
				if (x > 0 && std::find(NetIndices[i].begin(), NetIndices[i].end(), y * NetWidthPointCount + x - 1) == NetIndices[i].end()) {
					directions.push_back(2);
				}	// left
				if (y < NetHeightPointCount - 1 && std::find(NetIndices[i].begin(), NetIndices[i].end(), (y + 1) * NetWidthPointCount + x) == NetIndices[i].end()) {
					directions.push_back(3);
				}	// top
				if (y > 0 && std::find(NetIndices[i].begin(), NetIndices[i].end(), (y - 1) * NetWidthPointCount + x) == NetIndices[i].end()) {
					directions.push_back(4);
				}	// bottom
				if (x < NetWidthPointCount - 1 && y < NetHeightPointCount - 1 && std::find(NetIndices[i].begin(), NetIndices[i].end(), (y + 1) * NetWidthPointCount + x + 1) == NetIndices[i].end()) {
					directions.push_back(5);
				}	// right-bottom

				if (CrossWays)
					if (x > 0 && y < NetHeightPointCount - 1 && std::find(NetIndices[i].begin(), NetIndices[i].end(), (y + 1) * NetWidthPointCount + x - 1) == NetIndices[i].end()) {
						directions.push_back(6);
					}	// left-bottom

				if (x > 0 && y > 0 && std::find(NetIndices[i].begin(), NetIndices[i].end(), (y - 1) * NetWidthPointCount + x - 1) == NetIndices[i].end()) {
					directions.push_back(7);
				}	// left-top

				if (CrossWays)
					if (x < NetWidthPointCount - 1 && y > 0 && std::find(NetIndices[i].begin(), NetIndices[i].end(), (y - 1) * NetWidthPointCount + x + 1) == NetIndices[i].end()) {
						directions.push_back(8);
					}	// right-top

				for (auto it : directions) {
					if (it != main_direction) {
						if (FMath::FRandRange(0.0f, 1.0f) < AdditionLines) {
							using_directions.push_back(it);
						}
					}
				}

				/* Bind points and move next point. If next point was bound before, end cycle. */
				for (auto dir : using_directions) {
					switch (dir) {
					case 1:
						NetIndices[i].push_back(y * NetWidthPointCount + x + 1);
						NetIndices[y * NetWidthPointCount + x + 1].push_back(i);
						break;

					case 2:
						NetIndices[i].push_back(y * NetWidthPointCount + x - 1);
						NetIndices[y * NetWidthPointCount + x - 1].push_back(i);
						break;

					case 3:
						NetIndices[i].push_back((y + 1) * NetWidthPointCount + x);
						NetIndices[(y + 1) * NetWidthPointCount + x].push_back(i);
						break;

					case 4:
						NetIndices[i].push_back((y - 1) * NetWidthPointCount + x);
						NetIndices[(y - 1) * NetWidthPointCount + x].push_back(i);
						break;

					case 5:
						NetIndices[i].push_back((y + 1) * NetWidthPointCount + x + 1);
						NetIndices[(y + 1) * NetWidthPointCount + x + 1].push_back(i);
						break;

					case 6:
						NetIndices[i].push_back((y + 1) * NetWidthPointCount + x - 1);
						NetIndices[(y + 1) * NetWidthPointCount + x - 1].push_back(i);
						break;

					case 7:
						NetIndices[i].push_back((y - 1) * NetWidthPointCount + x - 1);
						NetIndices[(y - 1) * NetWidthPointCount + x - 1].push_back(i);
						break;

					case 8:
						NetIndices[i].push_back((y - 1) * NetWidthPointCount + x + 1);
						NetIndices[(y - 1) * NetWidthPointCount + x + 1].push_back(i);
						break;
					}
				}
			}
		}
	}
}

NetGenerator::~NetGenerator()
{
}

std::vector<NetLine> NetGenerator::getLines()
{
	// generate lines by indices table
	std::vector<NetLine> LinesList;

	for (int i = 0; i < NetIndices.size(); i++) {
		for (auto it : NetIndices[i]) {
			if (it > i) LinesList.push_back(NetLine(NetPoints[i], NetPoints[it]));
		}
	}

	return LinesList;
}

std::vector<FVector> NetGenerator::getPoints()
{
	return NetPoints;
}

std::vector<NetLine> NetGenerator::getPath(int start_ind, int end_ind) {
	std::vector<NetLine> result;

	std::vector <NetNode> NetNodes;
	int not_visited = NetPoints.size();

	for (int i = 0; i < NetPoints.size(); i++) {
		NetNodes.push_back(NetNode(i));
	}
	NetNodes[start_ind].weight = 0.0f;

	while (not_visited > 0) {
		int min_ind = -1;
		float min_weight = MAX_WEIGHT;
		for (int i = 0; i < NetNodes.size(); i++) {
			if (! NetNodes[i].visited && NetNodes[i].weight < min_weight) {
				min_ind = i;
				min_weight = NetNodes[i].weight;
			}
		}

		for (auto it : NetIndices[min_ind]) {
			int dist = distance(min_ind, it);
			if (dist + min_weight < NetNodes[it].weight) {
				NetNodes[it].last_node = min_ind;
				NetNodes[it].weight = dist + min_weight;
			}
		}

		not_visited--;
		NetNodes[min_ind].visited = true;
	}

	int cur_ind = end_ind, prev_ind;
	while (cur_ind != start_ind) {
		prev_ind = NetNodes[cur_ind].last_node;
		result.insert(result.begin(), NetLine(NetPoints[prev_ind], NetPoints[cur_ind]));
		cur_ind = prev_ind;
	}

	return result;
}
