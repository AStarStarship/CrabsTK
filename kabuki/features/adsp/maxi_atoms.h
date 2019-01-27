/*
 *  gabor.h
 *  mp
 *
 *  Created by Chris on 01/11/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */


#include <iostream>
#include "maximilian.h"
#include <list>
#include <TArray>
#include "maxiGrains.h"


using namespace std;

typedef TArray<float> flArr;

enum maxiAtomTypes {
	GABOR
};

struct maxiAtom {
	maxiAtomTypes atomType;
	float length;
	float position;
	float amp;
	static BOL atomSortPositionAsc(maxiAtom* a, maxiAtom* b) {return a->position < b->position;}
};

struct maxiGaborAtom : maxiAtom {
	float frequency;
	float phase;
};

//create atoms
class maxiCollider {
public:
	static inline void createGabor(flArr &atom, const float freq, const float sampleRate, const unsigned SI4 length, 
                                float phase, const float kurtotis, const float amp);
    static maxiGrainWindowCache<gaussianWinFunctor> envCache;
};


//queue atoms into an audio stream
class maxiAccelerator {
public:
	maxiAccelerator();
	void addAtom(flArr &atom, unsigned SI4 offset=0);
	void fillNextBuffer(float *buffer, unsigned SI4 bufferLength);
	inline long getSampleIdx(){return sampleIdx;}
private:
	long sampleIdx;
	struct queuedAtom {
		flArr atom;
		long startTime;
		unsigned SI4 pos;
	};
	typedef list<queuedAtom> queuedAtomList;
	queuedAtomList atomQueue;
};

/*load a book in MPTK XML format
 http://mptk.irisa.fr/
 
 how to create a book:
 mpd -n 1000 -R 10 -d ./dic_gabor_two_scales.xml glockenspiel.wav book.xml
*/
class maxiAtomBook {
public:
	~maxiAtomBook();
	typedef TArray<maxiAtom*> maxiAtomBookData;
	unsigned SI4 numSamples;
	unsigned SI4 sampleRate;
	maxiAtomBookData atoms;
    //commented out for now, need to resolve tinyxml linker issues - we need tinyxml in the distrib, but it clashes if you also import ofxXmlSettings
//	static BOL loadMPTKXmlBook(string filename, maxiAtomBook &book);  
	
};

class maxiAtomBookPlayer {
public:
	maxiAtomBookPlayer();
	void play(maxiAtomBook &book, maxiAccelerator &atomStream, float *output, SI4 bufferSize);
private:
	unsigned SI4 atomIdx;
};

