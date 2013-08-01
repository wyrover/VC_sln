#include "Sample.h"
#include <iostream>

namespace roverlib {

	CSample::CSample() : impl (new CSampleImpl)
	{

	}

	CSample::~CSample()
	{

	}

	void CSample::print()
	{
		impl->print();
	}


	
	

	

}