#ifndef MAXCOLOR
#define MAXCOLOR 255
#endif
/* 
This file contains the class for Entry or Input Bar
*/
namespace simple {
	class Entry {
		public:
			SDL_Rect destination; // text
			SDL_Rect source; // clip
			SDL_Point padding;
			SDL_Point mouse;
			SDL_Rect box; // the box
			SDL_Texture* mTexture;
			std::string stringValue;
			SDL_Rect cursor;
			
			std::string TypeOfInput;
			
			bool active;
			bool showCursor;
			
			
			SDL_Color DefaultColor;
			
			// -------- methods -----------
			// constructor 
			Entry () {
				mTexture = NULL;
				stringValue = " ";
				passcodeString = "";
				
				padding.x = 3;
				padding.y = 3;
				
				TypeOfInput = "normal";
				
				DefaultColor.r = 0;
				DefaultColor.g = 0;
				DefaultColor.b = 0;
				DefaultColor.a = 255;
				
				cursor.w = 5;
				cursor.h = 2;
				
				active = false;
				showCursor = false;
			}
			
			// method to free the texture (only)
			void freeEntry () {
				if (mTexture != NULL) {
					SDL_DestroyTexture (mTexture);
					mTexture = NULL;
				}
			}
			
			// method to create texture from the text stored in stringValue
			void createEntry (SDL_Renderer* lRenderer, TTF_Font* mFont, SDL_Color textColor) {
				freeEntry ();
				
				SDL_Surface* tSurface = TTF_RenderText_Solid (mFont, stringValue.c_str(), textColor);
				if (tSurface == nullptr) {
					std::cout << "TTF_RenderText_Solid () : " << TTF_GetError () << std::endl;
				}
				else {
					SDL_Texture* tTexture = SDL_CreateTextureFromSurface (lRenderer, tSurface);
					if (tTexture == nullptr) {
						std::cout << "SDL_CreateTextureFromSurface () : " << SDL_GetError () << std::endl;
					}
					else {
						destination.w = tSurface -> w;
						destination.h = tSurface -> h;
						
						SDL_FreeSurface (tSurface);
						
						mTexture = tTexture;
					}
				}
			}
			
			// method to setup the box coordinate and size -> this function also sets up the text coordinate (with padding) and clipping (side scrolling)
			void setupBox (int x, int y, int width, int height, int fontSize, SDL_Renderer* lRenderer, TTF_Font* mFont) {
				box.x = x;
				box.y = y;
				box.w = width;
				
				if (height == -1)
					box.h = fontSize + 6;
				else
					box.h = height;
				
				createEntry (lRenderer, mFont, DefaultColor);
				putInBox ();
				toCopy ();
				updateClip ();
				updateCursor ();
			}
			
			// method to put in the box -> this function puts the text input the input box (padding included )
			void putInBox () {
				destination.x = box.x;
				destination.y = box.y + padding.y;
			}
			
			// method to render the box and the texture (text) inside of the input box
			void renderEntry (SDL_Renderer* lRenderer) {
				SDL_SetRenderDrawColor (lRenderer, DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a);
				SDL_RenderDrawRect (lRenderer, &box);
				SDL_RenderCopy (lRenderer, mTexture, &source, &destination);
				
				if (showCursor) {
					SDL_SetRenderDrawColor (lRenderer, DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a);
					SDL_RenderFillRect (lRenderer, &cursor);
				}
			}
			
			// method to set the string value for display
			void setStringValue (std::string localString) {
				stringValue = localString;
			}
			
			// method to handle Events
			void handleEvents (SDL_Event& e, SDL_Renderer* lRenderer, TTF_Font* mFont) {
				if (active) {
					if (e.type == SDL_TEXTINPUT) {
						if (TypeOfInput == "normal") {
							stringValue += e.text.text;
							createEntry (lRenderer, mFont, DefaultColor);
							toCopy ();
						}
						else if (TypeOfInput == "passcode") {
							stringValue += "*";
							passcodeString += e.text.text;
							createEntry (lRenderer, mFont, DefaultColor);
							toCopy ();
						}
					}
					if (e.type == SDL_KEYDOWN) {
						// checking if SDL_StartTextInput is initialized or not
						if (!SDL_IsTextInputActive()) {
							std::cout << "SDL_StartTextInput is not initialized, please initialize" << std::endl;
						}
						
						if (e.key.keysym.sym == SDLK_BACKSPACE && stringValue.length() > 1) {
							if (TypeOfInput == "normal") {
								stringValue.pop_back ();
								createEntry (lRenderer, mFont, DefaultColor);
								toCopy ();
							}
							else if (TypeOfInput == "passcode") {
								stringValue.pop_back ();
								passcodeString.pop_back ();
								createEntry (lRenderer, mFont, DefaultColor);
								toCopy ();
							}
						}
						if (e.key.keysym.sym == SDLK_c && SDL_GetModState () & KMOD_CTRL) {
							SDL_SetClipboardText (stringValue.c_str());
						}
						if (e.key.keysym.sym == SDLK_v && SDL_GetModState () & KMOD_CTRL ) {
							char* temporaryText = SDL_GetClipboardText ();
							stringValue += temporaryText;
							SDL_free (temporaryText);
							createEntry (lRenderer, mFont, DefaultColor);
							toCopy ();
						}
					}
				}
				if (e.type == SDL_MOUSEMOTION) {
					SDL_GetMouseState (&mouse.x, &mouse.y);
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					if (SDL_PointInRect (&mouse, &box)) {
						if (!active && !showCursor) {
							active = true;
							showCursor = true;
						}
					}
					else {
						if (active && showCursor) {
							active = false;
							showCursor = false;
						}
					}
				}
			}
			
			// this one is related to clipping if there is overflow of the text
			void toCopy () {
				if (destination.w > box.w) {
					updateClip ();
					destination.w = box.w;
				}
				else {
					source.y = 0;
					source.w = box.w;
					source.h = destination.h;
				}
				updateCursor ();
			}
			
			void updateClip () {
				if (destination.w > box.w) {
					source.x = destination.w - box.w;
					source.y = 0;
					source.w = destination.w;
					source.h = destination.h;
				}
				else {
					source.x = destination.w;
					toCopy ();
				}
			}
			// this one is related to clipping if there is overflow of the text
			
			// method to change the default color
			void setDefaultColor (int red, int green, int blue, int opacity) {
				if (red > MAXCOLOR)
					DefaultColor.r = MAXCOLOR;
				else if (red < 0)
					DefaultColor.r = 0;
				else
					DefaultColor.r = red;
			
				if (green > MAXCOLOR)
					DefaultColor.g = MAXCOLOR;
				else if (green < 0)
					DefaultColor.g = 0;
				else
					DefaultColor.g = green;
				
				if (blue > MAXCOLOR)
					DefaultColor.b = MAXCOLOR;
				else if (blue < 0)
					DefaultColor.b = 0;
				else
					DefaultColor.b = blue;
				
				if (opacity > MAXCOLOR)
					DefaultColor.a = MAXCOLOR;
				else if (opacity < 0)
					DefaultColor.a = 0;
				else
					DefaultColor.a = opacity;
			}
			
			// method to set if the input is going to be passcode or normal text
			void setInputType (std::string localType) {
				if (localType == "normal") { // default value
					TypeOfInput = "normal";
				}
				else if (localType == "passcode") {
					TypeOfInput = "passcode";
				}
				else {
					std::cout << "There are only 2 types of input they are, normal or passcode" << std::endl;
					TypeOfInput = "normal";
				}
				
			}
			
			
			// getter to get the value of passcode String
			std::string getPasscodeString () {
				return passcodeString;
			}
			
			// method to update cursor
			void updateCursor () {
				cursor.x = destination.x + destination.w + 3;
				cursor.y = destination.y + destination.h;
			}
		private:
			std::string passcodeString; // string if the passcode is set
	};
};

/* 
TODO: Add a feature to set if the inputted string is passport or text
TODO: Add the feature for active or inactive of input bar
TODO: Add a cursor (optional)

DONE: Added a feature to set if the inputted string is passport or text
DONE: Added the feature for active or inactive of input bar
DONE: Added a cursor (optional)
*/