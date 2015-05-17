#include <chartwork/exceptions/EmptyException.h>

#include <iostream>

namespace chartwork
{
namespace exceptions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EmptyException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

EmptyException::EmptyException(const std::string &className, const QString &objectName,
	const std::string &variableName)
:	std::exception(),
	m_className(className),
	m_objectName(objectName),
	m_variableName(variableName)
{
	m_what
		<< "[Error] Cannot access " << m_variableName << " of " << m_className << " '" << m_objectName.toStdString() << "'"
		<< ", because it is empty." << std::endl;

	std::cerr << what() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

EmptyException::EmptyException(const EmptyException &other)
:	std::exception(),
	m_className(other.m_className),
	m_objectName(other.m_objectName),
	m_variableName(other.m_variableName)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *EmptyException::what()
{
	return m_what.str().c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
