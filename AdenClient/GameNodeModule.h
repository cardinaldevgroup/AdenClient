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
		Point		pointPosition;
		Point		pointScale;
		int			nZOrder;

		Point		pointAnchor;
		float		fAngle;

		Point		pointRelativePosition;
		float		fRelativeAngle;
		Point		pointRelativeScale;

		GameNode*	pParent;

		int			nTag;

		Def()
		{
			pointPosition = { 0.0f, 0.0f };
			pointScale = { 1.0f, 1.0f };
			nZOrder = 0;

			pointAnchor = { 0.5f, 0.5f };
			fAngle = 0.0f;

			pointRelativePosition = { 0.0f, 0.0f };
			fRelativeAngle = 0.0f;
			pointRelativeScale = { 1.0f, 1.0f };

			pParent = nullptr;

			nTag = 0;
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

	const int&		GetTag();
	void			SetTag(const int& nTag);

	const Point&	GetRelativePosition();
	void			SetRelativePosition(const Point& pointPosition);

	const float&	GetRelativeAngle();
	void			SetRelativeAngle(const float& fAngle);

	const Point&	GetRelativeScale();
	void			SetRelativeScale(const Point& pointScale);

	GameNode*		GetParent();
	void			SetParent(GameNode* pNode);

	GameNode*		GetChildHead();
	void			AddChild(GameNode* pNode);
	void			RemoveChild(GameNode* pNode);

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