
#ifndef MY_PLAYER_PLAYER_H
#define MY_PLAYER_PLAYER_H

/*
class PlayerState {
};*/

#include "Library.h"
#include "Track.h"

class Player {
private:

    Library m_library;

public:
    /*TrackId addTrack(const std::string & trackPath);

    void removeTrack();

    void createTrack();

    void play(const TrackId & trackId);*/

    void process();


    std::vector<Track> getView(int indexStart, int count);
};

#endif // MY_PLAYER_PLAYER_H