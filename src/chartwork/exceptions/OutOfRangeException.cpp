#include <chartwork/exceptions/OutOfRangeException.h>

#include <iostream>

namespace chartwork
{
namespace exceptions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// OutOfRangeException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

OutOfRangeException::OutOfRangeException(const std::string &className, const QString &objectName,
	const std::string &variableName, int accessedValue, int allowedRangeMin, int allowedRangeMax)
:	std::exception(),
	m_className(className),
	m_objectName(objectName),
	m_variableName(variableName),
	m_accessedValue(accessedValue),
	m_allowedRangeMin(allowedRangeMin),
	m_allowedRangeMax(allowedRangeMax)
{
	m_what
		<< "[Error] Access to " << m_className << " '" << m_objectName.toStdString() << "'"
		<< " at " << m_variableName << " = " << m_accessedValue
		<< " is out of range." << std::endl
		<< "        Allowed values are " << m_variableName << " = " << m_allowedRangeMin << "..." << m_allowedRangeMax << " here." << std::endl;

	std::cerr << what() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

OutOfRangeException::OutOfRangeException(const OutOfRangeException &other)
:	std::exception(),
	m_className(other.m_className),
	m_objectName(other.m_objectName),
	m_variableName(other.m_variableName),
	m_accessedValue(other.m_accessedValue),
	m_allowedRangeMin(other.m_allowedRangeMin),
	m_allowedRangeMax(other.m_allowedRangeMax)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *OutOfRangeException::what()
{
	return m_what.str().c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
