#ifndef _GAME_NODE_MODULE_H_
#define _GAME_NODE_MODULE_H_

class GameNode
{
public:
	struct Point
	{
		float x, y;
	};

	struct Def
	{
		Point	pointPosition;
		Point	pointScale;
		int		nZOrder;
		Point	pointAnchor;
		float	fAngle;

		Def()
		{
			pointPosition = { 0.0f, 0.0f };
			pointScale = { 1.0f, 1.0f };
			nZOrder = 0;
			pointAnchor = { 0.5f, 0.5f };
			fAngle = 0.0f;
		}
	};
	
	const Point&	GetPosition();
	void			SetPosition(const Point& pointPosition);

	const Point&	GetScale();
	void			SetScale(const Point& pointScale);

	const int&		GetZOrder();
	void			SetZOrder(const int& nZOrder);

	const Point&	GetAnchor();
	void			SetAnchor(const Point& pointAnchor);

	const float&	GetAngle();
	void			SetAngle(const float& fAngle);

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameNode();
	~GameNode();

	friend class GameNodeFactory;
};

class GameNodeFactory
{
public:
	GameNode*	CreateNode();
	GameNode*	CreateNode(const GameNode::Def& defNode);
	void		DestroyNode(GameNode* pNode);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameNodeFactory();
	GameNodeFactory(const GameNodeFactory&) = delete;
	GameNodeFactory& operator=(const GameNodeFactory&) = delete;
	static GameNodeFactory& GetInstance()
	{
		static GameNodeFactory instance;
		return instance;
	}
private:
	GameNodeFactory();
};

#endif