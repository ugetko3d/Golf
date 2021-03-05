#include "Player.h"

Player::Player()
{

}

Player::Player(const int id, const std::string& name, const int handicap)
    : m_id(id), m_name(name), m_handicap(handicap)
{

}

Player::~Player()
{

}

int Player::getId()
{
    return m_id;
}

void Player::setId(const int id)
{
    m_id = id;
}

std::string& Player::getName()
{
    return m_name;
}

void Player::setName(const std::string& name)
{
    m_name = name;
}

int Player::getHandicap()
{
    return m_handicap;
}

void Player::setHandicap(const int handicap)
{
    m_handicap = handicap;
}
