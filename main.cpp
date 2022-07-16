#include<SFML/Graphics.hpp>

#include<time.h>
#include<iostream>
#include<fstream>
#include<string>

using namespace sf;


class Objects {

public: Sprite exit, butplay;

};



class Logick {

public: bool menu = true, game = false, records = false, input = false;

};



//objects 
bool isCollide(Sprite s1, Sprite s2)
{
	return s1.getGlobalBounds().intersects(s2.getGlobalBounds());
}




int main()
{
	bool is_moving = false;

	int level = 1;

	srand(time(0));

	// load font for records
	Font font;
	font.loadFromFile("helvetica_cyr_oblique.ttf");


	// load saved records from file
	int record_size = 3;

	std::fstream f("records.txt");

	std::string* rec_text = new std::string[record_size];

	for (int i = 0; i < record_size; i++) {
		std::string name, score;
		f >> name >> score;
		rec_text[i] = name + " " + score ;
	}

	f.close();

	String playerInput;

	Text playerText;

	bool keyReleased = true; // for once letter input


	RenderWindow app(VideoMode(1200, 800), "Arkanoid");


	app.setFramerateLimit(60);

	Objects o;
	Logick l;


	//MAKE TEXTURE
	Texture t1, t2, t3, t4,t5,t6,t7;
	t1.loadFromFile("textures/block2.png");
	t2.loadFromFile("textures/Pixel.png");
	t3.loadFromFile("textures/ball.png");
	t4.loadFromFile("textures/player.png");
	t5.loadFromFile("textures/exit.png");
	t6.loadFromFile("textures/butplay.png");
	t7.loadFromFile("textures/records.png");


	Sprite sBackground(t2), ball(t3), line(t4), exit(t5), butplay(t6),records(t7);

	butplay.setPosition(50,400);
	records.setPosition(550,400);

	exit.setPosition(400,600);

	ball.setPosition(300, 440);
	line.setPosition(300, 700);




	//MAKE BLOCK
	Sprite block[1000];
	for (int i = 0; i < 1000; i++) block[i].setTexture(t1);




	int counter = 0;



	//made blocks
	int n = 0;
	for (int i = 1; i < 27; i++)
	{
		for (int j = 1; j < level+1; j++)
		{
			block[n].setPosition(i * 43, j * 20);
			n++;
		}
	}




	//maine cycle
	float dx = level + 5;
	float dy = level + 4;




	//mda
	while (app.isOpen())
	{

		app.draw(sBackground);

		Event e;
		while (app.pollEvent(e))
		{
			

			if (e.type == Event::Closed)
			{
				app.close();
			}

			if (e.type == Event::MouseButtonPressed)

				if (e.key.code == Mouse::Left) {
				
				Vector2i pos = Mouse::getPosition(app);

					

				if (l.menu) {

						

					if (pos.x >= 400 && pos.x <= 748 &&
						pos.y >= 600 && pos.y <= 741)
						app.close();


					if (pos.x >= 550 && pos.x <= 1164 &&
						pos.y >= 400 && pos.y <= 543)
						l.menu = false;
						l.records = true;
						

					if (pos.x >= 50 && pos.x <= 433 &&
						pos.y >= 400 && pos.y <= 533) {

						l.menu = false;
						l.game = true;
					}
				}
				
				
				}



		
}






		if (l.game) {

			l.menu = false;
			l.game = true;
			l.records = false;
			is_moving = false;


			//ball move and kill block
			ball.move(dx, 0);
			for (int i = 0; i < n; i++)
			{
				if (isCollide(ball, block[i]))
				{
					block[i].setPosition(-100, 0);
					dx = -dx;
					counter++;
				}
			}


			//ball move and kill block
			ball.move(0, dy);
			for (int i = 0; i < n; i++)
			{
				if (isCollide(ball, block[i]))
				{
					block[i].setPosition(-100, 0);
					dy = -dy;
					counter++;
				}
			}


			//ball and line
			Vector2f b = ball.getPosition();
			if (b.x < 0 || b.x > 1200)
			{
				dx = -dx;
			}


			if (b.y < 0 || b.y > 800)
			{
				dy = -dy;
			}

			// game over condition
			if (b.y > 780) {
					l.game = false;
					l.input = true;
			}

			//move line
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				line.move(level+5, 0);
				is_moving = true;
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				line.move(-5-level, 0);
				is_moving = true;
			}

			//border for line
			Vector2f li = line.getPosition();
			if (li.x > 1110) line.setPosition(1110, 700);
			if (li.x < 0) line.setPosition(0, 700);


			 //basic otskok
			if (isCollide(line, ball) && !is_moving)
			{
				if(dx == 0) // if ball dx = 0 random ball to left or right
					if (rand() % 2 == 1 ) dx = -(level + 5); else dx = level + 5;
				dy = -dy;
			}
			 //perpendicular otskok
			else if(isCollide(line, ball) && is_moving)
			{
				dy = -dy;
				dx = 0;
			}

		}

		// if all blocks killed make new level
		if (counter == n) {
			ball.setPosition(300, 440);
			dx = level + 5;
			dy = level + 4;
			level++;

			n = 0;
			for (int i = 1; i < 27; i++)
			{
				for (int j = 1; j < level + 1; j++)
				{
					block[n].setPosition(i * 43, j * 20);
					n++;
				}
			}
			
		}



		//RECORDS INPUT

		if (l.input) {

			Text intext;

			intext.setFont(font);
			intext.setString("Input Your Name");
			intext.setCharacterSize(48);
			intext.setFillColor(Color::White);
			intext.setPosition(400, 0);
			app.draw(intext);

			playerText.setFont(font);
			playerText.setPosition(60, 300);
			playerText.setFillColor(Color::Red);
			playerText.setCharacterSize(24);

			if (e.type == Event::KeyReleased) keyReleased = true;

			

			if (e.type == Event::TextEntered && keyReleased)
			{
				keyReleased = false;
				playerInput += e.text.unicode;
				playerText.setString(playerInput);
			}

			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				std::fstream a("records.txt", std::ios::out | std::ios::app);
				a << std::string(playerInput) << counter << std::endl;
				a.close();
				app.close();
			}

			app.draw(playerText);
			

		}


		




		//MENU
		if (l.menu) {

			

			app.draw(butplay);
			app.draw(records);
			app.draw(exit);
			

		}





		//GAME
		if (l.game) {

			// draw blocks

			for (int i = 0; i < n; i++)
			{
				app.draw(block[i]);
			}

			

			app.draw(ball);

			app.draw(line);

		}

		//records
		if (l.records) {
			l.input = false;

			Text text;
			Text* rec = new Text[record_size];

			for (int i = 0; i < record_size; i++) {
				rec[i].setFont(font);
				rec[i].setString(rec_text[i]);
				rec[i].setCharacterSize(36);
				rec[i].setFillColor(Color::White);
				rec[i].setPosition(0, 90 + i * 70);
				app.draw(rec[i]);
			}

			text.setFont(font);
			text.setString("RecordZ");
			text.setCharacterSize(48);
			text.setFillColor(Color::White);
			text.setPosition(500, 0);
			app.draw(text);
		}

		app.display();


	}

	return 0;
}