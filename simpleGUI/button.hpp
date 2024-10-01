/* 
very simple button
*/
namespace simple {
	class Button {
		public:
			std::string stringValue;
			
			SDL_Texture* mTexture;
			
			SDL_Rect box;
			SDL_Rect text;
			
			SDL_Rect clickAnimation; // for animation purposes -> for not only available for click
			
			SDL_Color DefaultColor;
			SDL_Color HoverColor;
			SDL_Point padding;
			
			bool showClickAnimation;
			bool showHoverAnimation;
			
			// bool globalShowClickAnimation;
			bool globalShowHoverAnimation;
			
			
			SDL_Point mouse;
			
			// ------------ methods -----------
			// constructor
			Button () {
				mTexture = NULL;
				
				DefaultColor.r = 0;
				DefaultColor.g = 0;
				DefaultColor.b = 0;
				DefaultColor.a = 255;
				
				HoverColor.r = 255;
				HoverColor.g = 80;
				HoverColor.b = 80;
				HoverColor.a = 255;
				
				padding.x = 5;
				padding.y = 5;
				
				showHoverAnimation = false;
				showClickAnimation = false;
				
				// globalShowClickAnimation = false;
				globalShowHoverAnimation = true;
				
				stringValue = "Enter";
			}
			
			// destructor -> only frees the texture
			~Button () {
				freeButton ();
			}
			
			// freeButton 
			void freeButton () {
				if (mTexture != NULL) {
					SDL_DestroyTexture (mTexture);
					mTexture = NULL;
				}
			}
			
			// create Button
			// method to create button (text) / texture
			void createButton (SDL_Renderer* lRenderer, TTF_Font* mFont) {
				freeButton ();
				
				SDL_Surface* tSurface = NULL;
				SDL_Texture* tTexture = NULL;
				
				if (showHoverAnimation) {
					tSurface = TTF_RenderText_Solid (mFont, stringValue.c_str(), HoverColor);
				}
				else {
					tSurface = TTF_RenderText_Solid (mFont, stringValue.c_str(), DefaultColor);
				}
				if (tSurface == nullptr) {
					std::cout << "TTF_RenderText_Solid () : " << TTF_GetError () << std::endl;
				}
				else {
					tTexture = SDL_CreateTextureFromSurface (lRenderer, tSurface);
					if (tTexture == nullptr) {
						std::cout << "SDL_CreateTextureFromSurface () : " << SDL_GetError () << std::endl;
					}
					else {
						text.w = tSurface -> w;
						text.h = tSurface -> h;
						
						SDL_FreeSurface (tSurface);
						
						mTexture = tTexture;
					}
				}
			}
			
			// method to render Button
			void renderButton (SDL_Renderer* lRenderer) {
				if (showClickAnimation) {
					SDL_SetRenderDrawColor (lRenderer, DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a);
					SDL_RenderDrawRect (lRenderer, &clickAnimation);
					SDL_RenderCopy (lRenderer, mTexture, NULL, &text);
				}
				// render text
				else {
					SDL_SetRenderDrawColor (lRenderer, DefaultColor.r, DefaultColor.g, DefaultColor.b, DefaultColor.a);
					PutInBox ();
					SDL_RenderDrawRect (lRenderer, &box);
					SDL_RenderCopy (lRenderer, mTexture, NULL, &text);
				}
			}
			
			// method to put in box
			void PutInBox () {
				text.x = box.x + padding.x;
				text.y = box.y + padding.y;
				
				updateBox ();
			}
			
			void updateBox () {
				box.w = text.w + padding.x;
				box.h = text.h + padding.y;
			}
			
			// method to set box coordinates
			void setBoxCoordinates (int x, int y) {
				if (x < 0) {
					box.x = 0;
				}
				else {
					box.x = x;
				}
				
				if (y < 0)
					box.y = 0;
				else
					box.y = y;
			}
			
			// method to set up DefaultColor
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
			
			// method to set padding
			void setPadding (int x, int y) {
				padding.x = x;
				padding.y = y;
			}
			
			// method to set the string
			void setStringValue (std::string localString) {
				if (localString == "") {
					stringValue = "Enter";
				}
				else {
					stringValue = localString;
				}
			}
			
			// method to set up the hover color
			void setHoverColor (int red, int green, int blue, int opacity) {
				if (red > MAXCOLOR)
					HoverColor.r = MAXCOLOR;
				else if (red < 0)
					HoverColor.r = 0;
				else
					HoverColor.r = red;
			
				if (green > MAXCOLOR)
					HoverColor.g = MAXCOLOR;
				else if (green < 0)
					HoverColor.g = 0;
				else
					HoverColor.g = green;
				
				if (blue > MAXCOLOR)
					HoverColor.b = MAXCOLOR;
				else if (blue < 0)
					HoverColor.b = 0;
				else
					HoverColor.b = blue;
				
				if (opacity > MAXCOLOR)
					HoverColor.a = MAXCOLOR;
				else if (opacity < 0)
					HoverColor.a = 0;
				else
					HoverColor.a = opacity;
			}
			
			// method to set up click animation 
			void setClickAnimation (int de_fault) {
				clickAnimation.x = box.x + 3;
				clickAnimation.y = box.y + 3;
				clickAnimation.w = box.w;
				clickAnimation.h = box.h;
			}
			
			// method to turn hover animation true or false
			void setAnimationStatus (bool click, bool hover) {
				showClickAnimation = click;
				globalShowHoverAnimation = hover;
			}
			
			// method to handle events
			void handleEvents (SDL_Event& e, SDL_Renderer* lRenderer, TTF_Font* mFont, bool* returnData) {
				if (e.type == SDL_MOUSEMOTION) {
					SDL_GetMouseState (&mouse.x, &mouse.y);
					
					
					// for hover effect, checking if mouse is above the button
					if (SDL_PointInRect (&mouse, &box)) {
						if (globalShowHoverAnimation) {
							showHoverAnimation = true;
							createButton (lRenderer, mFont);
						}
					}
					else {
						showHoverAnimation = false;
						createButton (lRenderer, mFont);
						// showClickAnimation = false;
						// renderButton (lRenderer, box); //resetting the button (for click)
					}
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					// if clicked
					if (SDL_PointInRect (&mouse, &box)) {
						showClickAnimation = true;
						*returnData = true;
						// std::cout << "Button down" << std::endl;
					}
					else {
						*returnData = false;
					}
				}
				else if (e.type == SDL_MOUSEBUTTONUP ) {
				// else {
					showClickAnimation = false;
					*returnData = false;
					// std::cout << "Button up" << std::endl;
				}
			}
			
			// void setupButton function
			void setupButton (int x, int y, SDL_Renderer* lRenderer, TTF_Font* mFont, std::string stringValue) {
				setStringValue (stringValue);
				setBoxCoordinates (x, y);
				createButton (lRenderer, mFont);
				PutInBox ();
				updateBox ();
				setClickAnimation (-1);
			}
	};
};

/* 
BUG: Please Fix double clicking
*/