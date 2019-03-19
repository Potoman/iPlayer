
#ifndef MY_PLAYER_LIBRARY_H
#define MY_PLAYER_LIBRARY_H

#include <vector>

#include "Track.h"

class Library {
private:
    std::vector<Track> m_tracks;

public:
    // @throw : std::out_of_range
    Track getTrack(uint32_t trackId);
    //Track getNextTrack(const Track & track);

};

#endif // MY_PLAYER_LIBRARY_H
