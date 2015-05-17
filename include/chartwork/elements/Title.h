#ifndef __ELEMENTS__TITLE_H
#define __ELEMENTS__TITLE_H

#include <QRect>
#include <QFont>
#include <QFontMetrics>

class QPainter;

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Title
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Title
{
	public:
		Title();

		void setText(const QString &text);
		const QString &text() const;

		void moveTopLeft(const QPoint &p);
		void setWidth(int width);

		void paint(QPainter &painter);

		const QRect &rect() const;

	private:
		void updateSize();

		QString m_text;

		QFont m_font;
		QRect m_rect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
