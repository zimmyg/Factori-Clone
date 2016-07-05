//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>


bool init(); //Starts up SDL and creates window
bool loadMedia(); //Loads media
SDL_Texture* loadTexture(std::string path); // Loads individual texture as image
void close(); //Frees media and shuts down SDL


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Current displayed texture
SDL_Texture* gTexture = NULL;

int main(int argc, char* args[])
{
	// Start up SDL and create window
	if ( !init() )
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if ( !loadMedia() )
		{
			printf("Failed to load media!\n");
		}
		else
		{
			// Main loop flag
			bool quit = false;

			// Event handler
			SDL_Event e;

			// While application is running
			while (!quit)
			{
				// Event processing
				while (SDL_PollEvent(&e) != 0)
				{
					// User request quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				// Clear screen
				SDL_RenderClear( gRenderer );

				// Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

				// Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	// Free resources and close SDL
	close();

	return 0;
}

bool init()
{
	bool success = true;

	// Initailise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if ( gRenderer == NULL )
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Initialize renderer colour to solid white
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture( "res/testImage.png" );
	if (gTexture == NULL)
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

SDL_Texture* loadTexture(std::string path)
{
	// Final texture
	SDL_Texture* newTexture = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if ( loadedSurface == NULL )
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if ( newTexture == NULL )
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		// Get rid of old surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}