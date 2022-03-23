#include <Windows.h>

#include "GameSpriteModule.h"
#include "GameInputModule.h"
#include "GameEventModule.h"

#include <chrono>
#include <thread>

#include <iostream>

class AdenPlayer
{
public:
	GameNode*	pNode;
	GameSprite* pSprite;

public:
	AdenPlayer()
	{
		pNode = GameNodeFactory::GetInstance().CreateNode();
		pNode->SetScale({ 0.2f, 0.2f });

		GameTexture* pTexture1 = GameGraphicManager::GetInstance().LoadTextureFromFile("resource//1.png");
		GameTexture* pTexture2 = GameGraphicManager::GetInstance().LoadTextureFromFile("resource//2.png");
		GameTexture* pTexture3 = GameGraphicManager::GetInstance().LoadTextureFromFile("resource//3.png");

		GameImage* pImage = GameGraphicManager::GetInstance().CreateImage({ { pTexture1, 5 }, { pTexture2, 5 }, { pTexture3, 5 } });

		pSprite = GameSpriteManager::GetInstance().CreateSprite(pNode, pImage);
	}
};

#undef main
int main(int argc, char* argv[])
{
	AdenPlayer test1;
	AdenPlayer test2;

	test2.pNode->SetParent(test1.pNode);
	test2.pNode->SetRelativePosition({5.0f, 5.0f});
	test2.pNode->SetRelativeRotation(3.14159f / 2);

	std::chrono::duration<double> dFrameTime(0.0);
	std::chrono::duration<double> dSleepAdjust(0.0);

	float fRotation = 0.0f;

	GameKeyboardEventManager::GetInstance().Register([](GameEvent* theEvent) {
		GameKeyboardEvent* pKey = (GameKeyboardEvent*)theEvent;
		if (pKey->emKeyCode == GameKeyboardEvent::Code::GKEY_A && pKey->emType == GameEvent::Type::KEY_UP)
		{
			std::cout << "test A!" << std::endl;
		}
		});

	while (true)
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		GameInput::GetInstance().Update();
		GameKeyboardEventManager::GetInstance().Notify();

		GameGraphicManager::GetInstance().ClearWindow();

		GameSpriteManager::GetInstance().Show(test1.pSprite);
		GameSpriteManager::GetInstance().Show(test2.pSprite);

		fRotation += 0.01f;
		test1.pNode->SetRotation(fRotation);

		GameGraphicManager::GetInstance().PresentWindow();

		// 算出应当休眠的时间
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> dTarget(1.0 / 60.0);
		std::chrono::duration<double> dTimeUsed = t2 - t1;
		std::chrono::duration<double> dSleepTime = dTarget - dTimeUsed + dSleepAdjust;
		if (dSleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(dSleepTime);
		}

		// 根据直到当前的实际帧数和预期帧数的差来调整下一帧的休眠时间
		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		dFrameTime = t3 - t1;
		dSleepAdjust = 0.9 * dSleepAdjust + 0.1 * (dTarget - dFrameTime);
	}

	return 0;
}