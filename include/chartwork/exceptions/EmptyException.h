#ifndef __EXCEPTIONS__EMPTY_EXCEPTION_H
#define __EXCEPTIONS__EMPTY_EXCEPTION_H

#include <QString>

#include <exception>
#include <sstream>

namespace chartwork
{
namespace exceptions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EmptyException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class EmptyException : public std::exception
{
	public:
		EmptyException(const std::string &className, const QString &objectName, const std::string &variableName);
		EmptyException(const EmptyException &other);

		const char *what();

	private:
		const std::string &m_className;
		const QString &m_objectName;
		const std::string &m_variableName;

		std::stringstream m_what;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
