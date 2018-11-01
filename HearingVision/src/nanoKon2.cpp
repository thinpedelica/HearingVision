#include "NanoKon2.h"

//--------------------------------------------------------------
NanoKon2::NanoKon2() : 
    buttonTrackLeft(false), buttonTrackRight(false),
    buttonCycle(false),
    buttonMarkerSet(false), buttonMarkerLeft(false), buttonMarkerRight(false),
    buttonRewind(false), buttonFastForward(false),
    buttonStop(false), buttonPlay(false), buttonRecord(false)
{
    knobs.resize(kToggleNum, 0);
    sliders.resize(kToggleNum, 0);
    buttonsSolo.resize(kToggleNum, false);
    buttonsMute.resize(kToggleNum, false);
    buttonsRec.resize(kToggleNum, false);
}
//--------------------------------------------------------------
NanoKon2::~NanoKon2()
{
    exit();
}
//--------------------------------------------------------------
void NanoKon2::setup()
{
    midiIn.openPort(0);

    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);

    // add listener
    midiIn.addListener(this);

    // print received messages to the console
    midiIn.setVerbose(false);
}

//--------------------------------------------------------------
void NanoKon2::update()
{
    int message_control = midiMessage.control;

    if (isMyMessage(message_control, kKnobsStartIndex, kToggleNum))
    {
        sliders.at(message_control) = midiMessage.value;
    }
    else if (isMyMessage(message_control, kSlidersStartIndex, kToggleNum))
    {
        knobs.at(message_control - kSlidersStartIndex) = midiMessage.value;
    }
    else
    {
        bool bool_value = false;
        if (isMessageBool(midiMessage.value, bool_value))
        {
            if (isMyMessage(message_control, kButtonnsSoloStartIndex, kToggleNum))
            {
                buttonsSolo.at(message_control - kButtonnsSoloStartIndex) = bool_value;
            }
            else if (isMyMessage(message_control, kButtonsMuteStartIndex, kToggleNum))
            {
                buttonsMute.at(message_control - kButtonsMuteStartIndex) = bool_value;
            }
            else if (isMyMessage(message_control, kButtonsRecStartIndex, kToggleNum))
            {
                buttonsRec.at(message_control - kButtonsRecStartIndex) = bool_value;
            }
            else if (isMyMessage(message_control, kButtonTrackLeftIndex))
            {
                buttonTrackLeft = bool_value;
            }
            else if (isMyMessage(message_control, kButtonTrackRightIndex))
            {
                buttonTrackRight = bool_value;
            }
            else if (isMyMessage(message_control, kButtonCycleIndex))
            {
                buttonCycle = bool_value;
            }
            else if (isMyMessage(message_control, kButtonMarkerSetIndex))
            {
                buttonMarkerSet = bool_value;
            }
            else if (isMyMessage(message_control, kButtonMarkerLeftIndex))
            {
                buttonMarkerLeft = bool_value;
            }
            else if (isMyMessage(message_control, kButtonMarkerRightIndex))
            {
                buttonMarkerRight = bool_value;
            }
            else if (isMyMessage(message_control, kButtonRewindIndex))
            {
                buttonRewind = bool_value;
            }
            else if (isMyMessage(message_control, kButtonFastForwardIndex))
            {
                buttonFastForward = bool_value;
            }
            else if (isMyMessage(message_control, kButtonStopIndex))
            {
                buttonStop = bool_value;
            }
            else if (isMyMessage(message_control, kButtonPlayIndex))
            {
                buttonPlay = bool_value;
            }
            else if (isMyMessage(message_control, kButtonRecordIndex))
            {
                buttonRecord = bool_value;
            }
        }
    }
}

//--------------------------------------------------------------
void NanoKon2::exit()
{
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------
void NanoKon2::newMidiMessage(ofxMidiMessage& msg)
{
    midiMessage = msg;
}

//--------------------------------------------------------------
bool NanoKon2::isMyMessage(const int message_control,
                           const int my_control,
                           const int range) const
{
    if ((message_control >= my_control) &&
        (message_control <  my_control + range)) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
bool NanoKon2::isMyMessage(const int message_control,
                           const int my_control) const
{
    if (message_control == my_control) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
bool NanoKon2::isMessageBool(const int bool_value,
                             bool &value) const
{
    if (bool_value == 0) {
        value = false;
        return true;
    } else if (bool_value == 127) {
        value = true;
        return true;
    } else {
        return false;
    }
}
