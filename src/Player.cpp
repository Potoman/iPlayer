    
#include "Player.h"

#include <string>
#include <thread>
#include <chrono>
#include <memory>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#include <rxterm/terminal.hpp>
#include <rxterm/style.hpp>
#include <rxterm/image.hpp>
#include <rxterm/reflow.hpp>
#include <rxterm/components/text.hpp>
#include <rxterm/components/stacklayout.hpp>
#include <rxterm/components/flowlayout.hpp>
#include <rxterm/components/progress.hpp>
#include <rxterm/components/maxwidth.hpp>

using namespace rxterm;

using namespace std::chrono_literals;
using namespace std::string_literals;

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

    auto mainView = [](auto percent, 
            auto tracks) -> StackLayout<> {
                return {
                        Text("Console music player"),
                        Text(tracks.at(0).getTitle()),
                        Text(tracks.at(1).getTitle()),
                        Text(tracks.at(2).getTitle()),
                        Text(tracks.at(3).getTitle()),
                        Text(tracks.at(4).getTitle()),
                        Progress(percent),
                        Text(Style(Color::Black, FontColor::White, Font::Bold),
                        "v Down     Switch     Up ^")
                };
            };

    for (int i = 0; i < 101; i += 20) {
        vt = renderToTerm(vt, 80, mainView(0.1, getView(0, 5)));
        std::this_thread::sleep_for(200ms);
    }

    /*
    auto superProgressBar3Rows = [](auto x, auto y, auto z) -> StackLayout<> {
    return {
    Text("3 segment progressbar: "),
    FlowLayout<>{
    MaxWidth(20, Progress(x)),
    MaxWidth(20, Progress(y)),
    MaxWidth(20, Progress(z))
    },
    Text(Style(Color::Black, FontColor::White, Font::Bold), "3 segment progressbar: ")
    };
    };
    auto superProgressBar3 = [](auto x, auto y, auto z) -> StackLayout<> {
    return {
    Text("3 segment progressbar: "),
    FlowLayout<>{
    MaxWidth(20, Progress(x)),
    MaxWidth(20, Progress(y)),
    MaxWidth(20, Progress(z))
    },
    Text(Style(Color::Black, FontColor::White, Font::Bold), "jment progressbar: ")
    };
    };

    for (int i = 0; i < 101; i += 20) {
    vt = renderToTerm(vt, 80, superProgressBar(0.01 * i, 0.02 * i, 0.03 * i));
    std::this_thread::sleep_for(200ms);
    }

    for (int i = 0; i < 101; i += 20) {
    vt = renderToTerm(vt, 80, superProgressBar3Rows(0.01 * i, 0.02 * i, 0.03 * i));
    std::this_thread::sleep_for(200ms);
    }*/
}


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