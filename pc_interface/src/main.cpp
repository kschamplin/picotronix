// 
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>



int main(int argc, char** argv) {
	std::cout << "hello world!" << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	// create the window.
	SDL_Window* window = SDL_CreateWindow("picotronix gui", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {
		std::cout << "Error creating window:" << SDL_GetError() << std::endl;
		return 1;
	}

	auto surface = SDL_GetWindowSurface(window);
	SDL_FillRect(surface, NULL, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ));

	SDL_UpdateWindowSurface(window);
	SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}
