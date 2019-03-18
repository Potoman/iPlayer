
#ifndef MY_PLAYER_LIBRARY_H
#define MY_PLAYER_LIBRARY_H

#include <vector>

#include "Track.h"

class Library {
private:
    std::vector<Track> m_tracks;

public:
    // @throw : std::out_of_range
    Track getTrack(int trackId);

};

#endif // MY_PLAYER_LIBRARY_H
