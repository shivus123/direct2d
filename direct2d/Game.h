#pragma once

#include "Graphics.h"
#include "SpriteSheet.h"
#include <ctime>
#include "Bunny.h"
#include "obstacle.h"
#include "Carrot.h"
#include "Fox.h"
#include "Water.h"

class Game {

	Fox* fox;
	Water* water;
	Carrot* carrot;
	SpriteSheet* sprites;
	Bunny* bunny;
	Graphics* gfx;
	Obstacle* obj;

public:
	
	Game(Graphics* gfx);
	~Game();

	void Run();
	void UpdateModel();
	void ComposeFrame();

private:

	double xScreen1, xScreen2, xScreen3;


	D2D1_RECT_F bottom = D2D1::Rect(0, 400, 1600, 600);
	D2D1_COLOR_F color = D2D1::ColorF(50, 50, 255);

	int carrots = 0;
	double speed = 1.0;
	double refreshRate = 1/30;
	std::clock_t clock;
	double charge = 0;
	bool checkCollision(D2D1_RECT_F rect1, D2D1_RECT_F rect2); 

};