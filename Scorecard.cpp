#include "Scorecard.h"

Scorecard::Scorecard(const Player& player, const Course& course, const std::string& time, const int* score)
	: m_player(player), m_course(course), m_time(time)
{
	setScore(score);
}

Scorecard::~Scorecard()
{

}

Player& Scorecard::getPlayer()
{
	return m_player;
}

void Scorecard::setPlayer(const Player& player)
{
	m_player = player;
}

Course& Scorecard::getCourse()
{
	return m_course;
}

void Scorecard::setCourse(const Course& course)
{
	m_course = course;
}

std::string Scorecard::getTime()
{
	return m_time;
}

void Scorecard::setTime(const std::string& time)
{
	m_time = time;
}

int* Scorecard::getScore()
{
	return m_score;
}

void Scorecard::setScore(const int* score)
{
	for (int i = 0; i < 18; i++)
		m_score[i] = score[i];
}
