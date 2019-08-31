#include "ofxNanokon2.h"
#include "nanokon2_midi.h"
#include "nanokon2_osc.h"

//--------------------------------------------------------------
ofxNanokon2::~ofxNanokon2() {
    exit();
}

//--------------------------------------------------------------
void ofxNanokon2::setup(const bool use_osc)
{
    if (use_osc) {
        nanokon2_ptr = std::make_unique<Nanokon2osc>();
    } else {
        nanokon2_ptr = std::make_unique<Nanokon2midi>();
    }

    nanokon2_ptr->setup();
}

//--------------------------------------------------------------
void ofxNanokon2::update()
{
    nanokon2_ptr->update(nanokon2_items);
}

//--------------------------------------------------------------
void ofxNanokon2::exit()
{
    nanokon2_ptr->exit();
}
