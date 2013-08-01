#ifndef _ROVERLIB_DIAGNOSTIC_H_
#define _ROVERLIB_DIAGNOSTIC_H_

#include <sstream>
#include <crtdbg.h>
#include "Exception.h"

#define ROVER_BASE_EXCEPT(desc, src)	 throw roverlib::Exception(desc, src, __FILE__, __LINE__);

#define ROVER_EXCEPT(dest) \
{ \
	_CrtDbgBreak(); \
	std::ostringstream stream; \
	stream << dest << "\n"; \
	ROVER_BASE_EXCEPT(stream.str().c_str(), "MyGUI"); \
}

#define ROVER_ASSERT(exp, dest) \
{ \
	if ( ! (exp) ) \
	{ \
	_CrtDbgBreak(); \
	std::ostringstream stream; \
	stream << dest << "\n"; \
	ROVER_BASE_EXCEPT(stream.str().c_str(), "MyGUI"); \
	} \
}


#endif