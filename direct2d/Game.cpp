#include "Game.h"
#include "SpriteSheet.h"
#include <ctime>
#include "Bunny.h"
#include "Carrot.h"
#include "Fox.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "Shroom.h"

Game::Game(Graphics * gfx) :
	gfx(gfx)
{

	//-------Sprites-----------
	water = new Water(gfx, 5);
	fox = new Fox(gfx);
	carrot = new Carrot(gfx, L"carrot.png");
	bunny = new Bunny(gfx);
	obj = new Obstacle(gfx);
	background = new Background(gfx);
	shroom = new Shroom(gfx);
	//-------------------------


	//---------Read File----------------

	infile.open("Highscore.txt");
	int compare = 0;
	while (std::getline(infile, highscoreString)) {
		compare = std::stoi(highscoreString, nullptr, 10);
		if (compare >= highscoreInt) {
			highscoreInt = compare;
		}
	}
	outfile.open("Highscore.txt");
	outfile << highscoreInt << std::endl;

	swprintf_s(highscoreWchar, L"%d", highscoreInt);
	//----------------------------------

}

Game::~Game()
{
	delete shroom;
	delete water;
	delete carrot;
	delete fox;
	delete background;
	delete bunny;
	delete obj;
	outfile.close();
}

void Game::Run()
{
	gfx->BeginDraw();
	if ((std::clock() - clockR) / (double)CLOCKS_PER_SEC >= refreshRate)
		UpdateModel();
	if ((std::clock() - clockFPS) / (double)CLOCKS_PER_SEC >= frameRate)
		ComposeFrame();
	gfx->EndDraw();
	
	/*
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	*/
}

void Game::UpdateModel()
{
	if (bunny->isDead()) {
		speed = 1.0;
	}

	if (!bunny->isDead())
	{

		if (abs(obj->returnPos().left - carrot->returnPos().left) < 100) {
			carrot->renew();
		}

		if (GetAsyncKeyState(VK_DOWN) && !bunny->isClouded()) {
			bunny->crouch();
		}

		if (GetAsyncKeyState(VK_UP) && bunny->onGround() && !bunny->isClouded())
			charge += 1.5;

		if (GetAsyncKeyState(VK_UP) && !bunny->onGround() && carrots && (std::clock() - carrotsTimer) / (double)CLOCKS_PER_SEC >= 0.5f && !bunny->isClouded()) {
			carrotsTimer = clockR;
			carrots--;
			charge = 0;
			bunny->jump(30);
		}

		if (!GetAsyncKeyState(VK_UP) && charge != 0 && bunny->onGround() && !bunny->isClouded()) {
			bunny->jump(charge);
			charge = 0;
		}

		if (checkCollision(bunny->returnPos(), obj->returnPos())) {
			bunny->die();
			distanceCount -= speed;
			updateHighscore();
		}

		if (checkCollision(obj->returnPos(), shroom->returnPos())) {
			obj->renew();
		}

		if (checkCollision(bunny->returnPos(), fox->returnPos())) {
			bunny->die();
			distanceCount -= speed;
			updateHighscore();
		}

		if (checkCollision(bunny->returnPos(), carrot->returnPos())) {
			if (speed <= 3.0)
				speed += 0.1;
			carrots++;
			carrot->renew();
		}

		if (checkCollision(bunny->returnPos(), shroom->returnPos())) {
			if (shroom->isBroom()) {
				bunny->jump(60, false);
			}
			else {
				bunny->jump(60, false);
				bunny->getClouded();
			}
		}

		if (checkCollision(fox->returnPos(), obj->returnPos())) {
			fox->changeDir();
		}

		swprintf_s(distanceCountText, L"%d", (int)distanceCount);
		swprintf_s(carrotCountText, L"%d", carrots);

		distanceCount += speed;

		carrot->update(speed);
		fox->update(speed);
		obj->update(speed);
		bunny->updateBunny(speed);
		background->update(speed);
		shroom->update(speed);

		clockR = std::clock();
	}
	else if (bunny->isDead()) {
		bunny->updateBunny(speed);
	}

	if (GetAsyncKeyState(VK_F9)) {
		Sleep(10);
		water = new Water(gfx, 5);
		bunny = new Bunny(gfx);
		obj = new Obstacle(gfx);
		carrot = new Carrot(gfx, L"carrot.png");
		fox = new Fox(gfx);
		speed = 1.0f;
		carrots = 0;
		distanceCount = 0.0f;
	}

}

void Game::ComposeFrame()
{
	gfx->ClearScreen(255, 255, 255);

	background->draw();
	obj->show();
	shroom->show();
	water->showWaterArea(bottom, speed);

	fox->show();
	obj->show();

	//gfx->DrawLine(0, 435, 1600, 435);

	carrot->show();

	/*
	D2D1_RECT_F a = bunny->returnPos();
	D2D1_RECT_F b = shroom->returnPos();
	gfx->DrawRectangle(a);
	gfx->DrawRectangle(b);
	*/

	if (bunny->isCrouched())
		gfx->DrawTEXT(&D2D1::Rect(50, 500, 500, 500), 50, L"Crouched");

	bunny->showBunny(carrots > 0);

	gfx->DrawTEXT(&D2D1::Rect(50, 10, 500, 500), 50, L"Score:");
	gfx->DrawTEXT(&D2D1::Rect(250, 10, 500, 500), 50, distanceCountText);

	//mbstowcs(outpuet, output, strlen(output + 1));
	//if((std::stoi(highscore)) >=0 )
	gfx->DrawTEXT(&D2D1::Rect(1100, 10, 1600, 500), 50, L"Highscore:");
	gfx->DrawTEXT(&D2D1::Rect(1300, 10, 1600, 500), 50, highscoreWchar);
	gfx->DrawTEXT(&D2D1::Rect(50, 60, 500, 500), 50, L"Carrots:");
	gfx->DrawTEXT(&D2D1::Rect(250, 60, 500, 500), 50, carrotCountText);

	clockFPS = std::clock();
	//}

}

void Game::updateHighscore()
{
	distanceCount += 0.5;

	if ((int)distanceCount >= highscoreInt) {
		outfile << (int)distanceCount << std::endl;
		highscoreInt = (int)distanceCount;
	}

	swprintf_s(highscoreWchar, L"%d", highscoreInt);

	outfile.flush();
}

bool Game::checkCollision(D2D1_RECT_F a, D2D1_RECT_F b)
{
	return (b.top <= a.bottom && b.bottom >= a.top && b.left <= a.right && b.right >= a.left);
}
