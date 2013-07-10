#include "globals.h"

gpc_polygon		oPolyLevel;
gpc_tristrip	oTristripLevel;

TextureIDs_t oTextureIDs;

void GameDataLoad()
{
	// load all textures
	GameDataLoadTextures();

	// DEBUG - temporarily just open a level right away
	GameDataOpenLevel("levels/test3.wwl");
	//GameDataOpenLevel("levels/test_orientation.wwl");
}

void GameDataUnload()
{
	// close the level and free all memory
	GameDataEndLevel();

	// unload all textures
	DESTROY_TEXMANAGER
}

// load all textures
bool GameDataLoadTextures()
{
	// load 1st font texture
	oTextureIDs.iFonts[0] = TEXMANAGER.LoadTexture("data/fonts/font1.tga");
	oTextureIDs.iFloor = TEXMANAGER.LoadTexture("data/textures/floor.tga");
	// ...

	// no error checking just yet
	return true;
}

// load a level in memory
void GameDataOpenLevel(char *chFileName)
{
	// close previous level
	GameDataEndLevel();

	if (oFile = fopen(chFileName, "r"))
	{
		gpc_read_polygon(oFile, 0, &oPolyLevel);
		gpc_polygon_to_tristrip(&oPolyLevel, &oTristripLevel);
		fclose(oFile);
	}
	else
	{
		// level not found
		sTempString = (string)"an eX0 level file \'" + chFileName + "\' could not be opened.";
#ifdef WIN32
		MessageBox(NULL, sTempString.c_str(), "eX0 error", MB_ICONERROR);
#else
		printf(sTempString.c_str());
#endif
		Terminate(1);
	}

	// DEBUG: Print info about the level, namely the tristrip vertex count
	printf("opened level %s with %d tristrips\n", chFileName, oTristripLevel.num_strips);
	int nTotalTriangles = 0;
	for (int nLoop1 = 0; nLoop1 < oTristripLevel.num_strips; ++nLoop1) {
		nTotalTriangles += oTristripLevel.strip[nLoop1].num_vertices - 2;
	}
	printf("total triangle count = %d\n", nTotalTriangles);
}

// close currently opened level, free memory, reset vars
void GameDataEndLevel()
{
	gpc_free_polygon(&oPolyLevel);
	gpc_free_tristrip(&oTristripLevel);
}