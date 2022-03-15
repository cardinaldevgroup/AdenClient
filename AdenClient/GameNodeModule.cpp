#include "GameNodeModule.h"

class GameNode::Impl
{
public:

public:

};

GameNode::GameNode()
{
	m_pImpl = new Impl();
}

GameNode::~GameNode()
{
	delete m_pImpl;
}

class GameNodeManager::Impl
{

};

GameNodeManager::GameNodeManager()
{
	m_pImpl = new Impl();
}

GameNodeManager::~GameNodeManager()
{
	delete m_pImpl;
}