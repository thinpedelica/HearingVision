#include "nanokon2_osc.h"

const std::string Nanokon2osc::kLabelNanokon2          = "/Nanokon2";
const std::string Nanokon2osc::kLabelKnobs             = Nanokon2osc::kLabelNanokon2 + "/Knobs";
const std::string Nanokon2osc::kLabelSliders           = Nanokon2osc::kLabelNanokon2 + "/Sliders";
const std::string Nanokon2osc::kLabelButttonsSolo      = Nanokon2osc::kLabelNanokon2 + "/ButttonsSolo";
const std::string Nanokon2osc::kLabelButttonsMute      = Nanokon2osc::kLabelNanokon2 + "/ButttonsMute";
const std::string Nanokon2osc::kLabelButttonsRec       = Nanokon2osc::kLabelNanokon2 + "/ButttonsRec";
const std::string Nanokon2osc::kLabelButtonTrackLeft   = Nanokon2osc::kLabelNanokon2 + "/ButtonTrackLeft";
const std::string Nanokon2osc::kLabelButtonTrackRight  = Nanokon2osc::kLabelNanokon2 + "/ButtonTrackRight";
const std::string Nanokon2osc::kLabelButtonCycle       = Nanokon2osc::kLabelNanokon2 + "/ButtonCycle";
const std::string Nanokon2osc::kLabelButtonMarkerSet   = Nanokon2osc::kLabelNanokon2 + "/ButtonMarkerSet";
const std::string Nanokon2osc::kLabelButtonMarkerLeft  = Nanokon2osc::kLabelNanokon2 + "/ButtonMarkerLeft";
const std::string Nanokon2osc::kLabelButtonMarkerRight = Nanokon2osc::kLabelNanokon2 + "/ButtonMarkerRight";
const std::string Nanokon2osc::kLabelButtonRewind      = Nanokon2osc::kLabelNanokon2 + "/ButtonRewind";
const std::string Nanokon2osc::kLabelButtonFastForward = Nanokon2osc::kLabelNanokon2 + "/ButtonFastForward";
const std::string Nanokon2osc::kLabelButtonStop        = Nanokon2osc::kLabelNanokon2 + "/ButtonStop";
const std::string Nanokon2osc::kLabelButtonPlay        = Nanokon2osc::kLabelNanokon2 + "/ButtonPlay";
const std::string Nanokon2osc::kLabelButtonRecord      = Nanokon2osc::kLabelNanokon2 + "/ButtonRecord";

//--------------------------------------------------------------
Nanokon2osc::~Nanokon2osc()
{
    exit();
}

//--------------------------------------------------------------
void Nanokon2osc::setup()
{
    receiver.setup(kListenPort);
}

//--------------------------------------------------------------
void Nanokon2osc::update(Nanokon2Items& items)
{
    updateOscMessage(items);
}

//--------------------------------------------------------------
void Nanokon2osc::updateOscMessage(Nanokon2Items& items)
{
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        receiver.getNextMessage(msg);
        getValue(msg, items);
    }
}

//--------------------------------------------------------------
void Nanokon2osc::getValue(const ofxOscMessage& msg,
                           Nanokon2Items& items)
{
    std::string addr = msg.getAddress();
    if (addr == kLabelKnobs) {
        getValue(msg, items.knobs);
    } else if (addr == kLabelSliders) {
        getValue(msg, items.sliders);
    } else if (addr == kLabelButttonsSolo) {
        getValue(msg, items.buttonsSolo);
    } else if (addr == kLabelButttonsMute) {
        getValue(msg, items.buttonsMute);
    } else if (addr == kLabelButttonsRec) {
        getValue(msg, items.buttonsRec);
    } else if (addr == kLabelButtonTrackLeft) {
        getValue(msg, items.buttonTrackLeft);
    } else if (addr == kLabelButtonTrackRight) {
        getValue(msg, items.buttonTrackRight);
    } else if (addr == kLabelButtonCycle) {
        getValue(msg, items.buttonCycle);
    } else if (addr == kLabelButtonMarkerSet) {
        getValue(msg, items.buttonMarkerSet);
    } else if (addr == kLabelButtonMarkerLeft) {
        getValue(msg, items.buttonMarkerLeft);
    } else if (addr == kLabelButtonMarkerRight) {
        getValue(msg, items.buttonMarkerRight);
    } else if (addr == kLabelButtonRewind) {
        getValue(msg, items.buttonRewind);
    } else if (addr == kLabelButtonFastForward) {
        getValue(msg, items.buttonFastForward);
    } else if (addr == kLabelButtonStop) {
        getValue(msg, items.buttonStop);
    } else if (addr == kLabelButtonPlay) {
        getValue(msg, items.buttonPlay);
    } else if (addr == kLabelButtonRecord) {
        getValue(msg, items.buttonRecord);
    } else {
        std::cout << "invalid osc msg: " << addr << std::endl;
    }
}


//--------------------------------------------------------------
void Nanokon2osc::getValue(const ofxOscMessage& msg,
                           std::vector<int>& item)
{
    int index = msg.getArgAsInt32(0);
    item.at(index) = msg.getArgAsInt32(1); 
}

//--------------------------------------------------------------
void Nanokon2osc::getValue(const ofxOscMessage& msg,
                           std::vector<bool>& item)
{
    int index = msg.getArgAsInt32(0);
    item.at(index) = getBoolValue(msg.getArgAsInt32(1));
}

//--------------------------------------------------------------
void Nanokon2osc::getValue(const ofxOscMessage& msg,
                           bool& item)
{
    item = getBoolValue(msg.getArgAsInt32(0));
}

//--------------------------------------------------------------
void Nanokon2osc::exit()
{
    // nop
}

//--------------------------------------------------------------
bool Nanokon2osc::getBoolValue(const int32_t value)
{
    if (value == 127) {
        return true;
    } else {
        return false;
    }
}
