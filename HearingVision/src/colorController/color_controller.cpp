#include "color_controller.h"

const std::string ColorController::kLabelColorR = "/Color/R";
const std::string ColorController::kLabelColorG = "/Color/G";
const std::string ColorController::kLabelColorB = "/Color/B";

//--------------------------------------------------------------
ColorController::~ColorController() {
    exit();
}

//--------------------------------------------------------------
void ColorController::setup()
{
    receiver.setup(kListenPort);
}

//--------------------------------------------------------------
void ColorController::update()
{
    updateOscMessage();
}

//--------------------------------------------------------------
void ColorController::updateOscMessage()
{
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        receiver.getNextMessage(msg);

        std::string addr = msg.getAddress();
        if (addr == kLabelColorR) {
            color_.r = static_cast<int>(msg.getArgAsFloat(0));
        } else if (addr == kLabelColorG) {
            color_.g = static_cast<int>(msg.getArgAsFloat(0));
        } else if (addr == kLabelColorB) {
            color_.b = static_cast<int>(msg.getArgAsFloat(0));
        }
    }
}

//--------------------------------------------------------------
ofColor ColorController::get() const {
    return color_;
}

//--------------------------------------------------------------
void ColorController::exit()
{
    // nop
}
