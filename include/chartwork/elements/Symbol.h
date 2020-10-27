#ifndef __ELEMENTS__SYMBOL_H
#define __ELEMENTS__SYMBOL_H

#include <QRect>
#include <QFont>
#include <QFontMetrics>
#include <cmath>

class QPainter;

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Symbol
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Symbol
{
	public:
		enum Type
		{
			Circle,
			Square,
			Diamond,
			Plus,
			CircleOutline,
			SquareOutline,
			DiamondOutline,
			Cross
		};

		static void paint(QPainter &painter, QPoint center, int type, const QColor &color, int size);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
