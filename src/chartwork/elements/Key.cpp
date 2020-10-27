#include <chartwork/elements/Key.h>

#include <QPainter>

#include <chartwork/Design.h>
#include <chartwork/elements/Symbol.h>

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Key
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Key::Key(std::shared_ptr<const QStringList> items, std::shared_ptr<const QList<QColor>> colors, std::shared_ptr<const QList<int>> symbols)
:	m_items(items),
	m_colors(colors),
	m_symbols(symbols),
	m_fontMetrics(m_font),
	m_invertOrder(false),
	m_isVisible(true),
	m_scale(1.0)
{
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::setInvertOrder(bool invertOrder)
{
	m_invertOrder = invertOrder;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::show()
{
	setVisible(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::hide()
{
	setVisible(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::setVisible(bool isVisible)
{
	m_isVisible = isVisible;

	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Key::isVisible() const
{
	if (m_items->isEmpty() || m_colors->isEmpty())
		return false;

	return m_isVisible;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::moveTopRight(const QPoint &p)
{
	m_rect.moveTopLeft(QPoint(p.x() - m_rect.width(), p.y()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::paint(QPainter &painter)
{
	if (!isVisible())
		return;

	painter.setPen(Qt::NoPen);
	painter.setBrush(design::superlightgray);
	painter.drawRect(m_rect);
	painter.setFont(m_font);

	const int numberOfElements = m_items->size();

	for (int i = 0; i < numberOfElements; i++)
	{
		const int elementIndex = m_invertOrder ? numberOfElements - 1 - i : i;

		const QRect squareRect(m_rect.left() + m_scale * design::keySpacing, m_rect.top() + m_scale * (design::keySpacing + (design::keySpacing + design::keyItemSize) * i), m_scale * design::keyItemSize, m_scale * design::keyItemSize);

		const QColor &color = (*m_colors)[elementIndex % m_colors->size()];

		// Draw a square by default
		if (!m_symbols || m_symbols->isEmpty())
		{
			painter.setBrush(color);
			painter.drawRect(squareRect);
		}
		// If custom symbols are available, draw those instead
		else
		{
			painter.setRenderHint(QPainter::Antialiasing);
			const int &symbol = (*m_symbols)[elementIndex % m_symbols->size()];
			Symbol::paint(painter, QPoint(squareRect.x() + squareRect.width() / 2, squareRect.y() + squareRect.height() / 2), symbol, color, squareRect.width() / 2);
			painter.setRenderHint(QPainter::Antialiasing, false);
		}

		painter.setPen(Qt::SolidLine);
		painter.drawText(QRect(squareRect.right() + m_scale * design::keySpacing, squareRect.top(), m_rect.width(), m_scale * design::keyItemSize), (*m_items)[elementIndex], QTextOption(Qt::AlignVCenter));
		painter.setPen(Qt::NoPen);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Key::maxStringWidth() const
{
	int maxStringWidth = 0;

	for (const QString &string : (*m_items))
		maxStringWidth = std::max(maxStringWidth, m_fontMetrics.horizontalAdvance(string));

	return maxStringWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QRect &Key::rect() const
{
	return m_rect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::updateSize()
{
	if (!isVisible())
	{
		m_rect.setSize(QSize());
		return;
	}

	m_rect.setWidth(m_scale * (3 * design::keySpacing + design::keyItemSize) + maxStringWidth());
	m_rect.setHeight(m_scale * (m_items->size() * design::keyItemSize + (m_items->size() + 1) * design::keySpacing));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::setSymbols(std::shared_ptr<const QList<int> > symbols)
{
	m_symbols = symbols;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Key::setScale(double scale)
{
	m_scale = std::max(0.5, std::min(1.0, scale));

	if (QFont().pixelSize() != -1)
		m_font.setPixelSize(std::round(m_scale * QFont().pixelSize()));
	else if (QFont().pointSize() != -1)
		m_font.setPointSizeF(std::round(m_scale * QFont().pointSizeF()));

	m_fontMetrics = QFontMetrics(m_font);

	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Key::scale() const
{
	return m_scale;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
