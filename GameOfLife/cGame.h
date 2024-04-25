#pragma once
#include "cCanvas.h"

class cGame
{
public:
	cGame(cCanvas* canvas);
	~cGame();

	void startGame();
	void setRunGame(bool run);
	void oneStep();

	void setHabitat(int height, int width, int* pHabitat);

	void setTime(int time);

	void back();
	void resetBack(bool resize, int height = 30, int width = 47);

private:
	cCanvas* m_Canvas = nullptr;

	int m_time = 500;

	bool runGame = true;
	int* m_pHabitat = nullptr;
	int* new_pHabitat = nullptr;
	int g_width = 47;
	int g_height = 30;
	int* neighbours = nullptr;

	bool blueActive = true;

	// stack stuff
	int** habitats = nullptr;
	int stackpointer = 0; // first write address
	int itemCount = 0; // empty
	int bufferSize = 10;

private:
	void runNextCalculation();
	void calculateNeighbours(int x, int y);

	void push(int* habitat);
	void pop();
	
};

