/*
	Name: Daniel Repko
	Course: Object Oriented Programming
	Prof: Majid Moghadam
	Title Of Program: Game Programming
	Date: 01/13/20
*/

//including the SDL.h header file
#include "SDL.h"
#include <stdlib.h>
#include <time.h>

//creating constants for the dimensions of the window
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

//creating a pointer for the SDL renderer
SDL_Renderer *renderer;

//creating a bool to represent whether the program is running
bool running = true;

//creating Rects for the paddles and ball
SDL_Rect PlayerPaddle;
SDL_Rect AIPaddle;
SDL_Rect Ball;
SDL_Rect Divider;

//creating an event variable to be used to read input events
SDL_Event event;

//Mouse coordinates
int mouse_x, mouse_y;

//Ball Speeds
int speed_x, speed_y;
int direction[2] = { -1, 1 };


bool CheckCollision(SDL_Rect rectA, SDL_Rect rectB)
{
	//the sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//calculate the sides of rect A
	leftA = rectA.x;
	rightA = rectA.x + rectA.w;
	topA = rectA.y;
	bottomA = rectA.y + rectA.h;

	//calculate the sides of rect B
	leftB = rectB.x;
	rightB = rectB.x + rectB.w;
	topB = rectB.y;
	bottomB= rectB.y + rectB.h;

	//if any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//if none of the sides from A are outside B
	return true;
}

/*
	Initialize the window, renderer, and game objects (paddles, ball)
*/
void LoadGame()
{
	//creating a pointer for the window
	SDL_Window *window;

	//initializing the SDL window, giving values for the title, position, and dimensions
	window = SDL_CreateWindow("Basic SDL Sprites",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);
	//returning out of LoadGame if window is null
	if (!window)
	{
		return;
	}

	//initializing the SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	//returning out of LoadGame if renderer is null
	if (!renderer)
	{
		return;
	}

	//setting the dimensions and position of the player paddle
	PlayerPaddle.x = 20;
	PlayerPaddle.y = 250;
	PlayerPaddle.h = 100;
	PlayerPaddle.w = 20;

	//setting the dimensions and position of the ai paddle
	AIPaddle.x = 760;
	AIPaddle.y = 250;
	AIPaddle.h = 100;
	AIPaddle.w = 20;

	//setting the dimensions and position of the ball
	Ball.x = 370;
	Ball.y = 290;
	Ball.h = 20;
	Ball.w = 20;

	//setting the speed of the ball
	speed_x = -1;
	speed_y = -1;

	//setting the dimensions and position of the divider
	Divider.x = 400;
	Divider.y = 0;
	Divider.w = 8;
	Divider.h = 600;
}

/*
	Reads the player inputs
*/
void Input()
{
	//Queuing events
	while (SDL_PollEvent(&event))
	{
		//Track mouse movement
		if (event.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
		}

		//Clicking 'x'
		if (event.type == SDL_QUIT)
		{
			running = false;
		}

		//pressing a key
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			//Pressing ESC exits from the game
			case SDLK_ESCAPE:
				running = false;
				break;
			}
		}
	}
}

/*
	runs every frame and performs various operations
*/
void Update()
{
	//making the player paddle move with the mouse
	PlayerPaddle.y = mouse_y;

	//adding movement to the ball
	Ball.x += speed_x;
	Ball.y += speed_y;

	//resetting the ball if it hits the side walls
	if (Ball.x < 0 || Ball.x > WINDOW_WIDTH)
	{
		Ball.x = WINDOW_WIDTH / 2;
		Ball.y = WINDOW_HEIGHT / 2;
		//this expression produces random -1, -2, 1 and 2
		srand(time(NULL));
		speed_x = (rand() % 2 + 1) * direction[rand() % 2];
		speed_y = (rand() % 2 + 1) * direction[rand() % 2];
	}

	//bouncing the ball off of the top and bottom walls
	if (Ball.y < 0 || Ball.y > (WINDOW_HEIGHT - Ball.h))
	{
		speed_y = -speed_y;
	}

	//moving the AIPaddle with the Ball
	AIPaddle.y = Ball.y - (AIPaddle.h / 2) + (Ball.h / 2);

	if (CheckCollision(Ball, AIPaddle) || CheckCollision(Ball, PlayerPaddle))
	{
		speed_x = -speed_x;
	}

	SDL_Delay(10);
}

/*
	Drawing the game objects to the renderer
*/
void DrawScreen()
{
	//clearing the renderer using the specified drawing color
	SDL_RenderClear(renderer);

	//creating a Rect for the background and placing it in the renderer
	SDL_Rect background = { 0, 0, 800, 600 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &background);

	//Drawing and setting the color of the paddles
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &PlayerPaddle);
	SDL_RenderFillRect(renderer, &AIPaddle);

	//Drawing and setting the color of the ball
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &Ball);

	//Drawing and setting the color of the divider
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &Divider);

	//Updating the renderer with the above drawing operations
	SDL_RenderPresent(renderer);
}

/*
	Quits the game
*/
void Quit()
{
	//Clearing all initializations ()
	SDL_Quit();
}

/*
	this method actually runs the game
*/
int main(int argc, char *argv[])
{
	//loading the game
	LoadGame();

	//calling the Input, Update, and DrawScreen functions every frame while the game is running
	while (running == true)
	{
		Input();
		Update();
		DrawScreen();
	}

	//quitting once the game stops running
	Quit();

	return 0;
}