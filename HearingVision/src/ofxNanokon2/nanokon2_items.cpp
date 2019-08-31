#include "nanoKon2_items.h"

Nanokon2Items::Nanokon2Items() : 
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
