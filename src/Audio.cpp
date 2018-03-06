#include "Audio.h"

Audio::Audio():musicVolume(100), soundVolume(100), nowPlaying(false) {
	listOfSongs.push_back(std::string("./music/Warriors.ogg"));
	listOfSongs.push_back(std::string("./music/Omen.ogg"));
	listOfSongs.push_back(std::string("./music/Highway_to_hell.ogg"));
	listOfSongs.push_back(std::string("./music/Sweet-dreams.ogg"));

	if (!listOfSongs.size()) {
		currentSongIndex = -1;
	}
	else {
		currentSongIndex = 0;
		music.openFromFile(listOfSongs[currentSongIndex]);
	}
	
	fireBuff.loadFromFile("./sounds/Shoot.ogg");
	fire.setBuffer(fireBuff);

	bigExpBuff.loadFromFile("./sounds/expBig.ogg");
	bigExp.setBuffer(bigExpBuff);

	medExpBuff.loadFromFile("./sounds/expMed.ogg");
	medExp.setBuffer(medExpBuff);
	
	smallExpBuff.loadFromFile("./sounds/expSmall.ogg");
	smallExp.setBuffer(smallExpBuff);
}

Audio::~Audio() {
	music.stop();
	fire.stop();
	bigExp.stop();
	medExp.stop();
	smallExp.stop();
}

void Audio::playMusic() {
	if (musicVolume && (currentSongIndex != -1)) {
		music.setVolume(musicVolume);

		if (!nowPlaying) {
			music.play();
			nowPlaying = true;
			songTime.restart();
		}
		else if (songTime.getElapsedTime().asSeconds() >= music.getDuration().asSeconds()) {
			currentSongIndex++;

			if(currentSongIndex == listOfSongs.size()) {
				currentSongIndex = 0;
			}

			music.openFromFile(listOfSongs[currentSongIndex]);
			music.play();
			songTime.restart();
		}
	}

	if (!musicVolume && nowPlaying) {
		music.setVolume(0);
	}
}

void Audio::soundFire() {
	if (soundVolume) {
		fire.setVolume(soundVolume);
		fire.play();
	}
}

void Audio::soundExplosion(int size) {
	if (soundVolume) {
		switch(size) {
			case 0 :
				bigExp.play();
				break;
			case 1 :
				medExp.play();
				break;
			case 2 :
				smallExp.play();
		}
	}
}

void Audio::incIndex() {
	if (currentSongIndex != -1) {
		currentSongIndex++;

		if (currentSongIndex == listOfSongs.size()){
			currentSongIndex = 0;
		}

		music.openFromFile(listOfSongs[currentSongIndex]);
	}
}

void Audio::incMusicVolume() {
	musicVolume++;

	if(musicVolume > 100){
		musicVolume = 100;
	}

	music.setVolume(musicVolume);
}

void Audio::decMusicVolume() {
	musicVolume--;

	if(musicVolume < 0) {
		musicVolume = 0;
	}

	music.setVolume(musicVolume);
}