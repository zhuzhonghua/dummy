#include "stdafx.h"
#include "player.h"
#include "timing.h"
#include "movie.h"
#include "movie_def_impl.h"

// 1280/720
int Player::width = 450;
int Player::height = 800;

float Player::timeScale = 1.0f;
float Player::elapsed = 0.0f;

Player* Player::instance = NULL;

float Player::density = 1;

GameState Player::state = GameState::PLAY;

Player::Player()
{
	instance = this;

	_requestedReset = true;

	_now = 0;
	_step = 0;
}

Player::~Player()
{

}

bool Player::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_window = SDL_CreateWindow("DummyPlayer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	_context = SDL_GL_CreateContext(_window);

	SDL_GL_SetSwapInterval(1);
	glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);

	TTF_Init();
	
	return true;
}

void Player::run()
{
	FPSLimiter fps;
	fps.setMaxFPS(60.0f);

	while (state == GameState::PLAY)
	{
		fps.begin();

		long rightNowTick = SDL_GetTicks();
		_step = (_now == 0 ? 0 : rightNowTick - _now);
		_now = rightNowTick;

		processInput();

		step();

		draw();

		SDL_GL_SwapWindow(_window);

		//_fps = fps.end();
    SDL_Delay(200);
	}
}

void Player::processInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			state = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_motionEvents.push_back(e);
			break;
		case SDL_KEYDOWN:
			
			break;
		case SDL_KEYUP:
			
			break;
		case SDL_MOUSEBUTTONDOWN:
			_motionEvents.push_back(e);
			break;
		case SDL_MOUSEBUTTONUP:
			_motionEvents.push_back(e);
			break;
		}
	}

	_motionEvents.clear();
}

void Player::vibrate(int milliseconds)
{
}


void Player::step()
{
	update();
}

void Player::update()
{
	Player::elapsed = Player::timeScale * _step * 0.001f;
}

void Player::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	glScissor(0, 0, width, height);
  
	glActiveTexture(GL_TEXTURE0);
}

void Player::finish()
{
	SDL_Quit();
	exit(0);
}

Movie* Player::createMovie(const std::string& file)
{
	File* f = new File(file.c_str());

	MovieDefImpl* mf = new MovieDefImpl();
	mf->read(f);

	return mf->createInstance();
}
