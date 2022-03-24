#ifndef _GAME_GRAPHIC_MODULE_H_
#define _GAME_GRAPHIC_MODULE_H_

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

	// 获取字体的风格
	Style	GetStyle();
	// 设置字体的风格
	void	SetStyle(Style emStyle);

	// 获取字体的微调
	Hinting	GetHinting();
	// 设置字体的微调
	void	SetHinting(Hinting emHinting);

	// 获取字体的字距
	int		GetKerning();
	// 设置字体的字距
	void	SetKerning(const int& nKerning);

	// 获取字体的轮框线宽度
	int		GetOutline();
	// 设置字体的轮框线宽度
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

class GameImage
{
public:
	enum class Flip
	{
		NONE = 0,
		HORIZONTAL,
		VERTICAL
	};

	// 组成图片的帧纹理
	struct Frame
	{
		GameTexture*	pGameTexture;
		int				nDuration;

		~Frame() = default;
	};

	// 获得图片大小
	void	GetImageSize(int& nWidth, int& nHeight);

	// 获得帧纹理的总数量
	int		GetFrameCount();

	// 获得总持续时间
	int		GetDurationCount();

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameImage();
	~GameImage();

	friend class GameGraphicManager;
};

class GameGraphicManager
{
public:
	// 使用路径下的该文件直接构造纹理对象
	GameTexture*	LoadTextureFromFile(const std::string& strFilePath);

	// 使用路径下的该文件直接构造字体对象
	GameFont*		LoadFontFromFile(const std::string& strFilePath, const int& nFontSize);

	// 创建一个静态图片对象
	GameImage*		CreateImage(GameTexture* pGameTexture);
	// 创建一个动态图片对象
	GameImage*		CreateImage(const std::initializer_list<GameImage::Frame>& ilGameFrames);

	// 析构一个纹理对象
	void			DestroyTexture(GameTexture* pGameTexture);
	// 析构一个字体对象
	void			DestroyFont(GameFont* pGameFont);
	// 析构一个图片对象
	void			DestroyImage(GameImage* pGameImage);

	// 读取图片某一进度对应的贴图，
	// 并按照旋转规则将贴图绘制到屏幕上的指定矩形区域上
	// @param pGameImage: 游戏图片类(静态/动态)
	// @param fDstX, fDstY, fDstW, fDstH: 在窗口上绘制的矩形区域
	// @param fRotation: 旋转角度
	// @param fAnchorX, fAnchorY: 旋转锚点，其中(0.5f, 0.5f)为中心
	// @param emFlip: 翻转类型
	// @param nProgress: 从图片的指定进度开始绘制
	void			Draw(GameImage* pGameImage,
		const float& fDstX, const float& fDstY, const float& fDstW, const float& fDstH,
		const float& fRotation, const float& fAnchorX, const float& fAnchorY, GameImage::Flip emFlip,
		int nProgress = 0);

	// 读取图片某一进度对应的贴图，
	// 裁取贴图指定矩形区域上的一块绘制到屏幕的指定矩形区域上，
	// 并按照旋转规则进行旋转
	// @param pGameImage: 游戏图片类(静态/动态)
	// @param nSrcX, nSrcY, nSrcW, nSrcH: 源图片的裁剪矩形区域
	// @param fDstX, fDstY, fDstW, fDstH: 在窗口上绘制的矩形区域
	// @param fRotation: 旋转角度
	// @param fAnchorX, fAnchorY: 锚点，其中(0.5f, 0.5f)为中心
	// @param emFlip: 翻转类型
	// @param nProgress: 从图片的指定进度开始绘制
	void			Draw(GameImage* pGameImage,
		const int& nSrcX, const int& nSrcY, const int& nSrcW, const int& nSrcH,
		const float& fDstX, const float& fDstY, const float& fDstW, const float& fDstH,
		const float& fRotation, const float& fAnchorX, const float& fAnchorY, GameImage::Flip emFlip,
		int nProgress = 0);

	// 清空当前窗口中的所有图像
	void			ClearWindow();

	// 将绘制在缓存区中的图像展现在窗口上
	void			PresentWindow();

	void			GetWindowSize(int& nWidth, int& nHeight);

	void			SetWindowSize(int nWidth, int nHeight);

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