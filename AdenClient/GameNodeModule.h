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
		float		fRotation;
		Point		pointScale;

		Point		pointAnchor;
		int			nZOrder;
		int			nTag;

		GameNode*	pParent;

		Def()
		{
			pointPosition = { 0.0f, 0.0f };
			fRotation = 0.0f;
			pointScale = { 1.0f, 1.0f };

			pointAnchor = { 0.5f, 0.5f };
			nZOrder = 0;
			nTag = 0;

			pParent = nullptr;
		}
	};
	
	const Point&	GetPosition();
	void			SetPosition(const Point& pointPosition);

	const float&	GetRotation();
	void			SetRotation(const float& fRotation);

	const Point&	GetScale();
	void			SetScale(const Point& pointScale);

	const Point&	GetAnchor();
	void			SetAnchor(const Point& pointAnchor);

	const int&		GetZOrder();
	void			SetZOrder(const int& nZOrder);

	const int&		GetTag();
	void			SetTag(const int& nTag);

	GameNode*		GetParent();
	void			SetParent(GameNode* pNode);

	GameNode*		GetChildHead();
	GameNode*		GetChildNext();
	GameNode*		GetChildPrev();

	void			AddChild(GameNode* pNode);
	void			RemoveChild(GameNode* pNode);

private:
	class			Impl;
	Impl*			m_pImpl;

private:
	GameNode();
	~GameNode();

	friend class GameNodeFactory;
};

class GameNodeFactory
{
public:
	GameNode*	CreateNode();
	GameNode*	CreateNode(const GameNode::Def& defNode);
	GameNode*	CloneNode(GameNode* pNode);
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