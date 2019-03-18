
#ifndef MY_PLAYER_PLAYER_H
#define MY_PLAYER_PLAYER_H

#include <string>

#include "Library.h"
#include "Track.h"

class PlayerView {
public:
    virtual ~PlayerView();

    virtual PlayerView & process(const std::string & cmd);
};

class PlayerViewTrack : public PlayerView {
public:
    virtual ~PlayerViewTrack();

    PlayerView & process(const std::string & cmd) override;
};

class PlayerViewListening : public PlayerView {
public:
    virtual ~PlayerViewListening();

    PlayerView & process(const std::string & cmd) override;
};

namespace {
PlayerViewTrack TRACK_VIEW;
PlayerViewListening LISTENING_VIEW;
}

class Player {
private:

    Library m_library;
    PlayerView & m_view;

public:

    Player() : m_view(TRACK_VIEW) {
    }
    /*TrackId addTrack(const std::string & trackPath);

    void removeTrack();

    void createTrack();

    void play(const TrackId & trackId);*/

    void process();


    std::vector<Track> getView(int indexStart, int count);

    void process(const std::string & cmd);
};

#endif // MY_PLAYER_PLAYER_H