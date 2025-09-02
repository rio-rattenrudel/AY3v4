void doPots()
{
    analogcc++;
    if (analogcc <= ASYNC_DELAY) return; // time-critical (insync with loop)

    analogcc = 0;
    int analogTemp;

    //uint16_t max16 = 0;

    switch (pottickcc) {
        case 0:     analogTemp = analogRead(A4) >> 2;
                    if (analogTemp > (potLast[0] + 1) || 
                        analogTemp < (potLast[0] - 1)) {
                        potLast[0]  = analogTemp;
                        lfoSpeed = analogTemp;
                        arpSpeed = 255 - analogTemp;
                    }
                    break;

        case 1:     analogTemp = analogRead(A5) >> 2;
                    if (analogTemp > (potLast[1] + 1) || 
                        analogTemp < (potLast[1] - 1)) {
                        potLast[1] = analogTemp;
                        lfoDepth = analogTemp;
                        arpRange = lfoDepth >> 6;
                    }
                    break;

        case 2:     analogTemp = analogRead(A7) >> 2;
                    if (analogTemp > (potLast[3] + 1) || 
                        analogTemp < (potLast[3] - 1)) {
                        potLast[3] = analogTemp;
                        glide = analogTemp;
                    }
                    break;

        case 3:     analogTemp = analogRead(A6) >> 2;
                    if (analogTemp > (potLast[4] + 1) || 
                        analogTemp < (potLast[4] - 1)) {
                        potLast[4] = analogTemp;

                        detune = analogTemp;
                        doDetune(detune);

                        if (potLast[4] > -1) {
                            if (detune < 127)   ledNumber = oldNumber;
                            else                ledNumber = chord;
                        }
                    }
                    break;

        case 4:     analogTemp = analogRead(A0);
                    if ((analogTemp >> 2) > (potLast[2] + 1) || 
                        (analogTemp >> 2) < (potLast[2] - 1)) {

                        // default (10bit log/lin mix)
                        if (envPeriodType == 0) {
                            lastEnvSpeed = analogTemp;
                            setEnvSpeed(lastEnvSpeed);
                        }

                        // scale down to 8bit
                        analogTemp >>= 2;

                        potLast[2] = analogTemp;
                        seqSpeed = 16 - (analogTemp >> 4); // 1...16

                        // alternate (old 8bit LUT)
                        if (envPeriodType == 1) {
                            lastEnvSpeedLUT = analogTemp;
                            setEnvSpeedLUT(analogTemp);
                        }
                    }
                    break;
    }

    pottickcc++;
    if (pottickcc > 4) pottickcc = 0;
}