#pragma once

#include "Player.h"
#include "Course.h"

#include <QDateTimeEdit>

class Scorecard
{
public:
	Scorecard(const Player& player, const Course& course, const std::string& time, const int* score = { 0 });
	~Scorecard();

	Player& getPlayer();
	void setPlayer(const Player& player);

	Course& getCourse();
	void setCourse(const Course& course);

	std::string getTime();
	void setTime(const std::string& time);

	int* getScore();
	void setScore(const int* score);

private:
	Player m_player;
	Course m_course;
	std::string m_time;
	int m_score[18] = { 0 };
};
