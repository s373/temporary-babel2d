/*
 *  PlayerBase.cpp
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "PlayerBase.h"
#include "TemporaryBabel2D.h"
#include "LevelBase.h"


void PlayerBase::setup(TemporaryBabel2D *app){
	tb2d = app;
	buildplayer();
}


void PlayerBase::buildplayer(){

	if(tb2d==NULL || tb2d->level->box2dFrontWorld.getWorld()==NULL){
		cout << "in player base tb2d is NULL"<<endl;
		return;
	}
	
	float size = 0.37;
	
	pos.set(10, ofGetHeight()-250);
	force.set(0,0);
	
	float density = 0.7;
	float bounce = 0.3;
	float friction = 0.7;
	float springlenHead = 0.250 * size;
	float springlenArm = 1.0 *size;
	float springlenLeg = 0.25 *size;
	float springlenSkate = 5.0 *size;
	
	// setup skate
	float triradius		= 34*size;
	float tritheta		= HALF_PI;//ofDegToRad(69);
	float triaperture	= (float)TWO_PI / (0.77f*(float)3);
	float xGap			= (float)TWO_PI / (float) 3;
	float xtemp, ytemp, xorigin, yorigin;
	for(int i=0; i<3; i++){		
		xtemp = cosf( tritheta + (float)i * triaperture) * triradius;
		ytemp = sinf( tritheta + (float)i * triaperture) * triradius;			  		
		if(i==0){
			xorigin = xtemp;
			yorigin = ytemp;
		}		
		skateanchor.addVertex( xtemp, ytemp );
		skateanchorshadow.addVertex( xtemp, ytemp );
	}
	skateanchor.addVertex( xorigin, yorigin );
	skateanchorshadow.addVertex( xorigin, yorigin );
	skateanchor.setAsEdge(false);
	skateanchorshadow.setAsEdge(false);
	skateanchor.close();
	skateanchorshadow.close();
	
	
	// set physis
	headanchor.setPhysics(density,bounce,friction);
	skateanchor.setPhysics(density,bounce,friction);
	for(int i=0; i<5; i++){
		bodyanchor[i].setPhysics(density,bounce,friction);	
	}
	headanchorshadow.setPhysics(density,bounce,friction);
	skateanchorshadow.setPhysics(density,bounce,friction);
	for(int i=0; i<5; i++){
		bodyanchorshadow[i].setPhysics(density,bounce,friction);	
	}
	
	// set pos
	
	headanchor.setup(tb2d->level->box2dFrontWorld.getWorld(), pos.x, pos.y, 16*size);
	headanchorshadow.setup(tb2d->level->box2dShadowWorld.getWorld(), pos.x, pos.y, 16*size);
	// 0 body, 1 left, 2 right, 3 leftleg, 4 right leg
	bodyanchor[0].setup(tb2d->level->box2dFrontWorld.getWorld(), pos.x, pos.y + 37*size, 8*size, 15*size);
	bodyanchorshadow[0].setup(tb2d->level->box2dShadowWorld.getWorld(), pos.x, pos.y + 37*size, 8*size, 15*size);
	bodyanchor[1].setup(tb2d->level->box2dFrontWorld.getWorld(), pos.x - 25*size, pos.y + 37*size, 8*size, 15*size);
	bodyanchorshadow[1].setup(tb2d->level->box2dShadowWorld.getWorld(), pos.x - 25*size, pos.y + 37*size, 4*size, 15*size);
	bodyanchor[2].setup(tb2d->level->box2dFrontWorld.getWorld(), pos.x + 25*size, pos.y + 37*size, 4*size, 15*size);
	bodyanchorshadow[2].setup(tb2d->level->box2dShadowWorld.getWorld(), pos.x + 25, pos.y + 37*size, 4*size, 15*size);
	bodyanchor[3].setup(tb2d->level->box2dFrontWorld.getWorld(), pos.x-25*size, pos.y + 52*size, 4*size, 12*size);
	bodyanchorshadow[3].setup(tb2d->level->box2dShadowWorld.getWorld(), pos.x-25*size, pos.y + 52, 4*size, 12*size);
	bodyanchor[4].setup(tb2d->level->box2dFrontWorld.getWorld(), pos.x+25*size, pos.y + 52*size, 4*size, 12*size);
	bodyanchorshadow[4].setup(tb2d->level->box2dShadowWorld.getWorld(), pos.x+25*size, pos.y + 52*size, 4*size, 12*size);

	
	skateanchor.create(tb2d->level->box2dFrontWorld.getWorld());///, pos.x, pos.y + 65, 8, 12);
	skateanchorshadow.create(tb2d->level->box2dShadowWorld.getWorld());//, pos.x, pos.y + 65, 8, 12);
	ofVec2f skatepos (pos.x, pos.y+47);
	skateanchor.setPosition(skatepos);
	skateanchorshadow.setPosition(skatepos);
	
	bodywojoints.clear();
	ofxBox2dJoint headbodyjoint,bodyarmljoint,bodyarmrjoint, bodylegljoint, bodylegrjoint, leglskatejoint,legrskatejoint;
	ofxBox2dJoint headbodyjointshadow,bodyarmljointshadow,bodyarmrjointshadow, bodylegljointshadow, bodylegrjointshadow, leglskatejointshadow,legrskatejointshadow;
	
	
	headbodyjoint.setup(tb2d->level->box2dFrontWorld.getWorld(), headanchor.body, bodyanchor[0].body);
	bodyarmljoint.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchor[0].body, bodyanchor[1].body);
	bodyarmrjoint.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchor[0].body, bodyanchor[2].body);
	bodylegljoint.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchor[0].body, bodyanchor[3].body);
	bodylegrjoint.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchor[0].body, bodyanchor[4].body);
//	leglskatejoint.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchor[3].body, skateanchor.getCenter());
//	legrskatejoint.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchor[4].body, skateanchor.getCenter());
	bodywojoints.push_back(headbodyjoint);
	bodywojoints.push_back(bodyarmljoint);
	bodywojoints.push_back(bodyarmrjoint);
	bodywojoints.push_back(bodylegljoint);
	bodywojoints.push_back(bodylegrjoint);
//	bodywojoints.push_back(leglskatejoint);
//	bodywojoints.push_back(legrskatejoint);

	headbodyjointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), headanchorshadow.body, bodyanchorshadow[0].body);
	bodyarmljointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchorshadow[0].body, bodyanchorshadow[1].body);
	bodyarmrjointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchorshadow[0].body, bodyanchorshadow[2].body);
	bodylegljointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchorshadow[0].body, bodyanchorshadow[3].body);
	bodylegrjointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchorshadow[0].body, bodyanchorshadow[4].body);
//	leglskatejointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchorshadow[3].body, skateanchorshadow.body);
//	legrskatejointshadow.setup(tb2d->level->box2dFrontWorld.getWorld(), bodyanchorshadow[4].body, skateanchorshadow.body);
	bodywojointsshadow.push_back(headbodyjointshadow);
	bodywojointsshadow.push_back(bodyarmljointshadow);
	bodywojointsshadow.push_back(bodyarmrjointshadow);
	bodywojointsshadow.push_back(bodylegljointshadow);
	bodywojointsshadow.push_back(bodylegrjointshadow);
//	bodywojointsshadow.push_back(leglskatejointshadow);
//	bodywojointsshadow.push_back(legrskatejointshadow);
	
	
}




void PlayerBase::update(){
	//box 2d updates then get pos and apply forces
	float forceconstant = 25.0f / 4.0f;
	float forcemag = tb2d->leveleasy * forceconstant;
	
	applyforce = ofGetElapsedTimef() > forcetimethen;
	bool iskeypressed = false;
	
	// always slow force & apply
	float av = tb2d->audioenergyI * 12.5;
	easy1f(ofRandom(-av,av), &force.x, 0.055);
	easy1f(ofRandom(-av,av), &force.y, 0.055);
	playerforcemagnitude = force.length();
	
	headanchor.addForce(force, 1.0f);
	headanchorshadow.addAttractionPoint(headanchor.getPosition(), 1);
	skateanchor.addAttractionPoint(ofRandomuf()<0.5? bodyanchor[3].getPosition() : bodyanchor[4].getPosition()  , 1.0f);
	skateanchorshadow.addAttractionPoint(ofRandomuf()<0.5? bodyanchor[3].getPosition() : bodyanchor[4].getPosition()  , 1.0f);
//	skateanchorshadow.addForce(force, 1.0f);
	
//	headanchorshadow.addForce(force, 1.0f);
//	skateanchor.addForce(force, 1.0f);
//	skateanchorshadow.addForce(force, 1.0f);
	
	if( getLives() ){ //&& applyforce ){
			
		applyforce = false;
		forcetimethen = ofGetElapsedTimef() + 0.1;
		
		if(keyup){
			iskeypressed = true;
//			keyup=false;
			
			force.y -= forcemag;
		}
		if(keyspace){
			iskeypressed = true;
//			keyspace=false;
			
			force.y -= 5*forcemag;
		}
		
		if(keydown){
			iskeypressed = true;
//			keydown=false;
			
			force.y += forcemag;
		}
		
		if(keyleft){
			iskeypressed = true;
//			keyleft=false;
			
			force.x -= forcemag;
		}

		if(keyright){
			iskeypressed = true;
//			keyright=false;
			
			force.x += forcemag;
		}
		
		
		if(iskeypressed){
			
			float maxy = tb2d->level->box2dFrontWorld.getGravity().y * 3.07;
			
			if( ABS(force.y) > maxy ){
				force.y = -1. * maxy;
			}
			   
			force.limit(2.5);
		}
		
	}
	
	
	
}




void PlayerBase::draw(){
	
	ofSetColor(tb2d->lettersColor);
	ofFill();
	for(int i=0; i<bodywojoints.size();i++){
		bodywojoints[i].draw();
		bodywojointsshadow[i].draw();
	}
	headanchor.draw();
	headanchorshadow.draw();	
	for(int i=0; i<5; i++){
		bodyanchor[i].draw();
		bodyanchorshadow[i].draw();
	}
	ofNoFill();
	ofSetColor(0, 255, 0, 221);
	skateanchorshadow.draw();
	skateanchor.draw();
	ofFill();
	
}





void PlayerBase::keyPressed(int key){
	if(tb2d!=NULL){
//		if(tb2d->gamestate == TemporaryBabel2D::LEVELPLAY){
			
			switch (key) {
				case 'w': case OF_KEY_UP:		keyup = true;break;
				case 'a': case OF_KEY_LEFT:		keyleft = true;break;
				case 's': case OF_KEY_DOWN:		keydown = true;break;
				case 'd': case OF_KEY_RIGHT:	keyright = true;break;			
				case ' ': keyspace = true;break;										
	
				default:
					break;
			}
			
			
		} else {
			
			if(keyup)		keyup = false;
			if(keyleft)		keyleft = false;
			if(keyright)	keyright = false;
			if(keydown)		keydown = false;
			if(keyspace)	keyspace = false;
			
			
		}
		
		
//	}
	
		
}

void PlayerBase::keyReleased(int key){
	
	if(tb2d!=NULL){
//		if(tb2d->gamestate == tb2d->LEVELPLAY){
			
			switch (key) {
				case 'w': case OF_KEY_UP:		keyup = false;break;
				case 'a': case OF_KEY_LEFT:		keyleft = false;break;
				case 's': case OF_KEY_DOWN:		keydown = false;break;
				case 'd': case OF_KEY_RIGHT:	keyright = false;break;				
				case ' ': keyspace = false;break;										
				default:
					break;
			}
			
			
		} else {
			
			if(keyup)		keyup = false;
			if(keyleft)		keyleft = false;
			if(keyright)	keyright = false;
			if(keydown)		keydown = false;
			if(keyspace)	keyspace = false;
			
			
		}
		
		
//	}
	
	
}