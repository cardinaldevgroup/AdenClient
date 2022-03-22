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

	GameKeyboard::GetInstance().Register([](GameEventManager::BaseEvent* theEvent) {
		GameKeyboard::Event* pKey = (GameKeyboard::Event*)theEvent;
		if (pKey->emKeyCode == GameKeyboard::Code::GKEY_A)
		{
			std::cout << "test A!" << std::endl;
		}
		});

	while (true)
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		GameInput::GetInstance().Update();
		GameKeyboard::GetInstance().Notify();

		GameGraphicManager::GetInstance().ClearWindow();

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