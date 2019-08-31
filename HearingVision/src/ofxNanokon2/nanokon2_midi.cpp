#include "nanokon2_midi.h"

//--------------------------------------------------------------
Nanokon2midi::~Nanokon2midi()
{
    exit();
}
//--------------------------------------------------------------
void Nanokon2midi::setup()
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
void Nanokon2midi::update(Nanokon2Items& items)
{
    int message_control = midiMessage.control;

    if (isMyMessage(message_control, kKnobsStartIndex, Nanokon2Items::kToggleNum))
    {
        items.sliders.at(message_control) = midiMessage.value;
    }
    else if (isMyMessage(message_control, kSlidersStartIndex, Nanokon2Items::kToggleNum))
    {
        items.knobs.at(message_control - kSlidersStartIndex) = midiMessage.value;
    }
    else
    {
        bool bool_value = false;
        if (isMessageBool(midiMessage.value, bool_value))
        {
            if (isMyMessage(message_control, kButtonnsSoloStartIndex, Nanokon2Items::kToggleNum))
            {
                items.buttonsSolo.at(message_control - kButtonnsSoloStartIndex) = bool_value;
            }
            else if (isMyMessage(message_control, kButtonsMuteStartIndex, Nanokon2Items::kToggleNum))
            {
                items.buttonsMute.at(message_control - kButtonsMuteStartIndex) = bool_value;
            }
            else if (isMyMessage(message_control, kButtonsRecStartIndex, Nanokon2Items::kToggleNum))
            {
                items.buttonsRec.at(message_control - kButtonsRecStartIndex) = bool_value;
            }
            else if (isMyMessage(message_control, kButtonTrackLeftIndex))
            {
                items.buttonTrackLeft = bool_value;
            }
            else if (isMyMessage(message_control, kButtonTrackRightIndex))
            {
                items.buttonTrackRight = bool_value;
            }
            else if (isMyMessage(message_control, kButtonCycleIndex))
            {
                items.buttonCycle = bool_value;
            }
            else if (isMyMessage(message_control, kButtonMarkerSetIndex))
            {
                items.buttonMarkerSet = bool_value;
            }
            else if (isMyMessage(message_control, kButtonMarkerLeftIndex))
            {
                items.buttonMarkerLeft = bool_value;
            }
            else if (isMyMessage(message_control, kButtonMarkerRightIndex))
            {
                items.buttonMarkerRight = bool_value;
            }
            else if (isMyMessage(message_control, kButtonRewindIndex))
            {
                items.buttonRewind = bool_value;
            }
            else if (isMyMessage(message_control, kButtonFastForwardIndex))
            {
                items.buttonFastForward = bool_value;
            }
            else if (isMyMessage(message_control, kButtonStopIndex))
            {
                items.buttonStop = bool_value;
            }
            else if (isMyMessage(message_control, kButtonPlayIndex))
            {
                items.buttonPlay = bool_value;
            }
            else if (isMyMessage(message_control, kButtonRecordIndex))
            {
                items.buttonRecord = bool_value;
            }
        }
    }
}

//--------------------------------------------------------------
void Nanokon2midi::exit()
{
    midiIn.closePort();
    midiIn.removeListener(this);
}

//--------------------------------------------------------------
void Nanokon2midi::newMidiMessage(ofxMidiMessage& msg)
{
    midiMessage = msg;
}

//--------------------------------------------------------------
bool Nanokon2midi::isMyMessage(const int message_control,
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
bool Nanokon2midi::isMyMessage(const int message_control,
                           const int my_control) const
{
    if (message_control == my_control) {
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
bool Nanokon2midi::isMessageBool(const int bool_value,
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
