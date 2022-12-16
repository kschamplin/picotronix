

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>


int main(int argc, char** argv) {
	std::cout << "hello world!" << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
}
