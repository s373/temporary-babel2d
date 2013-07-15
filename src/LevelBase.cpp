/*
 *  LevelBase.cpp
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */
#include "LevelBase.h"
#include "TemporaryBabel2D.h"


void LevelBase::setup(TemporaryBabel2D * app){

	tb2d = app;
	ofRectangle bounds(-100,-100, ofGetWidth()+200, ofGetHeight()+55);
	
	box2dShadowWorld.init();
	box2dFrontWorld.init();
	box2dShadowWorld.setFPS(30.0);
	box2dFrontWorld.registerGrabbing();
	box2dShadowWorld.setFPS(30.0);
//	box2dFrontWorld.registerGrabbing();
	
	//	box2d.createGround();
	box2dFrontWorld.createBounds(bounds);
	box2dShadowWorld.createBounds(bounds);
	
	// register the listener so that we get the events
	ofAddListener(box2dFrontWorld.contactStartEvents, this, &LevelBase::contactStart);
	ofAddListener(box2dFrontWorld.contactEndEvents, this, &LevelBase::contactEnd);
	
	
	
	
	leveltime = 0.0f;
	leveltimelimit = 7.0 + tb2d->leveleasy * 60 ;
	leveltimebegin = ofGetElapsedTimef();
	
	itemstocollect = 0;
	itemscollected = 0;
	
	exitopen = false;
	leveldone = false;
	donedistance = 0.0f;
	
	readersprob = ofMap(tb2d->leveleasy, 1, 0,  0.710e-3 ,  0.00125); 
	booksprob = ofMap(tb2d->leveleasy, 1, 0,  0.720e-3 ,  0.00125);
	trisprob = ofMap(tb2d->leveleasy, 1, 0,  0.725e-3 ,  0.00125);
	
	readersshadowprob = ofMap(tb2d->leveleasy, 1, 0,  1.0e-3 ,  0.121);
	wallbookshelvesshadowprob = ofMap(tb2d->leveleasy, 1, 0,  2.0e-3 ,  0.125);
	trisshadowprob = ofMap(tb2d->leveleasy, 1, 0,  2.0e-3 ,  0.125);
	
	//
	float x = ofRandom(-tb2d->drawdioXRange,tb2d->drawdioXRange) + ofGetWidth()/2;
	librarian.setup(box2dFrontWorld.getWorld(), x, ofRandom(10,50), ofRandom(55,69)*tb2d->leveleasy);
	//librarian.setup(<#b2World *b2dworld#>, <#float x#>, <#float y#>, <#float radius#>)

}

void LevelBase::addReaders() {

	ofxBox2dCircle  * c = new ofxBox2dCircle();
	c->setPhysics(ofRandom(0.05,0.16), ofRandom(0.55,0.91906), ofRandom(0.3,0.977));
//	c->setPhysics(<#float density#>, <#float bounce#>, <#float friction#>)
	float val = tb2d->audioenergyI * 10.0f;
//	c->setup(<#b2World *b2dworld#>, <#float x#>, <#float y#>, <#float radius#>)
	c->setup(box2dFrontWorld.getWorld(), tb2d->headPos + ofRandom(-val, val), tb2d->headPosY + 100, ofRandom(7, 25));
	
	c->setData(new ShadowLightData());
	ShadowLightData * sd = (ShadowLightData*)c->getData();
	sd->textureID = ofRandom(0, 4);
	sd->bHit	= false;
	
	readers.push_back(c);	
	
	
}


void LevelBase::addWallBookShelves(){
	
	ofVec2f locShelf ( tb2d->headPos, tb2d->headPosY + 25 + ofRandom(100)   );
	ofVec2f dimShelf ( ofRandom(55,105), 0   );
	dimShelf.y = dimShelf.x * ofRandom(0.7, 3);
	
//	locShelf.x -= dimShelf.x * 0.5;
//	locShelf.y -= dimShelf.y * 0.5;
	
	//addShelf(locShelf, dimShelf);
	// s373.monoflob codex
	int gx = (int) ofRandom(2,10);
	int gy = (int) ofRandom(2,10);
	int num = gx * gy;
	cout << "shelf with " << num << " books " << endl;
	float stridex = dimShelf.x / gx;
	float stridey = dimShelf.y / gy;	
	float gap = ofRandom(0.55, 0.9);
	float dimx = stridex*gap;
	float dimy = stridey*gap;
	
	
	for(int i=0; i<num; i++) {
		float x =  ((float)(i % gx) +0.5) * stridex + locShelf.x ;
		float y =  ((float)(i / gx) +0.5) * stridey + locShelf.y;
 				
		ofxBox2dRect * book = new ofxBox2dRect();
		
		book->setPhysics(ofRandom(0.05,0.08), ofRandom(0.5, 0.9), ofRandom(0.7,0.99));
//		book->setPhysics(<#float density#>, <#float bounce#>, <#float friction#>)
		book->setup(box2dFrontWorld.getWorld(),  x, y, dimx, dimy);
		wallbookshelves.push_back(book);
		
						
		
	}
	
	
	
	
}





void LevelBase::addTriangles() {
	
	
	int num = (int) ofRandom(2,10);
	float size = 0.37;
	
	for(int i=0; i<num; i++) {
		ofxBox2dPolygon	* tri = new ofxBox2dPolygon();
		
		// setup skate
		float triradius		= ofRandom(20,50)*size;
		float tritheta		= HALF_PI;//ofDegToRad(69);
		float triaperture	= (float)TWO_PI / (0.77f*(float)3);
		float xGap			= (float)TWO_PI / (float) 3;
		float xtemp, ytemp, xorigin, yorigin;
		for(int j=0; j<3; j++){		
			xtemp = cosf( tritheta + (float)j * triaperture) * triradius;
			ytemp = sinf( tritheta + (float)j * triaperture) * triradius;			  		
			if(i==0){
				xorigin = xtemp;
				yorigin = ytemp;
			}		
			tri->addVertex( xtemp, ytemp );
		}
		tri->addVertex( xorigin, yorigin );
		tri->setAsEdge(false);
		tri->close();

		tri->setPhysics(ofRandom(0.2,0.8), ofRandom(0.7, 0.9), ofRandom(0.55, 0.999));
		tri->create(box2dFrontWorld.getWorld());///, pos.x, pos.y + 65, 8, 12);
		ofVec2f tripos (tb2d->headPos + ofRandom(-50,50), tb2d->headPosY+ofRandom(27,77));
		tri->setPosition(tripos);

		trianglepool.push_back(tri);
	}	
	
		
	
}






void LevelBase::addReadersShadow() {
	
	ofxBox2dCircle  * c = new ofxBox2dCircle();
	c->setPhysics(1, 0.5, 0.9);
	float val = tb2d->audioenergyI * 10.0f;
	//	c->setup(<#b2World *b2dworld#>, <#float x#>, <#float y#>, <#float radius#>)
	c->setup(box2dShadowWorld.getWorld(), tb2d->headPos + ofRandom(-val, val), tb2d->headPosY + 10, ofRandom(20, 50));
	
	c->setData(new ShadowLightData());
	ShadowLightData * sd = (ShadowLightData*)c->getData();
	sd->textureID = ofRandom(0, 4);
	sd->bHit	= false;
	
	readersshadow.push_back(c);	
	
	
}


void LevelBase::addWallBookShelvesShadow(){
	
	ofVec2f locShelf ( tb2d->headPos, tb2d->headPosY + 10   );
	ofVec2f dimShelf ( ofRandom(55,105), 0   );
	dimShelf.y = dimShelf.x * ofRandom(0.7, 3);
	
	locShelf.x -= dimShelf.x * 0.5;
	locShelf.y -= dimShelf.y * 0.5;
	
	//addShelf(locShelf, dimShelf);
	// s373.monoflob codex
	int gx = (int) ofRandom(2,10);
	int gy = (int) ofRandom(2,10);
	int num = gx * gy;
	cout << "shelf with " << num << " books " << endl;
	float stridex = dimShelf.x / gx;
	float stridey = dimShelf.y / gy;	
	float gap = ofRandom(0.4, 0.9);
	
	
	for(int i=0; i<num; i++) {
		float x =  ((float)(i % gx) +0.5) * stridex + locShelf.x ;
		float y =  ((float)(i / gx) +0.5) * stridey + locShelf.y;
		
		ofxBox2dRect * book = new ofxBox2dRect();
		
		book->setPhysics(ofRandom(0.5,0.8), ofRandom(0.1, 0.5), ofRandom(0.7,0.99));
		book->setup(box2dShadowWorld.getWorld(),  tb2d->headPos , tb2d->headPosY + 10, stridex, stridey);
		wallbookshelvesshadow.push_back(book);
		
		
		
	}
	
	
	
	
}






void LevelBase::update(){
	
	box2dFrontWorld.update();
	box2dShadowWorld.update();
	
	
	if(!leveldone){//player logic
		ofVec2f playerpos = tb2d->player->headanchor.getPosition();
		ofVec2f libpos = librarian.getPosition();
		
		float playerhead = 7+tb2d->leveleasy*100;
		float libradius = librarian.getRadius();
		float distance = playerpos.distance(libpos);
		donedistance = distance;
		if( distance <= (playerhead+libradius) ){		
			leveldone = true;
			cout << "leveldone = true; distance " << distance << endl;			
		}
		
	}
	
	
	bool castreaders = false;
	bool castbooks=false;
	bool casttris=false;

	if(ofGetFrameNum()%((int)ofMap(tb2d->leveleasy, 1, 0, 180, 2) ) == 0 ) {		
		castreaders = tb2d->audioenergyI > 0.25;
		castbooks=tb2d->audioenergyI > 0.35;
		casttris=tb2d->audioenergyI > 0.15;

	}
	
	

//	if(castreaders||castbooks)
//		cout << "LevelBase::update called " << readersprob << " " << booksprob<< endl;
	
	if(castreaders||ofRandomuf() < readersprob ){
//		cout << "addReaders called" << endl;
		addReaders();		
	}
	if(castbooks||ofRandomuf() < booksprob ){	
//		cout << "addWallBookShelves called" << endl;
		addWallBookShelves();		
	}
	
	if(casttris||ofRandomuf() < trisprob ){	
//		cout << "addTriangles called" << endl;
		addTriangles();		
	}
	
	
	if(ofGetFrameNum()%300==0){
		float xtresh = 25;		
		
		for(int i=0; i<readers.size(); i++){				
			ofVec2f cpos = readers[i]->getPosition();			
			if( cpos.x < -xtresh || cpos.x > (xtresh + ofGetWidth())){
				readers[i]->destroy();
				readers.erase( readers.begin() + i );
				i--;
			}			
		}
		
		for(int i=0; i<wallbookshelves.size(); i++){				
			ofVec2f tpos = wallbookshelves[i]->getPosition();			
			if( tpos.x < -xtresh || tpos.x > (xtresh + ofGetWidth()) ){
				wallbookshelves[i]->destroy();
				wallbookshelves.erase( wallbookshelves.begin() + i );
				i--;
			}			
		}

		
		for(int i=0; i<trianglepool.size(); i++){				
			ofVec2f tpos = trianglepool[i]->getPosition();			
			if( tpos.x < -xtresh || tpos.x > (xtresh + ofGetWidth()) ){
				trianglepool[i]->destroy();
				trianglepool.erase( trianglepool.begin() + i );
				i--;
			}			
		}
		
		
		
	}
		
		

}
void LevelBase::draw(){
	
	
	ofSetColor(tb2d->lettersColor);
	ofFill();
	for(int i=0; i<readers.size();i++){
		readers[i]->draw();
	}
	for(int i=0; i<wallbookshelves.size();i++){
		wallbookshelves[i]->draw();
	}
	ofNoFill();
	for(int i=0; i<readersshadow.size();i++){
		readersshadow[i]->draw();
	}
	for(int i=0; i<wallbookshelvesshadow.size();i++){
		readersshadow[i]->draw();
	}
	for(int i=0; i<trianglepool.size();i++){
		trianglepool[i]->draw();
	}	
	
	
	ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));	
	librarian.draw();
	
	ofFill();
	if(leveldone){
		ofSetColor(tb2d->lettersColor);
	} else {
		ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255),
				   ofRandom(ofMap(donedistance, 1000, 20, 7,217, true)) );	
	}
	librarian.draw();
	
	
	
	
	

}

void LevelBase::buildlevel(){

}
void LevelBase::levelup(){
	leveltime = 0.0f;
	leveltimelimit = 7.0 + tb2d->leveleasy * 60 ;
	leveltimebegin = ofGetElapsedTimef();
	
	itemstocollect = 0;
	itemscollected = 0;
	
	exitopen = false;
	leveldone = false;
	donedistance = 0.0f;
	
	readersprob = ofMap(tb2d->leveleasy, 1, 0,  0.710e-3 ,  0.025); 
	booksprob = ofMap(tb2d->leveleasy, 1, 0,  0.720e-3 ,  0.025);
	trisprob = ofMap(tb2d->leveleasy, 1, 0,  0.725e-3 ,  0.025);
	
	readersshadowprob = ofMap(tb2d->leveleasy, 1, 0,  1.0e-3 ,  0.121);
	wallbookshelvesshadowprob = ofMap(tb2d->leveleasy, 1, 0,  2.0e-3 ,  0.125);
	trisshadowprob = ofMap(tb2d->leveleasy, 1, 0,  2.0e-3 ,  0.125);
	
	//
	float x = ofRandom(-tb2d->drawdioXRange,tb2d->drawdioXRange) + ofGetWidth()/2;
	librarian.setup(box2dFrontWorld.getWorld(), x, ofRandom(10,50), ofRandom(55,69)*tb2d->leveleasy);
	//librarian.setup(<#b2World *b2dworld#>, <#float x#>, <#float y#>, <#float radius#>)
	
	
	while(readers.size()>0){
		readers[0]->destroy();
		readers.erase(readers.begin());		
	}
	while(wallbookshelves.size()>0){
		wallbookshelves[0]->destroy();
		wallbookshelves.erase(wallbookshelves.begin());		
	}
	while(trianglepool.size()>0){
		trianglepool[0]->destroy();
		trianglepool.erase(trianglepool.begin());		
	}
	

}
void LevelBase::enterlevel(){

}
void LevelBase::exitlevel() {

}
void LevelBase::setFps(float fps){
	
	box2dFrontWorld.setFPS(fps);
	box2dShadowWorld.setFPS(fps);
	
}


//--------------------------------------------------------------
void LevelBase::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		
		// if we collide with the ground we do not
		// want to play a sound. this is how you do that
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			
			ShadowLightData * aData = (ShadowLightData*)e.a->GetBody()->GetUserData();
			ShadowLightData * bData = (ShadowLightData*)e.b->GetBody()->GetUserData();
			
			if(aData) {
				aData->bHit = true;
				//sound[aData->soundID].play();
				tb2d->playReaders();
			}
			
			if(bData) {
				bData->bHit = true;
				tb2d->playReaders();
			}
		}
	}
}

//--------------------------------------------------------------
void LevelBase::contactEnd(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		
		ShadowLightData * aData = (ShadowLightData*)e.a->GetBody()->GetUserData();
		ShadowLightData * bData = (ShadowLightData*)e.b->GetBody()->GetUserData();
		
		if(aData) {
			aData->bHit = false;
		}
		
		if(bData) {
			bData->bHit = false;
		}
	}
}



