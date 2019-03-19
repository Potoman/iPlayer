
#ifndef MY_PLAYER_TRACK_H
#define MY_PLAYER_TRACK_H

#include <string>

class Track {
public:

    static Track STUB;

    const std::string m_title;
    const std::string m_author;

    Track(const std::string & title, const std::string & author) : m_title(title), m_author(author)
    {}

    const std::string & getTitle() const {
        return m_title;
    }

    const std::string & getAuthor() const {
        return m_author;
    }

};

#endif // MY_PLAYER_TRACK_H