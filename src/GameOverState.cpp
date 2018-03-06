#include "GameOverState.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <cstring>

GameOverState::GameOverState(Game* _game, unsigned _score) :
score(_score), 
GameState(_game),
title(getText(60, 75.f, "YOUR SCORE:" + std::to_string(score))),
nameField(getText(45, 250.f)),
enterNameText(getText(25, 210.f, "ENTER YOUR NAME:", sf::Text::Style::Regular)),
confirmButton(getText(35, 350.f, "confirm")),
cancelButton(getText(35, 415.f, "cancel")) {
}

GameOverState::~GameOverState() {
}

void GameOverState::render() {
	window->clear();
	window->draw(title);
	window->draw(nameField);
	window->draw(enterNameText);
	window->draw(confirmButton);
	window->draw(cancelButton);
	window->display();
}

void GameOverState::handleEvents() {
	sf::Event event;

	while (window->pollEvent(event)) {
		if (event.type == sf::Event::EventType::Closed) {
			window->close();
		}
		if (event.type == sf::Event::EventType::TextEntered && event.text.unicode < 128) {
			if (event.text.unicode == '\b' && !name.empty()) {
				name.pop_back();
			}
			else if (name.size() < 15 && event.text.unicode > 8) {
				name += event.text.unicode;
			}
			nameField.setString(name);
			centerText(nameField, 250.f);
		}
		if (event.type == sf::Event::EventType::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			if (confirmButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
				addScore();
				game->changeState(Game::States::HIGHSCORES);
				break;
			}
			if (cancelButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
				game->changeState(Game::States::MAIN_MENU);
				break;
			}
		}
	}
}

void GameOverState::addScore() {
	std::ifstream file("./HighScoreFile", std::ios::in);

	if (!file) {
		std::cerr << "Error:can`t open stream!\n";

		return;
	}

	if (!name.size()) {
		name = "Unnamed";
	}

	std::map<std::string,std::string> score_list;
	std::string fname;
	std::string fscore;
	char *f_buf = NULL;
	char *f_reserv = NULL;
	char buf[4096];

	while (!file.eof()) {
		file.read(buf, sizeof buf);

		if (!strlen(buf) && !f_buf) {
			file.close();
			std::ofstream file("./HighScoreFile",std::ios::out);
			file << name << ":" << score << "\n";
			file.close();

			return;
		}

		if (!f_buf) {
			f_buf = new char[file.gcount()];
			snprintf(f_buf, file.gcount(), "%s", buf);
		}
		else {
		f_reserv = new char[strlen(f_buf) + 1];
		snprintf(f_reserv, strlen(f_buf) + 1, "%s", f_buf);
		delete []f_buf;
		f_buf = new char[strlen(f_reserv) + 1 + file.gcount()];
		snprintf(f_buf, strlen(f_reserv) + 1 + file.gcount(), "%s%s", f_reserv, buf);
		delete []f_reserv;
		}
	} 
	
	fname = strtok(f_buf,": \n");
	fscore = strtok(NULL, ": \n");
	score_list.insert(std::pair<std::string,std::string>(fname, fscore));

	while ((f_reserv = strtok(NULL, ": \n")) != NULL) {
		fname = f_reserv;
		fscore = strtok(NULL, ": \n");
		score_list.insert(std::pair<std::string,std::string>(fname, fscore));
	}

	file.close();
	std::ofstream out_file("./HighScoreFile",std::ios::out);
	
	std::map<std::string, std::string>::iterator iter;
	iter = score_list.find(name);
	
	if (iter -> first != name) {
		out_file << name << ":" << score << "\n";
		std::map<std::string,std::string>::iterator i = score_list.begin();

		for(i ; i != score_list.end() ; i++) {
			out_file << i->first << ":" << i->second << "\n";
		}
	}
	else {
		char s_score[256];
		snprintf(s_score, sizeof s_score, "%u",score);
		iter -> second = s_score;
		std::map<std::string,std::string>::iterator i = score_list.begin();

		for(i ; i != score_list.end() ; i++) {
			out_file << i->first << ":" << i->second << "\n";
		}
	}

	delete []f_buf;
	out_file.close();
}
