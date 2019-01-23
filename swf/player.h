#pragma  once

enum GameState
{
	PLAY,
	EXIT
};

class Movie;

class Player
{
public:	
	static Player* instance;
	// Actual size of the screen
	static int width;
	static int height;

	// Density: mdpi=1, hdpi=1.5, xhdpi=2...
	static float density;

	static float timeScale;
	static float elapsed;

	static GameState state;

	Player();
	~Player();

	bool init();
	void run();
	void processInput();
public:
	static void vibrate(int milliseconds);

	void finish();

	Movie* createMovie(const std::string& file);
protected:
	virtual void step();
	virtual void update();
	virtual void draw();

protected:
	long	_now;
	long	_step;

	bool _requestedReset;

	float	_fps;
protected:
	SDL_Window* _window;
	SDL_GLContext _context;

	std::vector<SDL_Event>	_motionEvents;
};
