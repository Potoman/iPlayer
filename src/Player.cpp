    
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

size_t SIZE = 36;
VirtualTerminal vt;

void Player::process() {

    vt = renderToTerm(vt, SIZE, m_state->getView());
    while (true) {
        char c = getch();
        if (c == 65) {
            process("up");
        }
        else if (c == 66) {
            process("down");
        }
        else if (c == 10) {
            process("enter");
        }
        else {
            std::string cmd(1, c);
            process(cmd);
        }
        updateView();
        std::this_thread::sleep_for(25ms);
    }
}

void Player::updateView() {
    vt = renderToTerm(vt, SIZE, m_state->getView());
}

void Player::process(const std::string & cmd) {
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    m_state->process(cmd);
    if (m_futureState) {
        m_state.swap(m_futureState);
        m_futureState.reset();
    }
}

void Player::setState(std::unique_ptr<PlayerView> & view) {
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    m_futureState.swap(view);
    view.reset();
}

PlayerView::~PlayerView() {
}

void PlayerView::fire() {
    p.updateView();
}

PlayerViewTrack::PlayerViewTrack(int index) : PlayerView(), m_currentIndex(index) {
}

PlayerViewTrack::~PlayerViewTrack() {
}

void PlayerViewTrack::process(const std::string & cmd) {
    if (cmd == "p" || cmd == "enter") {
        if (p.getLibrary().isValidIndex(m_currentIndex)) {
            std::unique_ptr<PlayerView> state(new PlayerViewListening(m_currentIndex));
            p.setState(state);
        }
    }
    else if (cmd == "d" || cmd == "down") {
        m_currentIndex++;
        fire();
    }
    else if (cmd == "u" || cmd == "up") {
        m_currentIndex--;
        fire();
    }
    else if (cmd == "a") {
        // Here add track.
        std::cout << "Adding music..." << std::endl;
        std::string title;
        std::cout << "Tap title : ";
        std::getline(std::cin, title);
        std::string author;
        std::cout << "Tap author : ";
        std::getline(std::cin, author);
        std::string time;
        std::cout << "Tap time (in seconds) of the song : ";
        std::getline(std::cin, time);
        try {
            Track t(title, author, std::chrono::seconds(std::stoi(time)));
            p.getLibrary().addTrack(t);
            fire();
        }
        catch (const std::invalid_argument & ia) {
            std::cerr << "Wrong time.";
            fire();
        }
    }
}

Component PlayerViewTrack::getView() {
    double percent = p.getLibrary().getSize() == 0 ? 1.0 : (double) m_currentIndex / (double) p.getLibrary().getSize();
    return StackLayout<>{
                    Text("Console music player"),
                    Text("  " + p.getLibrary().getTrack(m_currentIndex - 2).getTitle()),
                    Text("  " + p.getLibrary().getTrack(m_currentIndex - 1).getTitle()),
                    Text("> " + p.getLibrary().getTrack(m_currentIndex).getTitle() + " <"),
                    Text("  " + p.getLibrary().getTrack(m_currentIndex + 1).getTitle()),
                    Text("  " + p.getLibrary().getTrack(m_currentIndex + 2).getTitle()),
                    Progress(percent, Pixel{' ', {Color::White}}, Pixel{' ', {Color::Green}}),
                    Text(Style(Color::Black, FontColor::White, Font::Bold),
                    "v Down           Add            Up ^")
            };
}

PlayerViewListening::PlayerViewListening(const uint32_t trackId) : PlayerView(), m_trackId(trackId), m_isRunning(true), m_isPlay(true), m_timeElapse(0) {
    m_threadPlaying = std::thread([&] {
        while (m_isRunning) {
            for (int i = 0; i < 40; i++) { // Clearly a condition variable will do the job much better !
                std::this_thread::sleep_for(25ms);
                if (!m_isRunning) {
                    break;
                }
            }
            if (m_isPlay) {
                m_timeElapse++;
                fire();
            }
        }
    });
}

PlayerViewListening::~PlayerViewListening() {
    m_isRunning = false;
    if (m_threadPlaying.joinable()) {
        m_threadPlaying.join();
    }
}

void PlayerViewListening::process(const std::string & cmd) {
    if (cmd == "b") {
        std::unique_ptr<PlayerView> state(new PlayerViewTrack(m_trackId));
        p.setState(state);
    }
    else if (cmd == "p" || cmd == " ") {
        m_isPlay = !m_isPlay;
    }
    else if (cmd == "n") {
        const uint32_t trackId = m_trackId + 1;
        std::unique_ptr<PlayerView> state(new PlayerViewListening(trackId));
        p.setState(state);
    }
    else if (cmd == "e") {
        std::unique_ptr<PlayerView> state(new PlayerViewListening(m_trackId - 1));
        p.setState(state);
    }
}

Component PlayerViewListening::getView() {
    const Track & track = p.getLibrary().getTrack(m_trackId);
    double percent = 1.0 - (double) (std::min(track.getTime(), track.getTime() - m_timeElapse)).count() / (double) track.getTime().count();
    if (percent == 1.0) {
        // Song finish
        process("n");
    }
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
    auto lineStatus = [&] {
        std::ostringstream oss;
        oss << "< prEvious     " << (m_isPlay ? "Play " : "Pause") << "          Next >";
        return oss.str();
    };
    auto timeStatus = [&] {
        std::ostringstream oss;
        {
            uint32_t timeElpase = m_timeElapse.count();
            uint32_t sec = timeElpase % 60;
            oss << (timeElpase / 60) << "." << (sec < 10 ? "0" : "") << sec;
        }
        oss << "                            ";
        {
            uint32_t timeRemain = track.getTime().count() - m_timeElapse.count();
            uint32_t sec = timeRemain % 60;
            oss << (timeRemain / 60) << "." << (sec < 10 ? "0" : "") << sec;
        }
        return oss.str();
    };
    return StackLayout<>{
                    Text(line0()),
                    Text(line1()),
                    Progress(percent, Pixel{' ', {(m_isPlay ? Color::Blue : Color::Red)}}, Pixel{' ', {(m_isPlay ? Color::Cyan : Color::Yellow)}}),
                    Text(Style(Color::Black, FontColor::White, Font::Bold), timeStatus()),
                    Text(lineStatus()),
                    Text("Random                        rEpeat"),
                    Text("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"),
                    Text("Back to tracklist"),
            };
}

PlayerViewTrack TRACK_VIEW(0);
PlayerViewListening LISTENING_VIEW(0);

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

    for (int i = 0; i < 7; i++) {
        std::string title = "song " + std::to_string(i) + ".mp3";
        Track t(title, "Johnny", std::chrono::seconds(55));
        p.getLibrary().addTrack(t);
    }
    p.process();

    return 0;
}
