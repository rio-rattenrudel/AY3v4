// 
// Monophonic Voice Allocator (algo)
//
// (c) 2014 - Anton Savov (a.k.a antto)
//
// https://antonsavov.net/articles/303andmidi/#mva-mod
//

#define MIDI_MVA_SZ 8

struct mva_t {
    uint8_t buf[MIDI_MVA_SZ];
    uint8_t n;
};

void mvaNoteOn(mva_t *p, uint8_t note) {
    uint8_t s = 0;
    uint8_t i = 0;

    // shift all notes back
    uint8_t m = p->n + 1;

    m = (m > MIDI_MVA_SZ ? MIDI_MVA_SZ : m);
    s = m;
    i = m;

    while (i > 0) {
        --s;
        p->buf[i] = p->buf[s];
        i = s;
    }

    // put the new note first
    p->buf[0] = note;

    // update the voice counter
    p->n = m;
}

void mvaNoteOff(mva_t *p, uint8_t note) {
    uint8_t s = 0;

    // find if the note is actually in the buffer
    uint8_t m = p->n;
    uint8_t i = m;

    while (i) { // count backwards (oldest notes first)
        --i;

        if (note == (p->buf[i] & 0x7f)) {

            // found it!
            if (i < (p->n - 1)) { // don't shift if this was the last note..

                // remove it now.. just shift everything after it
                s = i;
                while (i < m) {
                    ++s;
                    p->buf[i] = p->buf[s];
                    i = s;
                }
            }

            // update the voice counter
            if (m > 0) { p->n = m - 1; }
            break;
        }
    }
}

void mvaReset(mva_t *p) { p->n = 0; }