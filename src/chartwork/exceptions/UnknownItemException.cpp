#include <chartwork/exceptions/UnknownItemException.h>

#include <iostream>

namespace chartwork
{
namespace exceptions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UnknownItemException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

UnknownItemException::UnknownItemException(const std::string &className, const QString &objectName,
	const std::string &variableName, const QString &accessedItem, const QStringList &allowedItems)
:	std::exception(),
	m_className(className),
	m_objectName(objectName),
	m_variableName(variableName),
	m_accessedItem(accessedItem),
	m_allowedItems(allowedItems)
{
	m_what
		<< "[Error] Access to " << m_className << " '" << m_objectName.toStdString() << "'"
		<< " at " << m_variableName << " = '" << m_accessedItem.toStdString() << "'"
		<< " is invalid." << std::endl
		<< "        Allowed values for " << m_variableName << " are here:" << std::endl;

	for (const QString &allowedItem : m_allowedItems)
		m_what << "        * " << allowedItem.toStdString() << std::endl;

	std::cerr << what() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

UnknownItemException::UnknownItemException(const UnknownItemException &other)
:	std::exception(),
	m_className(other.m_className),
	m_objectName(other.m_objectName),
	m_variableName(other.m_variableName),
	m_accessedItem(other.m_accessedItem),
	m_allowedItems(other.m_allowedItems)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *UnknownItemException::what()
{
	return m_what.str().c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
