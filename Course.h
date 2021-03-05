#pragma once

#include "Hole.h"

#include <string>
#include <vector>

class Course
{
public:
	Course();
	Course(const int id, const std::string& name);
	~Course();

	int getId();
	void setId(const int id);

	std::string getName();
	void setName(const std::string& name);

	std::vector<Hole> getHoleList();
	void setHoleList(const std::vector<Hole>& holeList);

private:
	int m_id;
	std::string m_name;
	std::vector<Hole> m_holeList;

};