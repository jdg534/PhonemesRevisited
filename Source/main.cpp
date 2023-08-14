#include "main.h"

#include "PhonemeIdentification.h"

#include <SDL3/SDL.h>

#include <memory>
#include <fstream>

#include <nlohmann/json.hpp>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* Window = SDL_CreateWindow("A SDL3 window", 100, 100, 800, 600, 0);
	SDL_ShowWindow(Window);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, nullptr, 0);
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	PhonemeIdentification* PhonemeIdentifier = new PhonemeIdentification;
	PhonemeIdentifier->Initialise("Phonemes/phonemes.json");

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
		drawState(Renderer);
		SDL_RenderPresent(Renderer);
	}
	SDL_HideWindow(Window);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	testWriteJson();
	delete PhonemeIdentifier;
	return 0;
}

void drawState(SDL_Renderer* renderer)
{
	int width = 0, height = 0;
	SDL_GetRendererOutputSize(renderer, &width, &height);
	SDL_Point LineVerticies[8];
	LineVerticies[0].x = width * 10 / 100;
	LineVerticies[1].x = width * 20 / 100;
	LineVerticies[2].x = width * 30 / 100;
	LineVerticies[3].x = width * 40 / 100;
	LineVerticies[4].x = width * 50 / 100;
	LineVerticies[5].x = width * 60 / 100;
	LineVerticies[6].x = width * 70 / 100;
	LineVerticies[7].x = width * 80 / 100;
	LineVerticies[0].y = height * 10 / 100;
	LineVerticies[1].y = height * 20 / 100;
	LineVerticies[2].y = height * 30 / 100;
	LineVerticies[3].y = height * 40 / 100;
	LineVerticies[4].y = height * 50 / 100;
	LineVerticies[5].y = height * 60 / 100;
	LineVerticies[6].y = height * 70 / 100;
	LineVerticies[7].y = height * 80 / 100;

	if (SDL_RenderDrawLines(renderer, LineVerticies, 8) != 0)
	{
		SDL_Log("Error message: %s \n", SDL_GetError());
	}
}

void testWriteJson()
{
	nlohmann::json toDump =
	{
		{"someJson", true},
		{ "pi", 3.141f }
	};
	std::ofstream output;
	output.open("test.json", std::ios::out);
	output << toDump.dump(1);
	output.close();
}