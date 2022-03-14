#include "GameGraphicModule.h"
#include "GameMediaModule.h"

#include <Windows.h>

#undef main
int main(int argc, char* argv[])
{
	GameMusic* pMusic = GameMediaManager::GetInstance().LoadMusicFromFile("bgm.mp3");
	GameMediaManager::GetInstance().PlayMusic(pMusic, 1);

	system("pause");

	return 0;
}