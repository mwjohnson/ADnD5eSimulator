#include "Utils.h"
#include "character.h"
#include <cassert>

utils::utils() : rng_(std::mt19937(rd_())) {
	//rng_ = std::mt19937(rd_());
}

int utils::getRandomUniform(int min, int max)
{
	std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
	return uni(rng_);
}

/*
int utils::getRandomUniformStatic(int min, int max)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
	return uni(rng);
}*/

int utils::getRandomNormalCountdSides(int count, int sides)
{
	float singleMean = (1 + sides) / 2.0f;
	float one_over_sides = 1.0f / sides;
	float singleVar = 0;
	for (int i = 1; i <= sides; i++) {
		singleVar += pow((i - singleMean), 2)*one_over_sides;
	}
	float mean = count*singleMean;
	float var = count*singleVar;
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	float stddev = sqrt(var);
	std::normal_distribution<float> norm(mean, stddev);
	return static_cast<int>(std::round(norm(rng)));
}

int utils::getDistance(const character* self, const character* enemy)
{
	return  self->getPosition() - enemy->getPosition();
}

int utils::getProficiencyBonus(int level)
{
	switch (level)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		return 2;
		break;
	case 5:
	case 6:
	case 7:
	case 8:
		return 3;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
		return 4;
		break;
	case 13:
	case 14:
	case 15:
	case 16:
		return 5;
		break;
	case 17:
	case 18:
	case 19:
	case 20:
		return 6;
		break;
	default:
		break;
	}
	assert(false);
	return 0;
}
