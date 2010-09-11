/*
 * GenericException.h
 *
 *  Created on: Apr 30, 2010
 *      Author: Rafael de Castro Dantas Sales - rafaelcds@gmail.com
 */

#ifndef GENERICEXCEPTION_H_
#define GENERICEXCEPTION_H_

#include <exception>

class GenericException: public std::exception {
private:
	const char* msg;
public:
	GenericException(const char* msg);
	virtual const char* what() const throw ();
};

#endif /* GENERICEXCEPTION_H_ */
