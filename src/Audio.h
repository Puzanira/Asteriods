#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <utility>
#include <SFML/Audio.hpp>

class Audio {
public :

	Audio();
	~Audio();

	void playMusic();
	void soundFire();
	void soundExplosion(int size);
	void incIndex();
	void incMusicVolume();
	void decMusicVolume();

	void setSoundVolume(int vol) {
		soundVolume = vol;
	}

	void setMusicVolume(int vol) {
		musicVolume = vol;
	}

	int getSoundVolume() {
		return soundVolume;
	}

	int getMusicVolume() {
		return musicVolume;
	}

	void stopSong() {
		music.stop();
		nowPlaying = false;
	}

private :
	sf::Music music;

	sf::SoundBuffer fireBuff;
	sf::SoundBuffer bigExpBuff;
	sf::SoundBuffer medExpBuff;
	sf::SoundBuffer smallExpBuff;
	
	sf::Sound fire;
	sf::Sound bigExp;
	sf::Sound medExp;
	sf::Sound smallExp;

	std::vector<std::string> listOfSongs;
	sf::Clock songTime;

	int musicVolume;
	int soundVolume;
	int currentSongIndex;
	bool nowPlaying;
};

#endif