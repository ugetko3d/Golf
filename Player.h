#pragma once

#include <string>

class Player
{
public:
	Player();
	Player(const int id, const std::string& name, const int handicap);
	~Player();

	int getId();
	void setId(const int id);

	std::string& getName();
	void setName(const std::string& name);

	int getHandicap();
	void setHandicap(const int handicap);

private:
	int m_id;
	std::string m_name;
	int m_handicap;

};