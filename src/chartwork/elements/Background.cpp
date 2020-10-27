#include <chartwork/elements/Background.h>

#include <QPainter>

#include <chartwork/Design.h>

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Background
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Background::Background(QWidget *parent)
:	m_parent(parent)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Background::paint(QPainter &painter)
{
	painter.setPen(Qt::NoPen);
    if (get_background_color().isValid())
    {
        painter.setBrush(get_background_color());
        painter.drawRect(m_parent->rect());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Background::set_background_color(QColor new_color)
{
    background_color_ = new_color;
}

QColor Background::get_background_color() const
{
	return background_color_;
}

}
}
