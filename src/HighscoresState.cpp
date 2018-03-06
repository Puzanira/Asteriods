#include "HighscoresState.h"
#include <SFML/Network.hpp>
#include <iostream>

HighscoresState::HighscoresState(Game *_game) :
GameState(_game),
title(getText(45,25.f,"high scores")),
backButton(getText(30, 500.f, "back")),
scores(getText(25, 150.f, "", sf::Text::Style::Regular)) {
	getScores();
}

HighscoresState::~HighscoresState() {
}

void HighscoresState::render() {
	window->clear();
	window->draw(title);
	window->draw(scores);
	window->draw(backButton);
	window->display();
}

void HighscoresState::getScores() {
	std::ifstream file("./HighScoreFile", std::ios::in);

	if (!file) {
		std::cerr << "Error:can`t open stream!\n";

		return;
	}

	char *f_buf = NULL;
	char *f_reserv = NULL;
	char buf[4096];

	 while (!file.eof()) {
		file.read(buf, sizeof buf);
		if(file.eof() && !strchr(buf, ':')) {
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
	
	std::string fname = strtok(f_buf,": \n");
	char *fscore= strtok(NULL, ": \n");
	std::list<std::pair<std::string, int> > score_list;
	score_list.push_back(std::pair<std::string, int >(fname, atoi(fscore)));

	while ((f_reserv = strtok(NULL, ": \n")) != NULL) {
		fname = f_reserv;
		fscore = strtok(NULL, ": \n");
		score_list.push_back(std::pair<std::string, int>(fname, atoi(fscore)));
	}

	file.close();
	delete []f_buf;

	score_list.sort(my_cmp);
	std::list<std::pair<std::string, int> >::iterator i = score_list.begin();
	std::string out("");

	for (int j = 0; j < 5 && score_list.size() ; j++) {
		char s_score[256];
		snprintf(s_score, sizeof s_score, "%u", i -> second);
		out = out + i -> first;
		out = out + ":";
		out = out + s_score;
		out = out + "\n";
		i = score_list.erase(i);
		
	}
	
	getText(40, 50, out);
	scores.setString(out);
}

void HighscoresState::handleEvents() {
	sf::Event event;

	while (window->pollEvent(event)) {
		if (event.type == sf::Event::EventType::Closed) {
			window->close();
		}
		if ((event.type == sf::Event::EventType::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::EventType::KeyPressed)) {
			if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || event.key.code == sf::Keyboard::B) {
				game->changeState(Game::States::MAIN_MENU);
				break;
			}
		}
	}
}

bool HighscoresState::my_cmp(std::pair<std::string, int> o1, std::pair<std::string, int> o2) {
	return o1.second > o2.second;
}