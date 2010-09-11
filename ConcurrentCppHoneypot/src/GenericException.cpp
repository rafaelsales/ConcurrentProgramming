/*
 * GenericException.cpp
 *
 *  Created on: Apr 30, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#include "GenericException.h"

GenericException::GenericException(const char* msg) :
	msg(msg) {
}

const char* GenericException::what() const throw () {
	return msg;
}
