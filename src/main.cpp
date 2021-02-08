#include <thread>
#include <Windows.h>
#include <iostream>
#include "chess.h"
#include "render.h"

Chess::Board* p_board = new Chess::Board();


void cb()
{
	//p_board->move(rand() % 8, rand() % 8, rand() % 8, rand() % 8);
}

int main()
{
	std::thread window(render3D, 1280, 720, p_board, cb);
	window.join();
	return 0;
}