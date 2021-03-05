#pragma once

class Hole
{
public:
	Hole(const int id, const int holeNr, const int par = 4, const int handicap = 25, const int length = 350,
		 const bool isWater = false, const bool isFairwayBunker = true, const bool isGreenBunker = true);
	~Hole();

	int getId();
	void setId(const int id);

	int getHoleNr();
	void setHoleNr(const int holeNr);

	int getPar();
	void setPar(const int par);

	int getHandicap();
	void setHandicap(const int handicap);

	int getLength();
	void setLength(const int length);

	bool getIsWater();
	void setIsWater(const bool isWater);

	bool getIsFairwayBunker();
	void setIsFairwayBunker(const bool isFairwayBunker);

	bool getIsGreenBunker();
	void setIsGreenBunker(const bool isGreenBunker);

private:
	int m_id;
	int m_holeNr;
	int m_par;
	int m_handicap;
	int m_length;
	bool m_isWater;
	bool m_isFairwayBunker;
	bool m_isGreenBunker;
};