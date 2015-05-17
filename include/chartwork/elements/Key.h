#ifndef __ELEMENTS__KEY_H
#define __ELEMENTS__KEY_H

#include <QRect>
#include <QFont>
#include <QFontMetrics>
#include <QStringList>
#include <memory>

class QPainter;

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Key
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Key
{
	public:
		Key(std::shared_ptr<const QStringList> items, std::shared_ptr<const QList<QColor>> colors, std::shared_ptr<const QList<int>> symbols = nullptr);

		void setSymbols(std::shared_ptr<const QList<int>> symbols);
		void updateSize();
		void setInvertOrder(bool invertOrder);

		void show();
		void hide();
		void setVisible(bool isVisible);
		bool isVisible() const;

		void moveTopRight(const QPoint &p);
		void paint(QPainter &painter);

		const QRect &rect() const;

		void setScale(double scale);
		double scale() const;

	private:
		int maxStringWidth() const;

		QRect m_rect;
		std::shared_ptr<const QStringList> m_items;
		std::shared_ptr<const QList<QColor>> m_colors;
		std::shared_ptr<const QList<int>> m_symbols;

		QFont m_font;
		QFontMetrics m_fontMetrics;
		bool m_invertOrder;

		bool m_isVisible;

		double m_scale;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
