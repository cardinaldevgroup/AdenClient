#include "GameSceneModule.h"

#include "GameBlockAllocator.h"

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

void GameScene::CameraFollow(GameNode* pNode)
{
	m_pCamera->pFollowNode = pNode;
}

GameScene::GameScene(b2Vec2 vec2Gravity)
{
	m_pWorld = new b2World(vec2Gravity);

	b2BodyDef defOriginBody;
	m_pOrigin = m_pWorld->CreateBody(&defOriginBody);

	b2BodyDef defCameraBody;
	defCameraBody.type = b2_dynamicBody;
	defCameraBody.fixedRotation = true;
	defCameraBody.gravityScale = 0.0f;
	b2Body* pCameraBody = m_pWorld->CreateBody(&defCameraBody);

	b2MotorJointDef defMotorJoint;
	defMotorJoint.bodyA = m_pOrigin;
	defMotorJoint.bodyB = pCameraBody;
	defMotorJoint.maxForce = 4000.0f;
	b2MotorJoint* pMotorJoint = (b2MotorJoint*)m_pWorld->CreateJoint(&defMotorJoint);

	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Camera));
	m_pCamera = new (pMem) Camera{ pCameraBody, nullptr, pMotorJoint };
}

GameScene::~GameScene()
{
	GameBlockAllocator::GetInstance().Free(m_pCamera, sizeof(Camera));
	delete m_pWorld;
}

void GameSceneManager::RegisterScene(std::string strName, GameScene::ConstructFunc funcConstructor)
{
	m_mapSceneEntry[strName] = funcConstructor;
}

void GameSceneManager::SwitchToScene(std::string strName)
{
	if (m_pCurrentScene)
	{
		delete m_pCurrentScene;
	}
	m_pCurrentScene = m_mapSceneEntry[strName]();
}

GameSceneManager::GameSceneManager()
{
	m_pCurrentScene = nullptr;
}

GameSceneManager::~GameSceneManager()
{
	if (m_pCurrentScene)
	{
		delete m_pCurrentScene;
	}
}