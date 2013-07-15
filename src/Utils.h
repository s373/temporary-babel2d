/*
 *  Utils.h
 *  TemporaryBabel2D
 *
 *  Created by andré sier on 20130428.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */

#pragma once


static float easy1f(const float target,  float *src, const float efactor){
	float delta = target - (*src); 
	if( ABS(delta) > 1e-7 ) {
		(*src) += (target - (*src)) * efactor;
	}	
}
