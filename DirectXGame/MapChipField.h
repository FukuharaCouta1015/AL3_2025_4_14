#pragma once
#include <cstdint>
#include "kamataEngine.h"
#include <vector>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data_;
};

class MapChipField {
public:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlocksHorizontal = 100;
	static inline const uint32_t kNumBlocksVertical = 20;

	uint32_t GetNumBlockHorizontal()const { return kNumBlocksHorizontal; }
	uint32_t GetNumBlockVertical() const { return kNumBlocksVertical; }

	void ResetMapChipData();

	void LodeMapChipCsv(const std::string& filePaht);
	
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	MapChipData mapChipData_;

private:

};