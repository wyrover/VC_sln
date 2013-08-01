#ifndef _ROVERLIB_EXCEPTION_H_
#define _ROVERLIB_EXCEPTION_H_

#include <exception>
#include <string>

namespace roverlib {

	class Exception : public std::exception
	{
	public:
		Exception(const std::string& _description, const std::string& _source, const char* _file, long _line ) :
		  mDescription(_description),
			  mSource(_source),
			  mFile(_file),
			  mLine(_line)
		  {
		  }

		  Exception(const Exception& _rhs) :
		  mDescription(_rhs.mDescription),
			  mSource(_rhs.mSource),
			  mFile(_rhs.mFile),
			  mLine(_rhs.mLine),
			  mFullDesc(_rhs.mFullDesc)
		  {
		  }

		// Needed for  compatibility with std::exception
		  ~Exception() throw()
		  {
		  }

		Exception& operator = (const Exception& _rhs)		
		{
			mDescription = _rhs.mDescription;
			mSource = _rhs.mSource;
			mFile = _rhs.mFile;
			mLine = _rhs.mLine;
			mFullDesc = _rhs.mFullDesc;
			return *this;
		}

		virtual const std::string& getFullDescription() const		
		{
			if (mFullDesc.empty())
			{
				if ( mLine > 0 )
				{
					mFullDesc = utility::toString("MyGUI EXCEPTION : ", mDescription, " in ", mSource, " at ", mFile, " (line ", mLine, ")");
				}
				else
				{
					mFullDesc = utility::toString("MyGUI EXCEPTION : ", mDescription, " in ", mSource);
				}
			}

			return mFullDesc;
		}

		virtual const std::string& getSource() const		
		{
			return mSource;
		}

		virtual const std::string& getFile() const		
		{
			return mFile;
		}

		virtual long getLine() const		
		{
			return mLine;
		}

		virtual const std::string& getDescription() const		
		{
			return mDescription;
		}


		// Override std::exception::what
		const char* what() const throw()		
		{
			return getFullDescription().c_str();
		}

	protected:
		std::string mDescription;
		std::string mSource;
		std::string mFile;
		long mLine;
		mutable std::string mFullDesc;
	};
}

#endif