/* 
Creating simple GUI using simple GUI
*/
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define screenWidth 640
#define screenHeight 480

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// including the libraries 
#include "\Desktop\simpleGUI\simpleGUI\Entry.hpp" // for input
#include "\Desktop\simpleGUI\simpleGUI\Label.hpp" // for label
#include "\Desktop\simpleGUI\simpleGUI\Button.hpp" // for button

// function main
int main (int argc, char** argv) {
	SDL_Init (SDL_INIT_VIDEO);
	window = SDL_CreateWindow ("GUI Testing - SimpleGUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
	
	TTF_Init (); // initializing ttf library for sdl (mandatory)
	const int fontSize = 24;
	TTF_Font* mFont = TTF_OpenFont ("raster.ttf", fontSize);
	if (mFont == nullptr) { // if failed to load the font
		std::cout << "TTF_OpenFont () : " << TTF_GetError () << std::endl;
		return 0;
	}
	
	SDL_Event e;
	bool quit = false;
	
	// creating objects
		// creating label object
		simple::Label l1;
		l1.setDefaultColor (0, 255, 199, 255); // color unknown
		l1.setupLabel (20, 30, "Enter your name : ", renderer, mFont);
		
		// creating entry object
		simple::Entry nameInput;
		// this function goes like (x, y, width, height, fontsize, renderer, font)
		nameInput.setupBox (l1.text.w + 13, 30, 240, -1, fontSize, renderer, mFont);
		
		// creatin button 
		simple::Button btn;
		btn.setHoverColor (0, 255, 0, 255); // green color
		btn.setupButton (nameInput.box.x + nameInput.box.w + 3, 30, renderer, mFont, "Please click me!");
	
	bool returnData = false;
	
	SDL_StartTextInput ();
	
	// setting up the main loop
	while (!quit) {
		while (SDL_PollEvent (&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// code here
			nameInput.handleEvents (e, renderer, mFont);
			btn.handleEvents (e, renderer, mFont, &returnData);
		}
		SDL_SetRenderDrawColor (renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear (renderer);
		
		// rendering the label
		l1.renderLabel (renderer);
		nameInput.renderEntry (renderer);
		btn.renderButton (renderer);
		// if the button is clicked 
		if (returnData) {
			std::cout << "You have clicked the button" << std::endl;
			std::cout << "The name is : " << nameInput.stringValue << std::endl;
			returnData = false;
		}
		
		SDL_RenderPresent (renderer); // updating the window
	}
	
	SDL_StopTextInput ();
	
	// freeing everything
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (window);
	TTF_CloseFont (mFont);
	TTF_Quit();
	SDL_Quit ();
	
	return 0;
}