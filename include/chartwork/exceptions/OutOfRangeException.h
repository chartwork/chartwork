#ifndef __EXCEPTIONS__OUT_OF_RANGE_EXCEPTION_H
#define __EXCEPTIONS__OUT_OF_RANGE_EXCEPTION_H

#include <QString>

#include <exception>
#include <sstream>

namespace chartwork
{
namespace exceptions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// OutOfRangeException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class OutOfRangeException : public std::exception
{
	public:
		OutOfRangeException(const std::string &className, const QString &objectName, const std::string &variableName, int accessedValue, int allowedRangeMin, int allowedRangeMax);
		OutOfRangeException(const OutOfRangeException &other);

		const char *what();

	private:
		const std::string &m_className;
		const QString &m_objectName;
		const std::string &m_variableName;
		int m_accessedValue;
		int m_allowedRangeMin;
		int m_allowedRangeMax;

		std::stringstream m_what;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
