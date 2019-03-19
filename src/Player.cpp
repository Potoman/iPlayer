    
#include "Player.h"

#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <memory>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

using namespace std::chrono_literals;
using namespace std::string_literals;

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

Player p;

auto renderToTerm = [](auto const& vt, unsigned const w, Component const& c) {
    // TODO: get actual terminal width
    return vt.flip(c.render(w).toString());
};

std::vector<Track> Player::getView(int indexStart, int count) {
    std::vector<Track> view;
    for (int i = indexStart; i < indexStart + count; i++) {
        view.push_back(m_library.getTrack(i));
    }
    return view;
}

void Player::process() {
    // What do you want to do ?
    // < add track
    // < remove a track
    // < play a track

    // play, pause, next, previous
    // Random

    VirtualTerminal vt;

    vt = renderToTerm(vt, 80, m_view->getView());
    while (true) {
        char c = getch();
        std::string cmd(1, c);
        process(cmd);
        vt = renderToTerm(vt, 80, m_view->getView());
        std::this_thread::sleep_for(200ms);
    }
}

void Player::process(const std::string & cmd) {
    m_view = m_view->process(cmd);
}

PlayerView::~PlayerView() {
}

PlayerViewTrack::PlayerViewTrack(int index) : PlayerView("mlk"), m_currentIndex(index) {
}

PlayerViewTrack::~PlayerViewTrack() {
}

std::unique_ptr<PlayerView> PlayerViewTrack::process(const std::string & cmd) {
    if (cmd == "p") {
        return std::unique_ptr<PlayerView>(new PlayerViewListening(p.getLibrary().getTrack(m_currentIndex)));
    }
    return std::unique_ptr<PlayerView>(new PlayerViewTrack(m_currentIndex));
}

Component PlayerViewTrack::getView() {
    static double percent = 0.0;
    percent += 0.1;
    return StackLayout<>{
                    Text("Console music player"),
                    Text(p.getLibrary().getTrack(0).getTitle()),
                    Text(p.getLibrary().getTrack(1).getTitle()),
                    Text(p.getLibrary().getTrack(2).getTitle()),
                    Text(p.getLibrary().getTrack(3).getTitle()),
                    Text(p.getLibrary().getTrack(4).getTitle()),
                    Progress(percent),
                    Text(Style(Color::Black, FontColor::White, Font::Bold),
                    "v Down     Switch     Up ^")
            };
}

PlayerViewListening::PlayerViewListening(const Track & track) : PlayerView("mlk"), m_track(track) {
}

PlayerViewListening::~PlayerViewListening() {
}

bool isPlay = true;

std::unique_ptr<PlayerView> PlayerViewListening::process(const std::string & cmd) {
    if (cmd == "b") {
        return std::unique_ptr<PlayerView>(new PlayerViewTrack(0));
    }
    else if (cmd == "p" || cmd == " ") {
        isPlay = !isPlay;
        return std::unique_ptr<PlayerView>(new PlayerViewListening(m_track));
    }
    else {
        return std::unique_ptr<PlayerView>(new PlayerViewListening(m_track));
    }
}

Component PlayerViewListening::getView() {
    static double percent = 0.0;
    percent += 0.1;
    percent = std::min(percent, 1.0);
    const Track & track = p.getLibrary().getTrack(0);
    auto line0 = [&track] {
        std::ostringstream oss;
        oss << "Name : " << track.getTitle();
        return oss.str();
    };
    auto line1 = [&track] {
        std::ostringstream oss;
        oss << "Author : " << track.getAuthor();
        return oss.str();
    };
    auto lineStatus = [] {
        std::ostringstream oss;
        oss << "< Previous     " << (isPlay ? "Play " : "Pause") << "          Next >";
        return oss.str();
    };
    
    return StackLayout<>{
                    Text(line0()),
                    Text(line1()),
                    Text("----------------------I-------------"),
                    Text("0.00                            2.35"),
                    Text(lineStatus()),
                    Text("Random                        rEpeat"),
                    Text("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"),
                    Text("Back to tracklist"),
            };
}

PlayerViewTrack TRACK_VIEW = PlayerViewTrack(0);
PlayerViewListening LISTENING_VIEW = PlayerViewListening(Track::STUB);

// When playing :

// Main menu :

// Console Music Player
// 
// > Manage track
// > Play track
// > Show tracklist
// > Configure

// Track : 

// Track list :

// Track : toto.tl
// ^
// 3: first_track             <
// 4: my_track
// 5: my_second_track
// v

// Song list :

// Track : toto.tl
// ^
// 3: toto.mp3                <
// 4: plip.mp3
// 5: plip.mp3
// v

// Track listen :

//      Name : toto.mp3
// ----------^-----------------
// 0.00                    2.35
// < Previous   Pause    Next >
// Random O            rEpeat O
// Back to menu

int main() {

  p.process();

  return 0;
}
