#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

class Audio{
protected:
    static sf::Music _music;
    static sf::SoundBuffer _buffer;
    static sf::Sound _sound;

public:
    static void play(const char* path);
    static void start(const char* path);
    ~Audio();
};