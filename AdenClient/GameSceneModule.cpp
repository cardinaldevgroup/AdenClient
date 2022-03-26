#include "GameSceneModule.h"

void GameScene::Update()
{
	GameInput::GetInstance().Update();

	GameKeyboardEventManager::GetInstance().Notify();
	GameMouseButtonEventManager::GetInstance().Notify();
	GameMouseMotionEventManager::GetInstance().Notify();
	GameMouseWheelEventManager::GetInstance().Notify();

	// 更新所有绑定有刚体的节点对象的 Transform
	for (b2Body* pBody = m_pWorld->GetBodyList(); pBody; pBody = pBody->GetNext())
	{
		GameNode* pNode = (GameNode*)pBody->GetUserData().pointer;
		pNode->SetPosition({ pBody->GetPosition().x, pBody->GetPosition().y });
		pNode->SetRotation(pBody->GetAngle());
	}
}

void GameScene::Draw()
{
	for (GameSprite*& pSprite : m_vecSprite)
	{
		GameSpriteManager::GetInstance().Show(pSprite);
	}
}

b2Body* GameScene::CreateBody(const b2BodyDef* defBody, GameNode* pNode)
{
	b2Body* pBody = m_pWorld->CreateBody(defBody);
	pBody->GetUserData().pointer = (uintptr_t)pNode;
}

void GameScene::DestroyBody(b2Body* pBody)
{
	m_pWorld->DestroyBody(pBody);
}

b2Joint* GameScene::CreateJoint(const b2JointDef* defJoint)
{
	b2Joint* pJoint = m_pWorld->CreateJoint(defJoint);
}

void GameScene::DestroyJoint(b2Joint* pJoint)
{
	m_pWorld->DestroyJoint(pJoint);
}

b2Joint* GameScene::CameraFollow(b2Body* pBody)
{

}

GameScene::GameScene(b2Vec2 vec2Gravity)
{
	m_pWorld = new b2World(vec2Gravity);
}

GameScene::~GameScene()
{
	delete m_pWorld;
}

GameSceneManager::GameSceneManager()
{

}

GameSceneManager::~GameSceneManager()
{

}