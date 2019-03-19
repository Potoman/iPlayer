
#ifndef MY_PLAYER_PLAYER_H
#define MY_PLAYER_PLAYER_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>
#include <rxterm/reflow.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/flowlayout.hpp>
#include <rxterm/components/progress.hpp>
#include <rxterm/components/maxwidth.hpp>

#include "Library.h"
#include "Track.h"

using namespace rxterm;

class PlayerView {
protected:

public:
    PlayerView() {
    }

    virtual ~PlayerView();

    virtual void process(const std::string & cmd) = 0;
    virtual Component getView() = 0;

    void fire();
};

class PlayerViewTrack : public PlayerView {
private:

    int m_currentIndex;

public:
    PlayerViewTrack(int index);
    virtual ~PlayerViewTrack();

    void process(const std::string & cmd) override;
    Component getView() override;
};

class PlayerViewListening : public PlayerView {
private:

    const uint32_t m_trackId;

    std::atomic_bool m_isRunning;
    bool m_isPlay;
    std::chrono::seconds m_timeElapse;
    std::thread m_threadPlaying;

public:
    PlayerViewListening(const uint32_t trackId);
    virtual ~PlayerViewListening();

    void process(const std::string & cmd) override;
    Component getView() override;
};

class Player {
private:

    std::recursive_mutex m_lock;
    Library m_library;
    std::unique_ptr<PlayerView> m_state;
    std::unique_ptr<PlayerView> m_futureState;

public:

    Player() : m_state(new PlayerViewTrack(0)) {
    }

    void process();

    Library & getLibrary() {
        return m_library;
    }

    std::vector<Track> getView(int indexStart, int count);

    void updateView();
    void process(const std::string & cmd);
    void setState(std::unique_ptr<PlayerView> & view);
};

#endif // MY_PLAYER_PLAYER_H