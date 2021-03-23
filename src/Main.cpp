#include "Platform/Platform.hpp"
#include "Connector.hpp"
#include <time.h>
#include <string>

using namespace sf;
using namespace std;

string toChessNote(Vector2f);
Vector2f toCoord(char, char);
void move(string);
void loadPosition();

// Vector2f offset(28,28);

string position = "";

//You can't use size or SIZE because these are declared else where in this program
int sizes = 56;

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


string toChessNote(Vector2f p)
{
	string s = "";
	s += char(p.x / sizes + 97);
	s += char(7 - p.y / sizes + 49);
	return s;
}

Vector2f toCoord(char a, char b)
{
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;
	return Vector2f(x * sizes, y * sizes);
}

void move(string str)
{
	Vector2f oldPos = toCoord(str[0], str[1]);
	Vector2f newPos = toCoord(str[2], str[3]);

	for(int i = 0; i < 32; i++)
	{
		if(f[i].getPosition() == newPos)
		{
			f[i].setPosition(-100,-100);
		}
	}

	for(int i = 0; i < 32; i++)
	{
		if(f[i].getPosition() == oldPos)
		{
			f[i].setPosition(newPos);
		}
	}

	// Castling  THIS IS NOT COMPLETED
	// if(str == "e1g1") if (position.find("e1") == 1) move("h1f1");
	// if (str=="e8g8") if (position.find("e8")==-1) move("h8f8");
	// if (str=="e1c1") if (position.find("e1")==-1) move("a1d1");
	// if (str=="e8c8") if (position.find("e8")==-1) move("a8d8");

}

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
			f[k].setTextureRect(IntRect(sizes * x, sizes * y, sizes, sizes));
			f[k].setPosition(sizes * j, sizes * i);
			k++;
		}
	}

	for(unsigned int i = 0; i < position.length(); i += 5)
	{
		move(position.substr(i, 4));
	}
}



int main()
{
	util::Platform platform;

	RenderWindow window(VideoMode(454, 455), "Chess Simulator");

	wchar_t filename[] = L"stockfish_13_win_x64_bmi2.exe"; //notice the L infront of the string, this makes it into a wchar_t value

	//Connecting to the stockfish
	ConnectToEngine(filename);

	Texture t1, t2;
	t1.loadFromFile("images/figures.png");
	t2.loadFromFile("images/board0.png");

	// Sprite s(t1);
	Sprite sBoard(t2);

	for(int i = 0; i < 32; i++) f[i].setTexture(t1);

	loadPosition();

	bool isMove = false;
	float dx = 0, dy = 0;
	Vector2f oldPos, newPos;
	string str;
	int n = 0;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			/////move back or undo move/////
			if(e.type == Event::KeyPressed)
			{
				if(e.key.code == Keyboard::Backspace)
				{
					position.erase(position.length() - 6, 5);
					loadPosition();
				}
			}

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
							oldPos =     f[i].getPosition();
						}
					}
				}
			}
			if (e.type == Event::MouseButtonReleased)
			{
				if (e.mouseButton.button == Mouse::Left)
				{
					isMove = false;
					Vector2f p = f[n].getPosition() + Vector2f(sizes/2, sizes/2);
					newPos = Vector2f(sizes * int(p.x/sizes), sizes * int(p.y/sizes));
					str = toChessNote(oldPos) + toChessNote(newPos);
					move(str);
					position += str + " ";
					cout << str << endl;
					f[n].setPosition(newPos);
				}
			}
		}


		////computer move////
		if(Keyboard::isKeyPressed(Keyboard::Space))
		{
			str = getNextMove(position);

			oldPos = toCoord(str[0], str[1]);
			newPos = toCoord(str[2], str[3]);

			for(int i = 0; i < 32; i++)
			{
				if(f[i].getPosition() == oldPos)
				{
					n = i;
				}
			}

			////animation////
			for(int k = 0; k < 50; k++)
			{
				Vector2f p = newPos - oldPos;
				f[n].move(p.x / 50, p.y / 50);
				window.draw(sBoard);
				for(int i = 0; i < 32; i++)
				{
					window.draw(f[i]);
					window.draw(f[n]);
				}
				window.display();
			}

			move(str);
			position += str + " ";
			f[n].setPosition(newPos);

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

	// sf::CircleShape shape(window.getsizes().x / 2);
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
