#ifndef _GAME_GRAPHIC_MODULE_H_
#define _GAME_GRAPHIC_MODULE_H_

#include <string>
#include <initializer_list>

class GameTexture
{
public:
	int	GetWidth();
	int	GetHeight();

private:
	class Impl;
	Impl* m_pImpl;

public:
	GameTexture();
	~GameTexture();

	friend class GameGraphicManager;
};

class GameFont
{
public:
	enum class Style
	{
		NORMAL = 0x00,
		BOLD = 0x01,
		ITALIC = 0x02,
		UNDERLINE = 0x04,
		STRIKETHROUGH = 0X08
	};

	enum class Hinting
	{
		NORMAL = 0,
		LIGHT = 1,
		MONO = 2,
		NONE = 3
	};

	/* ��ȡ����ķ��
	*/
	Style	GetStyle();
	/* ��������ķ��
	*/
	void	SetStyle(Style emStyle);

	/* ��ȡ�����΢��
	*/
	Hinting	GetHinting();
	/* ���������΢��
	*/
	void	SetHinting(Hinting emHinting);

	/* ��ȡ������־�
	*/
	int		GetKerning();
	/* ����������־�
	*/
	void	SetKerning(int nKerning);

	/* ��ȡ������ֿ��߿��
	*/
	int		GetOutline();
	/* ����������ֿ��߿��
	*/
	void	SetOutline(int nOutlineWidth);

private:
	class Impl;
	Impl* m_pImpl;

public:
	GameFont();
	~GameFont();

	friend class GameGraphicManager;
};

class GameImage
{
public:
	struct Def
	{
		GameTexture*	pGameTexture;
		int				nDuration;
	};

private:
	class Impl;
	Impl* m_pImpl;

public:
	GameImage();
	GameImage(Def& def);
	~GameImage();

	friend class GameGraphicManager;
};

class GameGraphicManager
{
public:
	GameTexture*	LoadTextureFromFile(std::string strFilePath);
	GameFont*		LoadFontFromFile(std::string strFilePath, int nFontSize);

	GameImage*		CreateStaticImage(GameTexture* pGameTexture);
	GameImage*		CreateDynamicImage(std::initializer_list<GameImage::Def>& ilGameImages);

	void			DestroyTexture(GameTexture* pGameTexture);
	void			DestroyFont(GameFont* pGameFont);
	void			DestroryImage(GameImage* pGameImage);

	void			Render();

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameGraphicManager();
	GameGraphicManager(const GameGraphicManager&) = delete;
	GameGraphicManager& operator=(const GameGraphicManager&) = delete;
	static GameGraphicManager& GetInstance()
	{
		static GameGraphicManager instance;
		return instance;
	}
private:
	GameGraphicManager();
};

#endif // !_GAME_GRAPHIC_MODULE_H_