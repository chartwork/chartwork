#ifndef __EXCEPTIONS__UNKNOWN_ITEM_EXCEPTION_H
#define __EXCEPTIONS__UNKNOWN_ITEM_EXCEPTION_H

#include <QStringList>

#include <exception>
#include <sstream>

namespace chartwork
{
namespace exceptions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UnknownItemException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class UnknownItemException : public std::exception
{
	public:
		UnknownItemException(const std::string &className, const QString &objectName,
			const std::string &variableName, const QString &accessedItem, const QStringList &allowedItems);
		UnknownItemException(const UnknownItemException &other);

		const char *what();

	private:
		const std::string &m_className;
		const QString &m_objectName;
		const std::string &m_variableName;
		const QString &m_accessedItem;
		const QStringList &m_allowedItems;

		std::stringstream m_what;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
