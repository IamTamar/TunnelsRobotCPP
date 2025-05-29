#pragma once
class SoundSensors
{

	public:
		float rightLevels;
		float leftLevels;
		float range = 100;//only for example!

		SoundSensors() {
			rightLevels = 0;
			leftLevels = 0;
		}
		float getRightLevels() {
			return this->rightLevels;

		}
		float getLeftLevels() {
			return this->leftLevels;
		}
		void setRightLevels(float level) {
			this->rightLevels = level;
		}
		void setLeftLevels(float level) {
			this->leftLevels = level;
		}
	
};


