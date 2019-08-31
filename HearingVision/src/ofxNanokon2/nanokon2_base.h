#pragma once

#include "ofMain.h"
#include "nanokon2_items.h"

class Nanokon2Base {
	
public:
	Nanokon2Base() = default;
	virtual ~Nanokon2Base() = default;
	virtual void setup() = 0;
	virtual void update(Nanokon2Items& items) = 0;
	virtual void exit() = 0;
};
