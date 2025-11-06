#pragma once

#include <Arduino.h>

class PedalHandler {
public:
    using NoteOffCallback = void (*)(byte channel, byte note);

    // Maintains per-channel sustain pedal and deferred note-off state.

    PedalHandler()
    {
        reset();
    }

    void handleNoteOn(byte channel, byte note, NoteOffCallback noteOffCallback)
    {
        byte index;
        if (!channelIndex(channel, index)) return;
        bool hadPending = removePending(index, note); // drop deferred release when a note retriggers
        if (hadPending && noteOffCallback) noteOffCallback(channel, note); // keep internal note counters balanced
    }

    bool handleNoteOff(byte channel, byte note)
    {
        byte index;
        if (!channelIndex(channel, index)) return true;

        if (!sustainActive[index]) { // normal release when pedal is up
            removePending(index, note);
            return true;
        }

        addPending(index, note); // store each release until pedal lifts
        return false;
    }

    void handleControlChange(byte channel, byte number, byte value, NoteOffCallback noteOffCallback)
    {
        if (number != 64) return;

        byte index;
        if (!channelIndex(channel, index)) return;

        if (value >= 64) { // pedal down: start deferring note-offs
            sustainActive[index] = true;
            return;
        }

        sustainActive[index] = false;

        if (!noteOffCallback) {
            pendingCount[index] = 0;
            return;
        }

        for (byte i = 0; i < pendingCount[index]; i++) // pedal up: release stored notes
            noteOffCallback(channel, pendingNotes[index][i]);

        pendingCount[index] = 0;
    }

private:
    static const byte kMaxChannels = 16;
    static const byte kMaxPendingNotes = 20;

    bool sustainActive[kMaxChannels];
    byte pendingCount[kMaxChannels];
    byte pendingNotes[kMaxChannels][kMaxPendingNotes];

    void reset()
    {
        for (byte i = 0; i < kMaxChannels; i++) {
            sustainActive[i] = false;
            pendingCount[i] = 0;
            for (byte j = 0; j < kMaxPendingNotes; j++)
                pendingNotes[i][j] = 0;
        }
    }

    static bool channelIndex(byte channel, byte& index)
    {
        if (channel == 0 || channel > kMaxChannels) return false;
        index = channel - 1;
        return true;
    }

    void addPending(byte index, byte note)
    {
        if (pendingCount[index] >= kMaxPendingNotes) return;
        pendingNotes[index][pendingCount[index]++] = note;
    }

    bool removePending(byte index, byte note)
    {
        if (pendingCount[index] == 0) return false;

        for (byte i = 0; i < pendingCount[index]; i++) {
            if (pendingNotes[index][i] == note) {
                for (byte j = i + 1; j < pendingCount[index]; j++)
                    pendingNotes[index][j - 1] = pendingNotes[index][j];
                pendingCount[index]--;
                return true;
            }
        }
        return false;
    }
};
