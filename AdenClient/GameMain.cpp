#include <Windows.h>

#include "GameSpriteModule.h"

#include <chrono>
#include <thread>

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
	test2.pNode->SetRelativePosition({2.0f, 2.0f});

	test1.pNode->SetRotation(0.5f);

	std::chrono::duration<double> dFrameTime(0.0);
	std::chrono::duration<double> dSleepAdjust(0.0);

	while (true)
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		GameGraphicManager::GetInstance().ClearWindow();
		GameSpriteManager::GetInstance().Show(test1.pSprite);
		GameSpriteManager::GetInstance().Show(test2.pSprite);
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