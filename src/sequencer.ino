void doSequencer()
{
    // ignore aymid
    if (aymidState.enabled) return;

    // internal
    if (!clockSynced) {

        // count
        if (seqSetup == 1) seqcc++;

        // process
        if (seqcc > seqSpeed) processMixerAndSteps();
    }
}

void doSequencerClock()
{
    // count
    if (seqSetup == 1) seqcc++;

    // process
    if (seqcc >= clockSpeeds[seqSpeed ? seqSpeed-1 : 0]) processMixerAndSteps();
}