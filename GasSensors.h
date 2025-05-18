#pragma once
#include "file.h"
class GasSensors
{
public:
	double rightLevels;
	double leftLevels;
	double range = 100;//only for example!

	GasSensors() {
		rightLevels = 0;
		leftLevels = 0;
	}
	double getRightLevels() {
		return this->rightLevels;

	}
	double getLeftLevels() {
		return this->leftLevels;
	}
	void setRightLevels(double level) {
		this->rightLevels = level;
	}
	void setLeftLevels(double level) {
		this->leftLevels = level;
	}
};
