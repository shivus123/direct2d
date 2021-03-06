// Diese Programm macht die Hindernisse: Baumst�mme und Steinen.

#include "obstacle.h"
//Zufallgenerator
#include <random>



Obstacle::Obstacle(Graphics * gfx):
	gfx(gfx)
{
	//	Die H�hen der Elemente werden gespeichert
	spriteHeights.push_back(547);	//	Stone
	spriteHeights.push_back(403);	//	Trunk
	spriteHeights.push_back(320);	//	Stone2

	renew();
}

Obstacle::Obstacle(Graphics * gfx, LPCWSTR name):
	gfx(gfx)
{
	//	Konstruktor der Karotte
	ownSprite = true;								//	Zeigt an, dass es sich um Karotte handelt
	sprite = new SpriteSheet(name, gfx, 1.0f);		//	Bild der Karotte
	//	Andere, fixe Gr�ssen
	height = 80;
	width = 80;
}

Obstacle::~Obstacle()
{
	//	Aufr�umen
	delete stone;
	delete stone2;
	delete trunk;
	delete sprite;
	delete &rect;
	delete gfx;
}

void Obstacle::show()
{
	//	Je nach dem welches Objekt existiert wird dieses dargestellt
	if (stone != NULL)
		stone->Draw(x, y - height + 50*size, size);
	if (stone2 != NULL)
		stone2->Draw(x, y - height + 170*size, size);
	if (trunk != NULL)
		trunk->Draw(x, y - height - 50*size, size);
	// gfx->DrawRectangle(rect);
}

void Obstacle::update(double speed)
{
	//	X Wert wird passend zur Spielgeschwindigkeit angepasst
	x -= speed*5;

	//	Falls ausserhalb
	if (x + width <= 0) {
		renew();
	}

	//	Die verschiedenen Rechtecke der jeweiligen Positionen der Objekte
	if (!ownSprite) {
		if(stone)
			rect = { x + 210 * size , y - height +  100 * size, x + width - 200 * size, y };
		if(stone2)
			rect = { x + 150 * size , y - height + 230 * size, x + width - 150 * size, y };
		if(trunk)
			rect = { x + 230 * size , y - height + 100 * size, x + width - 170 * size, y };
	}
	else {	//	Karotte
		rect = { x, (y - height / 2) - 200 , x + width, (y + height / 2) - 200 };
	}

}

void Obstacle::renew()
{
	//	Zuf�llige Zahl
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1800, 2400);

	float heighta = height;	//	Zwischenspeicher der alten H�he damit diese nicht wiederverwendet wird
	
	//	Anpassen der Gr�sse nur falls es keine Karotte ist
	if (!ownSprite) {

		height = 900;

		//	Damit nicht zu gross
		while (height > 300) {
			//	Zuf�llig wird eines der Objekte ausgew�hlt und erstellt
			switch (dist(rng) % 3) {
			case 0:
				size = float(dist(rng) % 5) / 10 + 0.25f;
				stone = new SpriteSheet(L"stone.png", gfx, 1.0f);
				width = 640 * size;
				height = spriteHeights[0] * size;
				stone2 = NULL;
				trunk = NULL;
				break;
			case 1:
				size = float(dist(rng) % 6) / 10 + 0.3f;
				height = spriteHeights[1] * size;
				stone = NULL;
				width = 640 * size;
				stone2 = new SpriteSheet(L"stone2.png", gfx, 1.0f);
				trunk = NULL;
				break;
			case 2:
				size = float(dist(rng) % 3) / 10 + 0.15f;
				width = 840 * size;
				height = spriteHeights[2] * size;
				stone = NULL;
				stone2 = NULL;
				trunk = new SpriteSheet(L"trunk.png", gfx, 1.0f);
				break;
			default:
				break;
				//EEEEEEEEEERROR
			}
		}

	}
	//	Zuf�llige Position auf der x-Achse
	x = dist(rng) - width;
}

D2D_RECT_F Obstacle::returnPos()
{
	/*int a = rect.bottom;
	rect.bottom = rect.top;
	rect.top = a;
	*/
	return rect;
}

int setX(Obstacle &obj)
{
	//	Das Objekt kriegt eine zuf�llige Position und 1 oder 0 wird zur�ckgegeben (entscheidet dar�ber, welcher Pilz dargestellt wird)
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1800*3, 2400*3);

	obj.x = dist(rng);

	return dist(rng) % 2;
}
