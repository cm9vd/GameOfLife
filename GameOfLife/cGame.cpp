#include "cGame.h"

#include<thread>
#include<chrono>

cGame::cGame(cCanvas* canvas)
{
	m_Canvas = canvas;

	habitats = new int* [bufferSize];
	for (int i = 0; i < bufferSize; i++) {
		habitats[i] = new int[g_height * g_width];
	}

}

cGame::~cGame()
{
	for (int i = 0; i < bufferSize; i++) {
		delete[] habitats[i];
	}
	delete[] habitats;
}

void cGame::setHabitat(int height, int width, int* pHabitat)
{
	m_pHabitat = pHabitat;
	g_height = height;
	g_width = width;
}

void cGame::setTime(int time)
{
	m_time = time;
}

void cGame::back()
{
	// pop from habitat stack (habitats) and show
	pop();
}

void cGame::resetBack(bool resize, int height, int width)
{
	if (resize) {
		for (int i = 0; i < bufferSize; i++) {
			delete[] habitats[i];
		}
		delete[] habitats;

		habitats = new int* [bufferSize];
		for (int i = 0; i < bufferSize; i++) {
			habitats[i] = new int[height * width];
		}
	}
	// set stack to empty
	stackpointer = 0;
	itemCount = 0;
}

void cGame::startGame()
{
	while (runGame) {				

		runNextCalculation();

		// show next calculation
		m_Canvas->SetHabitatData(g_height, g_width, m_pHabitat);

		//wait
		for (int i = 0; i < m_time / 10; i++) {
			if (runGame) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	}
	// wait for cMain before exit
	std::this_thread::sleep_for(std::chrono::milliseconds(60));
}

void cGame::setRunGame(bool run)
{
	runGame = run;
}

void cGame::oneStep()
{
	runNextCalculation();

	// show next calculation
	m_Canvas->SetHabitatData(g_height, g_width, m_pHabitat);
}

void cGame::runNextCalculation()
{
	int cell = 0;
	int neighbourCount;
	int totalCellCount = 0;

	new_pHabitat = new int[g_height * g_width]{ 0 };

	for(int y = 0; y < g_height; y++)
		for (int x = 0; x < g_width; x++) {

			// get next cell
			cell = m_pHabitat[y * g_width + x];
			// invalid access protection on delete
			if (cell < 0) {
				cell = 0;
			}

			totalCellCount = totalCellCount + cell;

			// check if blue active
			if (cell == 5 && !blueActive) {
				new_pHabitat[y * g_width + x] = cell;
				continue;
			}

			// fresh for every cell
			neighbourCount = 0;
			neighbours = new int[8]{ 0 };
			int* colors = new int[9]{ 0 };

			

			calculateNeighbours(y, x);

			// analyze neighbours
			for (int i = 0; i < 8; i++) {
				colors[neighbours[i]]++;
				if (neighbours[i] != 0 && neighbours[i] != 2) {
					neighbourCount++;
				}
			}

			if (cell > 2) {
				// cell is alive

				// green
				if(cell == 4 && 1 < neighbourCount && neighbourCount < 4) {
					new_pHabitat[y * g_width + x] = cell; 

					// purple infection / take over
					if (colors[6] != 0) {
						new_pHabitat[y * g_width + x] = 6;
					}

				// default
				} else if ((colors[cell] == 2 || colors[cell] == 3) && neighbourCount <= 3) {
					// cell survives
					new_pHabitat[y * g_width + x] = cell;

					// purple infection / take over
					if (colors[6] != 0) {
						new_pHabitat[y * g_width + x] = 6;
					}
				}

				// red kill
				if (colors[3] != 0 && cell != 3) {
					new_pHabitat[y * g_width + x] = 0;
				}

			}

			// black or white (overwrites if necessary)
			if (cell == 1 || cell == 2) {
				if (1 < colors[cell] && colors[cell] < 4) {
					new_pHabitat[y * g_width + x] = cell;
				}
			}


			// dead or white
			if (cell == 0 || cell == 2) {

				// check for white
				if (colors[2] == 3) {
					new_pHabitat[y * g_width + x] = 2;
				}

				// default
				if (neighbourCount == 3) {
					// check for parent color
					for (int i = 1; i < 9; i++) {
						if (colors[i] == 3) {
							new_pHabitat[y * g_width + x] = i;

							// set back if blue and not active
							if (i == 5 && !blueActive) {
								new_pHabitat[y * g_width + x] = cell;
							}
						}
					}
				}
			}

			// black take over
			if (colors[1] == 3) {  // black
				new_pHabitat[y * g_width + x] = 1;
			}
			
			

			// clean up
			delete[] neighbours;
			delete[] colors;
		}

	// blue
	blueActive = !blueActive;

	if (totalCellCount == 0) {
		runGame = false;
	}

	// pass result
	push(m_pHabitat);
	memcpy(m_pHabitat, new_pHabitat, g_height * g_width * sizeof(int));
	delete[] new_pHabitat;
	
}

void cGame::calculateNeighbours(int y, int x)
{
	int temp_y = 0;
	int temp_x = 0;
	int temp = 0;

	for (int i = 0; i < 8; i++) {
		switch (i)
		{
		case 0:
			temp_y = y - 1;
			temp_x = x - 1;
			break;
		case 1:
			temp_y = y - 1;
			temp_x = x;
			break;
		case 2:
			temp_y = y - 1;
			temp_x = x + 1;
			break;
		case 3:
			temp_y = y;
			temp_x = x - 1;
			break;
		case 4:
			temp_y = y;
			temp_x = x + 1;
			break;
		case 5:
			temp_y = y + 1;
			temp_x = x - 1;
			break;
		case 6:
			temp_y = y + 1;
			temp_x = x;
			break;
		case 7:
			temp_y = y + 1;
			temp_x = x + 1;
			break;
		}

		if ((0 <= temp_y) && (temp_y < g_height) && (0 <= temp_x) && (temp_x < g_width)) {

			temp = m_pHabitat[temp_y * g_width + temp_x];
			if (temp > -1) {
				neighbours[i] = temp;
			}
		}
	}
}

void cGame::push(int* habitat)
{
	memcpy(habitats[stackpointer], habitat, g_height * g_width * sizeof(int));

	stackpointer = (stackpointer + 1) % bufferSize;
	
	if (itemCount < bufferSize) {
		itemCount++;
	}
}

void cGame::pop()
{
	if (itemCount > 0) {
		stackpointer = (stackpointer + bufferSize - 1) % bufferSize;

		memcpy(m_pHabitat, habitats[stackpointer], g_height * g_width * sizeof(int));
		m_Canvas->SetHabitatData(g_height, g_width, m_pHabitat);

		itemCount--;
		
	}
}
