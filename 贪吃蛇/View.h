#pragma once
typedef struct UI {
	int gameZoneWidth;
	int gameZoneHeight;

	int marginTop;
	int marginLeft;

	char* snakeBlock;
	char* foodBlock;
	char* wallBlock;
	int blockWidth;
} UI;

void UIEndGame(const UI* pUI, int x, int y, const char* message);

void UIInitialize(UI *pUI, int gameZoneWidth, int gameZoneHeight);

void UIDisplayFoodBlockAtXY(const UI* pUI, int x, int y);

void UICleanBlockAtXY(const UI* pUI, int x, int y);

void UIDisplaySnakeBlockAtXY(const UI* pUI, int x, int y);

void UIDestroy(UI *pUI);

void UIDisplayWizard(const UI *pUI);

void UIDisplayGameZone(const UI *pUI);
