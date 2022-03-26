#ifndef _GAME_GRAPHIC_MODULE_H_
#define _GAME_GRAPHIC_MODULE_H_

#include <tuple>
#include <string>
#include <initializer_list>

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
		LIGHT,
		MONO,
		NONE
	};

	// ��ȡ����ķ��
	Style	GetStyle();
	// ��������ķ��
	void	SetStyle(Style emStyle);

	// ��ȡ�����΢��
	Hinting	GetHinting();
	// ���������΢��
	void	SetHinting(Hinting emHinting);

	// ��ȡ������־�
	int		GetKerning();
	// ����������־�
	void	SetKerning(const int& nKerning);

	// ��ȡ������ֿ��߿��
	int		GetOutline();
	// ����������ֿ��߿��
	void	SetOutline(const int& nOutlineWidth);

private:
	class	Impl;
	Impl*	m_pImpl;

public:
	GameFont();
	~GameFont();

	friend class GameGraphicManager;
};

class GameTexture
{
public:
	struct Array
	{
		GameTexture*	pTextures;
		int				nTextureCount;
	};

	int		GetWidth();
	int		GetHeight();

private:
	class	Impl;
	Impl*	m_pImpl;

public:
	GameTexture();
	~GameTexture();

	friend class GameGraphicManager;
};

class GameImage
{
public:
	enum class Flip
	{
		NONE = 0,
		HORIZONTAL,
		VERTICAL
	};

	// ���ͼƬ��֡����
	struct Frame
	{
		GameTexture*	pGameTexture;
		int				nDuration;

		~Frame() = default;
	};

	// ���ͼƬ��С
	void	GetImageSize(int& nWidth, int& nHeight);

	// ���֡�����������
	int		GetFrameCount();

	// ����ܳ���ʱ��
	int		GetDurationCount();

private:
	class	Impl;
	Impl*	m_pImpl;

private:
	GameImage();
	~GameImage();

	friend class GameGraphicManager;
};

class GameGraphicManager
{
public:
	// ʹ��·���µĸ��ļ�ֱ�ӹ����������
	GameTexture*	LoadTextureFromFile(const std::string& strFilePath);

	// ʹ�����崴��ָ����ɫ�� Solid �ı�����
	GameTexture*	CreateTextSolid(GameFont* pFont, const std::string& strText,
		uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	// ʹ�����崴��ָ����ɫ�� Blended �ı�����
	GameTexture*	CreateTextBlended(GameFont* pFont, const std::string& strText,
		uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	// ʹ�����崴��ָ����ɫ�� Shaded�ı�����
	GameTexture*	CreateTextShaded(GameFont* pFont, const std::string& strText,
		uint8_t rFore, uint8_t gFore, uint8_t bFore, uint8_t aFore, uint8_t rBack, uint8_t gBack, uint8_t bBack, uint8_t aBack);

	// ʹ��·���µĸ��ļ�ֱ�ӹ����������
	GameFont*		LoadFontFromFile(const std::string& strFilePath, const int& nFontSize);

	// ����һ����̬ͼƬ����
	GameImage*		CreateImage(GameTexture* pGameTexture);
	// ����һ����̬ͼƬ����
	GameImage*		CreateImage(const std::initializer_list<GameImage::Frame>& ilGameFrames);

	// ����һ���������
	void	DestroyTexture(GameTexture* pGameTexture);

	// ����һ���������
	void	DestroyFont(GameFont* pGameFont);

	// ����һ��ͼƬ����
	void	DestroyImage(GameImage* pGameImage);

	// ��ȡͼƬĳһ���ȶ�Ӧ����ͼ��
	// ��������ת������ͼ���Ƶ���Ļ�ϵ�ָ������������
	// @param pGameImage: ��ϷͼƬ��(��̬/��̬)
	// @param fDstX, fDstY, fDstW, fDstH: �ڴ����ϻ��Ƶľ�������
	// @param fRotation: ��ת�Ƕ�
	// @param fAnchorX, fAnchorY: ��תê�㣬����(0.5f, 0.5f)Ϊ����
	// @param emFlip: ��ת����
	// @param nProgress: ��ͼƬ��ָ�����ȿ�ʼ����
	void	DrawImage(GameImage* pGameImage,
		float fDstX, float fDstY, float fDstW, float fDstH,
		float fRotation, float fAnchorX, float fAnchorY, GameImage::Flip emFlip,
		int nProgress = 0);

	// ��ȡͼƬĳһ���ȶ�Ӧ����ͼ��
	// ��ȡ��ͼָ�����������ϵ�һ����Ƶ���Ļ��ָ�����������ϣ�
	// ��������ת���������ת
	// @param pGameImage: ��ϷͼƬ��(��̬/��̬)
	// @param nSrcX, nSrcY, nSrcW, nSrcH: ԴͼƬ�Ĳü���������
	// @param fDstX, fDstY, fDstW, fDstH: �ڴ����ϻ��Ƶľ�������
	// @param fRotation: ��ת�Ƕ�
	// @param fAnchorX, fAnchorY: ê�㣬����(0.5f, 0.5f)Ϊ����
	// @param emFlip: ��ת����
	// @param nProgress: ��ͼƬ��ָ�����ȿ�ʼ����
	void	DrawImage(GameImage* pGameImage,
		int nSrcX, int nSrcY, int nSrcW, int nSrcH,
		float fDstX, float fDstY, float fDstW, float fDstH,
		float fRotation, float fAnchorX, float fAnchorY, GameImage::Flip emFlip,
		int nProgress = 0);

	// ��ȡ���Ƶ���ɫ
	void	GetDrawColor(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);
	// ���û��Ƶ���ɫ
	void	SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	// �ڴ�������ϵ�»���һ����
	void	DrawPoint(int x, int y);

	// �ڴ�������ϵ�»���һ����
	void	DrawLine(int x1, int y1, int x2, int y2, uint8_t nWidth = 1);

	// �ڴ�������ϵ�»���һ��ֱ�Ǿ���
	void	DrawRectangle(int x, int y, int w, int h, bool isFilled = true);

	// �ڴ�������ϵ�»���һ��Բ�Ǿ���
	void	DrawRoundRectangle(int x, int y, int w, int h, int nRadius, bool isFilled = true);

	// �ڴ�������ϵ�»���һ��Բ
	void	DrawCircle(int x, int y, int nRadius, bool isFilled = true);

	// �ڴ�������ϵ�»���һ����Բ
	void	DrawEllipse(int x, int y, int nRadiusX, int nRadiusY, bool isFilled = true);

	// �ڴ�������ϵ�»���һ������
	void	DrawPie(int x, int y, int nRadius, int nAngleStart, int nAngleEnd, bool isFilled = true);

	// �ڴ�������ϵ�»���һ��������
	void	DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, bool isFilled = true);

	// �ڴ�������ϵ�»���һ�������
	void	DrawPolygon(std::initializer_list<std::tuple<int, int> > ilPoints, bool isFilled = true);

	// �ڴ�������ϵ�»���һ������������
	void	DrawBezier(std::initializer_list<std::tuple<int, int> > ilPoints, int nInterpolationCount);

	// ��յ�ǰ�����е�����ͼ��
	void	ClearWindow();
	// �������ڻ������е�ͼ��չ���ڴ�����
	void	PresentWindow();

	// ���ô��ڵĴ�С
	void	GetWindowSize(int& nWidth, int& nHeight);
	// ��ȡ���ڵĴ�С
	void	SetWindowSize(int nWidth, int nHeight);

private:
	class	Impl;
	Impl*	m_pImpl;

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