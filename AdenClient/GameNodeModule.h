#ifndef _GAME_NODE_MODULE_H_
#define _GAME_NODE_MODULE_H_

class GameNode
{
public:

private:
	class Impl;
	Impl* m_pImpl;
public:
	GameNode();
	~GameNode();
};

class GameNodeManager
{
public:


private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameNodeManager();
	GameNodeManager(const GameNodeManager&) = delete;
	GameNodeManager& operator=(const GameNodeManager&) = delete;
	static GameNodeManager& GetInstance()
	{
		static GameNodeManager instance;
		return instance;
	}
private:
	GameNodeManager();
};

#endif