#include "Hole.h"



Hole::Hole(const int id, const int holeNr, const int par, const int handicap, const int length,
	       const bool isWater, const bool isFairwayBunker, const bool isGreenBunker)
	: m_id(id), m_holeNr(holeNr), m_par(par), m_handicap(handicap), m_length(length),
	  m_isWater(isWater), m_isFairwayBunker(isFairwayBunker), m_isGreenBunker(isGreenBunker)
{

}

Hole::~Hole()
{

}

int Hole::getId()
{
	return m_id;
}

void Hole::setId(const int id)
{
	m_id = id;
}

int Hole::getHoleNr()
{
	return m_holeNr;
}

void Hole::setHoleNr(const int holeNr)
{
	m_holeNr = holeNr;
}

int Hole::getPar()
{
	return m_par;
}

void Hole::setPar(const int par)
{
	m_par = par;
}

int Hole::getHandicap()
{
	return m_handicap;
}

void Hole::setHandicap(const int handicap)
{
	m_handicap = handicap;
}

int Hole::getLength()
{
	return m_length;
}

void Hole::setLength(const int length)
{
	m_length = length;
}

bool Hole::getIsWater()
{
	return m_isWater;
}

void Hole::setIsWater(const bool isWater)
{
	m_isWater = isWater;
}

bool Hole::getIsFairwayBunker()
{
	return m_isFairwayBunker;
}

void Hole::setIsFairwayBunker(const bool isFairwayBunker)
{
	m_isFairwayBunker = isFairwayBunker;
}

bool Hole::getIsGreenBunker()
{
	return m_isGreenBunker;
}

void Hole::setIsGreenBunker(const bool isGreenBunker)
{
	m_isGreenBunker = isGreenBunker;
}