
#ifndef MY_PLAYER_TRACK_H
#define MY_PLAYER_TRACK_H

#include <string>

class Track {
public:

    static Track STUB;

    const std::string m_title;

    Track(const std::string & title) : m_title(title)
    {}

    const std::string & getTitle() const {
        return m_title;
    }

};

#endif // MY_PLAYER_TRACK_H