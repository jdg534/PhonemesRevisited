#include "main.h"

#include <SDL3/SDL.h>

#include <memory>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* Window = SDL_CreateWindow("A SDL3 window", 100, 100, 800, 600, 0);
	SDL_ShowWindow(Window);
	
	bool CloseRequested = false;
	while (!CloseRequested)
	{
		SDL_Event Event;
		std::memset(&Event, 0, sizeof(SDL_Event));
		while (SDL_PollEvent(&Event) > 0)
		{
			if (Event.type == SDL_QUIT)
			{
				CloseRequested = true;
			}
		}
	}
	SDL_HideWindow(Window);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}
