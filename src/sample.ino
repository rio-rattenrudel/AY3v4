void doSample()
{
    // ignore other samples then aymid ones
    if (aymidState.enabled) return;

    for (byte channel = 1; channel < 7; channel++) {

        // limit range of pitch
        if      (destiPitch[channel] > 3822)    destiPitch[channel] = 3822;
        else if (destiPitch[channel] < 0)       destiPitch[channel] = 0;

        // proceed pitch/glide
        if (base[channel] != 0) {

            byte steps = 64 - (glide >> 2);

            if (destiPitch[channel] + bend[channel] > pitch[channel]) {
                pitch[channel] += steps;
                if (pitch[channel] > destiPitch[channel] + bend[channel])
                    pitch[channel] = destiPitch[channel] + bend[channel];

            } else if (destiPitch[channel] + bend[channel] < pitch[channel]) {
                pitch[channel] -= steps;
                if (pitch[channel] < destiPitch[channel] + bend[channel])
                    pitch[channel] = destiPitch[channel] + bend[channel];
            }

            if (chord == 8) pitch[channel] = destiPitch[channel] + bend[channel];
        }
    }

    //
    // update register changes
    //

    updatePitches();
}