#ifndef _GAME_INPUT_MODULE_H_
#define _GAME_INPUT_MODULE_H_

// 负责处理输入事件并 PushEvent
class GameInput
{
public:
	void Update();

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameInput();
	GameInput(const GameInput&) = delete;
	GameInput& operator=(const GameInput&) = delete;
	static GameInput& GetInstance()
	{
		static GameInput instance;
		return instance;
	}
private:
	GameInput();
};

#endif // !_GAME_INPUT_MODULE_H_