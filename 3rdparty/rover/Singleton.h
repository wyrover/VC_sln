#ifndef _ROVERLIB_SINGLETON_H_
#define _ROVERLIB_SINGLETON_H_

#include "Diagnostic.h"

namespace roverlib {


	/************************************************************************/
	/* 模板单件类                                                                     */
	/************************************************************************/
	template <class T>
	class Singleton
	{
	public:
		typedef Singleton<T> Base;

		Singleton()
		{
			ROVER_ASSERT(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			ROVER_ASSERT(nullptr != msInstance, "Destroying Singleton instance " << getClassTypeName() << " before constructing it.");
			msInstance = nullptr;
		}

		static T& getInstance()
		{
			ROVER_ASSERT(nullptr != getInstancePtr(), "Singleton instance " << getClassTypeName() << " was not created");
			return (*getInstancePtr());
		}

		static T* getInstancePtr()
		{
			return msInstance;
		}

		static const char* getClassTypeName()
		{
			return mClassTypeName;
		}

	private:
		static T* msInstance;
		static const char* mClassTypeName;
	};
}


#endif