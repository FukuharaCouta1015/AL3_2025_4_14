#include "MapChipField.h"
#include "kamataEngine.h"
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

void MapChipField::ResetMapChipData() {

	mapChipData_.data_.clear();
	mapChipData_.data_.resize(kNumBlocksVertical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data_) {
		mapChipDataLine.resize(kNumBlocksHorizontal);
	}
}

void MapChipField::LodeMapChipCsv(const std::string& filePaht) {
	// マップチップのデータリセット
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePaht);
	assert(file.is_open());
	// マップチップcsv
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	// csvからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlocksVertical; ++i) {
		std::string line;
		std::getline(mapChipCsv, line);
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlocksHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data_[i][j] = mapChipTable[word];

			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) { 
	if (xIndex <0 ||kNumBlocksHorizontal - 1< xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlocksVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data_[yIndex][xIndex];
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return KamataEngine::Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlocksVertical - 1 - yIndex), 0);
}
