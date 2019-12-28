#include "param_controller.h"

const std::string ParamController::kLabelButton = "/Param/button";
const std::string ParamController::kLabelKnob   = "/Param/knob";


//--------------------------------------------------------------
ParamController::Param::Param() :
    level(0.f),
    speed(0.f),
    gain(0.f),
    mode(false),
    reset(false) {
    // nop
}

//--------------------------------------------------------------
ParamController::ParamController() {
    params_.resize(2);
}

//--------------------------------------------------------------
ParamController::~ParamController() {
    exit();
}

//--------------------------------------------------------------
void ParamController::setup() {
    receiver.setup(kListenPort);
}

//--------------------------------------------------------------
void ParamController::update() {
    updateOscMessage();
}

//--------------------------------------------------------------
void ParamController::updateOscMessage() {
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        receiver.getNextMessage(msg);

        std::string addr = msg.getAddress();

        if (addr == kLabelButton) {
            getValueButton(msg, params_);
        } else if (addr == kLabelKnob) {
            getValueKnob(msg, params_);
        }
    }
}

//--------------------------------------------------------------
ParamController::Param ParamController::get(const size_t index) const {
    if (index >= params_.size()) {
        return Param();
    } else {
        return params_.at(index);
    }
}

//--------------------------------------------------------------
void ParamController::exit() {
    // nop
}

//--------------------------------------------------------------
void ParamController::getValueKnob(const ofxOscMessage& msg,
                                   std::vector<Param>& params) {
    int index   = msg.getArgAsInt32(0);
    int item_no = msg.getArgAsInt32(1);
    float value = msg.getArgAsFloat(2);

    switch (item_no) {
        case 0: 
            params.at(index).level = value;
            break;
        case 1: 
            params.at(index).speed = value;
            break;
        case 2: 
            params.at(index).gain = value;
            break;
        default:
            break;
    } 
}

//--------------------------------------------------------------
void ParamController::getValueButton(const ofxOscMessage& msg,
                                     std::vector<Param>& params) {
    int index   = msg.getArgAsInt32(0);
    int item_no = msg.getArgAsInt32(1);
    float value = msg.getArgAsFloat(2);

    bool bool_value = false;
    if (value > 0.5) {
        bool_value = true;
    }

    switch (item_no) {
        case 0: 
            params.at(index).mode = bool_value;
            break;
        case 1: 
            params.at(index).reset = bool_value;
            break;
        default:
            break;
    } 
}