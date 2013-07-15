/*

	Temporary-Babel2D	

	s373.net/x 201304
 
	a game by Andre Sier
 
 
	licensed under CC-BY-NC 3.0
	http://creativecommons.org/licenses/by-nc/3.0/
	
 
 

	downloaded alpha version for ludum dare #26 2013-04-27,28
 


 
	game notes:
 
 
			uses microphone, keys (wasd or arrows and space).
		 
			game logic goals is trying to reach the library within time for the next level
		 
			you have 60 minutes to complete 100 levels. time seems to slow down.
		 
			especially on hyperskates, on following versions.
  	
 
 
 
	source code notes:
 
 
			the code is very messy, proceed with caution, handle with care.
		 
			this is an alpha state version like code that will most likely be improved
		 
			to release a final built game with some more of the prototyped logic herein. 
		 
			this alpha version presented for #LD26, as a playble minimalist game.
		 
			
	
 
 
 
	media tools used:	
 
					gimp (textures), 
					audacity (sounds), 
					xcode (programming),
					openframeworks (programming),
					ofxbox2d (programming)
 

 
	game credits:
 
					coding, 
					media, 
					art, 
					game, 
					music		(andre sier)
	
					produced at s373.net/x
					contact us at s373.net/contact
 

	thank you for playing temporary-babel2d!
 
 */

#include "ofMain.h" 

#include "TemporaryBabel2D.h" 

#include "ofAppGlutWindow.h" 

int main( ){
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 473, 761, OF_WINDOW);
	ofRunApp( new TemporaryBabel2D() );
}
// <!-- temporary-babel2d eof -->