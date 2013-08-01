#ifndef _ROVERLIB_SAMPLE_H_
#define _ROVERLIB_SAMPLE_H_

#include <boost/shared_ptr.hpp>

namespace roverlib
{
	class CSample
	{
	public:
		CSample();
		~CSample();

		void print();
	private:
		class CSampleImpl;
		boost::shared_ptr<CSampleImpl> impl;
	
	};

	class CSample::CSampleImpl
	{
	public:
		void print()
		{
			std::cout << "test ге╫сдёй╫" << std::endl;
		}
	};

	
}

#endif
