#include "Course.h"

Course::Course()
{

}

Course::Course(const int id, const std::string& name)
	: m_id(id), m_name(name)
{

}

Course::~Course()
{

}

int Course::getId()
{
	return m_id;
}

void Course::setId(const int id)
{
	m_id = id;
}

std::string Course::getName()
{
	return m_name;
}

void Course::setName(const std::string& name)
{
	m_name = name;
}

std::vector<Hole> Course::getHoleList()
{
	return m_holeList;
}

void Course::setHoleList(const std::vector<Hole>& holeList)
{
	m_holeList = holeList;
}
