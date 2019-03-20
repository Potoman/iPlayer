#include "Library.h"

Track Library::getTrack(uint32_t trackId) {
    if (isValidIndex(trackId)) {
        return m_tracks.at(trackId);
    }
    return Track::STUB;
}

void Library::addTrack(Track & track) {
    m_tracks.push_back(track);
}

bool Library::isValidIndex(uint32_t trackId) {
    return m_tracks.size() > trackId;
}

uint32_t Library::getSize() {
    return m_tracks.size();
}
