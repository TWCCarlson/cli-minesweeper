#pragma once
#include <map>
#include <string>

namespace mineDifficulty
{
	const std::map<int, std::string> DifficultyOptions = {
		{ -1, "Custom" },
		{ 0, "Beginnner" },
		{ 1, "Intermediate" },
		{ 2, "Expert" }
	};

	const enum class difficultyEnum {
		Custom = -1,
		Beginner,
		Intermediate,
		Expert
	};
	
	const std::map<difficultyEnum, int> HEIGHT = {
		{difficultyEnum::Beginner, 9},
		{difficultyEnum::Intermediate, 16},
		{difficultyEnum::Expert, 16}
	};

	const std::map<difficultyEnum, int> WIDTH = {
		{difficultyEnum::Beginner, 9},
		{difficultyEnum::Intermediate, 16},
		{difficultyEnum::Expert, 26}
	};

	const std::map<difficultyEnum, int> MINECOUNT = {
		{difficultyEnum::Beginner, 10},
		{difficultyEnum::Intermediate, 40},
		{difficultyEnum::Expert, 84}
	};

	struct setupValues {
		int boardHeight;
		int boardWidth;
		int mineCount;
	};
}