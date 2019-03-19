#include "Library.h"




Track Library::getTrack(uint32_t trackId) {
    if (m_tracks.size() <= trackId) {
        return Track::STUB;
    }
    return m_tracks.at(trackId);
}