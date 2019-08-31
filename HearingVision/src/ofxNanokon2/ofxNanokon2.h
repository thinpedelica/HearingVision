#pragma once

#include "nanokon2_base.h"
#include "nanokon2_items.h"

#include "ofMain.h"
#include <memory>
#include <vector>

class ofxNanokon2 {
public:
	ofxNanokon2() = default;
	~ofxNanokon2();
	void setup(const bool use_osc = false);
	void update();
	void exit();
	
	Nanokon2Items nanokon2_items;

private:
	std::unique_ptr<Nanokon2Base> nanokon2_ptr;
};
