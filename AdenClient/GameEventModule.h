#ifndef _GAME_EVENT_MODULE_H_
#define _GAME_EVENT_MODULE_H_

#include <functional>

class GameEventBase
{
public:
	struct Listener
	{
		std::function<void(void*)>	funcCallback;
		uint16_t					nOrder;

		bool operator<(const Listener& compare) const {
			return nOrder < compare.nOrder;
		}
	};

	virtual void	Notify() = 0;

	Listener*		Register(std::function<void(void*)> funcCallback, uint16_t nOrder);
	void			Unregister(Listener* pListener);

private:
	class	Impl;
	Impl*	m_pImpl;

protected:
	GameEventBase();
	virtual ~GameEventBase();
};

// �����¼�
class GameKeyboard : public GameEventBase
{
public:
	struct Event
	{

	};

	virtual void	Notify();
	virtual void	PushEvent(void* pEvent);

public:
	virtual ~GameKeyboard();
	GameKeyboard(const GameKeyboard&) = delete;
	GameKeyboard& operator=(const GameKeyboard&) = delete;
	static GameKeyboard& GetInstance()
	{
		static GameKeyboard instance;
		return instance;
	}
private:
	GameKeyboard();
};

// ��갴���¼�
class GameMouseButton : public GameEventBase
{
public:
	struct Event
	{

	};

	virtual void	Notify();
	virtual void	PushEvent(void* pEvent);

public:
	virtual ~GameMouseButton();
	GameMouseButton(const GameMouseButton&) = delete;
	GameMouseButton& operator=(const GameMouseButton&) = delete;
	static GameMouseButton& GetInstance()
	{
		static GameMouseButton instance;
		return instance;
	}
private:
	GameMouseButton();
};

// ����ƶ��¼�
class GameMouseMotion : public GameEventBase
{
public:
	struct Event
	{

	};

	virtual void	Notify();
	virtual void	PushEvent(void* pEvent);

public:
	virtual ~GameMouseMotion();
	GameMouseMotion(const GameMouseMotion&) = delete;
	GameMouseMotion& operator=(const GameMouseMotion&) = delete;
	static GameMouseMotion& GetInstance()
	{
		static GameMouseMotion instance;
		return instance;
	}
private:
	GameMouseMotion();
};

// �������¼�
class GameMouseWheel : public GameEventBase
{
public:
	struct Event
	{

	};

	virtual void	Notify();
	virtual void	PushEvent(void* pEvent);

public:
	virtual ~GameMouseWheel();
	GameMouseWheel(const GameMouseWheel&) = delete;
	GameMouseWheel& operator=(const GameMouseWheel&) = delete;
	static GameMouseWheel& GetInstance()
	{
		static GameMouseWheel instance;
		return instance;
	}
private:
	GameMouseWheel();
};

// ��ײ�¼�
class GameCollision : public GameEventBase
{
public:
	struct Event
	{

	};

	virtual void	Notify();
	virtual void	PushEvent(void* pEvent);

public:
	virtual ~GameCollision();
	GameCollision(const GameCollision&) = delete;
	GameCollision& operator=(const GameCollision&) = delete;
	static GameCollision& GetInstance()
	{
		static GameCollision instance;
		return instance;
	}
private:
	GameCollision();
};

// �����豸���벢���� PushEvent
class GameInputHandler
{
public:
	void Update();

private:
	class Impl;
	Impl* m_pImpl;

public:
	virtual ~GameInputHandler();
	GameInputHandler(const GameInputHandler&) = delete;
	GameInputHandler& operator=(const GameInputHandler&) = delete;
	static GameInputHandler& GetInstance()
	{
		static GameInputHandler instance;
		return instance;
	}
private:
	GameInputHandler();
};

// ������ײ������ PushEvent
class GameCollisionHandler
{
public:
	void Update();

private:
	class Impl;
	Impl* m_pImpl;

public:
	virtual ~GameCollisionHandler();
	GameCollisionHandler(const GameCollisionHandler&) = delete;
	GameCollisionHandler& operator=(const GameCollisionHandler&) = delete;
	static GameCollisionHandler& GetInstance()
	{
		static GameCollisionHandler instance;
		return instance;
	}
private:
	GameCollisionHandler();
};

#endif // !_GAME_EVENT_MODULE_H_