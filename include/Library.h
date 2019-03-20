
#ifndef MY_PLAYER_LIBRARY_H
#define MY_PLAYER_LIBRARY_H

#include <vector>

#include "Track.h"

class Library {
private:
    std::vector<Track> m_tracks;

public:

    Track getTrack(uint32_t trackId);
    void addTrack(Track & track);
    bool isValidIndex(uint32_t trackId);
    uint32_t getSize();

};

#endif // MY_PLAYER_LIBRARY_H
