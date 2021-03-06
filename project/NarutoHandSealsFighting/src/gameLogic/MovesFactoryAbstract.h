/*
 ******************************************************
 * NAME:
 * MovesFactoryAstract.h
 ******************************************************
 * DESCRIPTION:
 *
 ******************************************************
 *	Created on: Nov 20, 2010
 ******************************************************
 *  Author: Michele Tamburini
 *******************************************************
 *
 */

#ifndef MOVESFACTORYASTRACT_H_
#define MOVESFACTORYASTRACT_H_

#include "MovesSet.h"
#include "SealsMap.h"

class MovesFactoryAbstract{
public:
	virtual void buildMovesSet(MovesSet* mSet,  SealsMap* sm) = 0;
};


#endif /* MOVESFACTORYASTRACT_H_ */
