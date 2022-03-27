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

	struct Camera
	{
		b2Body*			pCameraBody;
		GameNode*		pFollowNode;
		b2MotorJoint*	pMotorJoint;
	};

	virtual void	Update();
	virtual void	Draw();

	b2Body*			CreateBody(const b2BodyDef* defBody, GameNode* pNode);
	void			DestroyBody(b2Body* pBody);

	b2Joint*		CreateJoint(const b2JointDef* defJoint);
	void			DestroyJoint(b2Joint* pJoint);

	void			CameraFollow(GameNode* pNode);

private:

	Camera*		m_pCamera;
	b2Body*		m_pOrigin;
	b2World*	m_pWorld;

	std::vector<GameSprite*>	m_vecSprite;

private:
	GameScene(b2Vec2 vec2Gravity);
	virtual ~GameScene();

	friend class GameSceneManager;
};

class GameSceneManager
{
public:
	void RegisterScene(std::string strName, GameScene::ConstructFunc funcConstructor);
	void SwitchToScene(std::string strName);

private:
	GameScene* m_pCurrentScene;

	std::unordered_map<std::string, GameScene::ConstructFunc> m_mapSceneEntry;

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