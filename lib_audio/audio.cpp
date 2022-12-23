#include "audio.h"

sf::Music Audio::_music;
sf::SoundBuffer Audio::_buffer;
sf::Sound Audio::_sound;

void Audio::start(const char* path){
    _sound.setBuffer(_buffer);
    if(!_music.openFromFile(path))
        std::cout << "Music file can't be accessed" << std::endl;
    _music.setVolume(10.f);
    _music.setLoop(true);
    _music.play();
}

void Audio::play(const char *path) {
    if(!_buffer.loadFromFile(path))
        std::cout << "Audio file can't be accessed" << std::endl;
    _sound.play();
}

Audio::~Audio() = default;