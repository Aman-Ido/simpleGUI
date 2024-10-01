#ifndef MAXCOLOR
#define MAXCOLOR 255
#endif
/* 
This is the label just to print string to a certain location
*/
namespace simple {
	class Label {
		public:
			SDL_Rect text;
			SDL_Texture* mTexture;
			SDL_Color DefaultColor;
			std::string stringValue;
			
			// ---- methods --------
			// constructor
			Label () {
				mTexture = NULL;
				
				DefaultColor.r = 0;
				DefaultColor.g = 0;
				DefaultColor.b = 0;
				DefaultColor.a = 255;
				
				stringValue = "";
			}
			
			// destructor
			~Label () {
				freeLabel ();
			}
			
			// method to free texture
			void freeLabel () {
				if (mTexture != NULL) {
					SDL_DestroyTexture (mTexture);
					mTexture = NULL;
				}
			}
			
			// method to create label (text) / texture
			void createLabel (SDL_Renderer* lRenderer, TTF_Font* mFont) {
				freeLabel ();
				
				SDL_Surface* tSurface = NULL;
				SDL_Texture* tTexture = NULL;
				
				tSurface = TTF_RenderText_Solid (mFont, stringValue.c_str(), DefaultColor);
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
			
			// method to set the default color
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
			
			// method to set the stringValue
			void setStringValue (std::string localString) {
				if (localString == "") {
					stringValue = " ";
				}
				else {
					stringValue = localString;
				}
			}
			
			// method to set label coordinate
			void setLabelCoordinates (int x, int y) {
				if (x < 0)
					text.x = 0;
				else 
					text.x = x;
				
				if (y < 0)
					text.y = 0;
				else 
					text.y = y;
			}
			
			// method to set label
			void setupLabel (int x, int y, std::string message, SDL_Renderer* lRenderer, TTF_Font* mFont) {
				setLabelCoordinates (x, y);
				setStringValue (message);
				createLabel (lRenderer, mFont);
			}
			
			// method to renderLabel
			void renderLabel (SDL_Renderer* lRenderer) {
				SDL_RenderCopy (lRenderer, mTexture, NULL, &text);
			}
		private:
	};
};