
#ifndef MY_PLAYER_PLAYER_H
#define MY_PLAYER_PLAYER_H

#include <string>

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

    std::string m_component;

public:
    PlayerView(std::string cmp) : m_component(cmp) {
    }
    virtual ~PlayerView();

    virtual std::unique_ptr<PlayerView> process(const std::string & cmd) = 0;
    virtual Component getView() = 0;
};

class PlayerViewTrack : public PlayerView {
private:

    int m_currentIndex;

public:
    PlayerViewTrack(int index);
    virtual ~PlayerViewTrack();

    std::unique_ptr<PlayerView> process(const std::string & cmd) override;
    Component getView() override;
};

class PlayerViewListening : public PlayerView {
private:

    const Track & m_track;

public:
    PlayerViewListening(const Track & track);
    virtual ~PlayerViewListening();

    std::unique_ptr<PlayerView> process(const std::string & cmd) override;
    Component getView() override;
};

class Player {
private:

    Library m_library;
    std::unique_ptr<PlayerView> m_view;

public:

    Player() : m_view(new PlayerViewTrack(0)) {
    }

    void process();

    Library & getLibrary() {
        return m_library;
    }

    std::vector<Track> getView(int indexStart, int count);

    void process(const std::string & cmd);
};

#endif // MY_PLAYER_PLAYER_H