
#ifndef MY_PLAYER_TRACK_H
#define MY_PLAYER_TRACK_H

#include <chrono>
#include <string>

class Track {
public:

    static Track STUB;

    const std::string m_title;
    const std::string m_author;
    const std::chrono::seconds m_seconds;

    Track(const std::string & title, const std::string & author, uint32_t seconds) : m_title(title), m_author(author), m_seconds(seconds)
    {}

    const std::string & getTitle() const {
        return m_title;
    }

    const std::string & getAuthor() const {
        return m_author;
    }

    const std::chrono::seconds getTime() const {
        return m_seconds;
    }

};

#endif // MY_PLAYER_TRACK_H