#include "Platform/Platform.hpp"
#include <time.h>

using namespace sf;

void loadPosition();

int size = 56;

Sprite f[32]; //figures

//This board is talored to the figures.png
//This can be changed depending on the pieces array
int board[8][8] =
	{ -1,-2,-3,-4,-5,-3,-2,-1,
	  -6,-6,-6,-6,-6,-6,-6,-6,
	   0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0,
	   6, 6, 6, 6, 6, 6, 6, 6,
	   1, 2, 3, 4, 5, 3, 2, 1};


void loadPosition()
{
	int k = 0;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			int n = board[i][j];

			if(!n) continue;

			int x = abs(n) - 1;
			int y = n > 0 ? 1:0;
			f[k].setTextureRect(IntRect(size * x, size * y, size, size));
			f[k].setPosition(size * j, size * i);
			k++;
		}
	}
}



int main()
{
	util::Platform platform;

	RenderWindow window(VideoMode(454, 455), "Chess Simulator");

	Texture t1, t2;
	t1.loadFromFile("images/figures.png");
	t2.loadFromFile("images/board0.png");

	Sprite s(t1);
	Sprite sBoard(t2);

	for(int i = 0; i < 32; i++) f[i].setTexture(t1);

	loadPosition();

	bool isMove = false;
	float dx = 0, dy = 0;
	int n = 0;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			/////drag and drop/////
			if (e.type == Event::MouseButtonPressed)
			{
				if (e.mouseButton.button == Mouse::Left)
				{
					for(int i = 0; i < 32; i++)
					{
						if (f[i].getGlobalBounds().contains(pos.x, pos.y))
						{
							isMove = true;
							n = i;
							dx = pos.x - f[i].getPosition().x;
							dy = pos.y - f[i].getPosition().y;
						}
					}
				}
			}
			if (e.type == Event::MouseButtonReleased)
			{
				if (e.mouseButton.button == Mouse::Left)
				{
					isMove = false;
				}
			}
		}

		if (isMove)
			f[n].setPosition(pos.x - dx, pos.y - dy);

		///////draw////////
		window.clear();
		window.draw(sBoard);
		for (int i = 0; i < 32; i++)
		{
			window.draw(f[i]);
		}

		window.display();
	}

	// sf::RenderWindow window;
	// // in Windows at least, this must be called before creating the window
	// float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// // Use the screenScalingFactor
	// window.create(sf::VideoMode(200.0f * screenScalingFactor, 200.0f * screenScalingFactor), "SFML works!");
	// platform.setIcon(window.getSystemHandle());

	// sf::CircleShape shape(window.getSize().x / 2);
	// shape.setFillColor(sf::Color::White);

	// sf::Texture shapeTexture;
	// shapeTexture.loadFromFile("content/sfml.png");
	// shape.setTexture(&shapeTexture);

	// sf::Event event;

	// while (window.isOpen())
	// {
	// 	while (window.pollEvent(event))
	// 	{
	// 		if (event.type == sf::Event::Closed)
	// 			window.close();
	// 	}

	// 	window.clear();
	// 	window.draw(shape);
	// 	window.display();
	// }

	return 0;
}
