#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "GameSpriteModule.h"
#include "GameWidgetModule.h"
#include "GameEventModule.h"
#include "GameInputModule.h"

#include <box2d.h>
#include <vector>

class GameScene
{
public:
	typedef GameScene* (*ConstructFunc)();
	struct Entry 
	{
		std::string		strName;
		ConstructFunc	funcConstruct;
	};

	struct Camera
	{
		b2Body*		pBody;
		b2Joint*	pJoint;
	};

	virtual void	Update();
	virtual void	Draw();

	b2Body*			CreateBody(const b2BodyDef* defBody, GameNode* pNode);
	void			DestroyBody(b2Body* pBody);

	b2Joint*		CreateJoint(const b2JointDef* defJoint);
	void			DestroyJoint(b2Joint* pJoint);

	b2Joint*		CameraFollow(b2Body* pBody);

private:
	b2World*	m_pWorld;

	std::vector<GameNode*>						m_vecNode;
	std::vector<GameSprite*>					m_vecSprite;
	std::vector<GameEventManager::Listener*>	m_vecListener;

private:
	GameScene(b2Vec2 vec2Gravity);
	virtual ~GameScene();
};

class GameSceneManager
{
public:


private:


public:
	~GameSceneManager();
	GameSceneManager(const GameSceneManager&) = delete;
	GameSceneManager& operator=(const GameSceneManager&) = delete;
	static GameSceneManager& GetInstance()
	{
		static GameSceneManager instance;
		return instance;
	}
private:
	GameSceneManager();
};

#endif // !_GAME_SCENE_H_