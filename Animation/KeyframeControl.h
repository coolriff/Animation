#pragma once
#include "Keyframe.h"

class KeyframeControl
{
public:
	KeyframeControl(void);
	~KeyframeControl(void);

	Keyframe* keyframe[15];

	void penaltyAction_1(float dt);
};

