#include <chartwork/elements/Symbol.h>

#include <QPainter>

#include <chartwork/Design.h>

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Symbol
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void Symbol::paint(QPainter &painter, QPoint center, int symbolType, const QColor &color, int size)
{
	const QPoint w(size, 0);
	const QPoint h(0, size);

	const QColor &backgroundColor = design::superlightgray;

	switch (symbolType)
	{
		default:
		case Circle:
		{
			const double scale = 2.0 / std::sqrt(M_PI);
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawEllipse(center, (int)std::round(size * scale), (int)std::round(size * scale));
			break;
		}

		case Square:
		{
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawRect(center.x() - size, center.y() - size, 2 * size, 2 * size);
			break;
		}

		case Diamond:
		{
			const double sqrt2 = std::sqrt(2.0);
			const QPoint points[4] = {center + w * sqrt2, center - h * sqrt2, center - w * sqrt2, center + h * sqrt2};
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawConvexPolygon(points, 4);
			break;
		}

		case Plus:
		{
			const QPointF pixel = size % 2 == 1 ? QPointF(0.5, 0.5) : QPointF();

			const double sqrt2 = std::sqrt(2.0);
			painter.setBrush(Qt::NoBrush);
			painter.setPen(QPen(color, size));
			painter.drawLine((QPointF)center - w / sqrt2 + pixel, (QPointF)center + w / sqrt2 + pixel);
			painter.drawLine((QPointF)center - h / sqrt2 + pixel, (QPointF)center + h / sqrt2 + pixel);
			break;
		}

		case CircleOutline:
		{
			const double scale = 2.0 / std::sqrt(M_PI);
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawEllipse(center, (int)std::round(size * scale), (int)std::round(size * scale));
			painter.setBrush(backgroundColor);
			painter.setPen(Qt::NoPen);
			painter.drawEllipse(center, (int)std::round(size * scale / 2), (int)std::round(size * scale / 2));
			break;
		}

		case SquareOutline:
		{
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawRect(center.x() - size, center.y() - size, 2 * size, 2 * size);
			painter.setBrush(backgroundColor);
			painter.setPen(Qt::NoPen);
			painter.drawRect(center.x() - size / 2, center.y() - size / 2, (int)(size / 2) * 2, (int)(size / 2) * 2);
			break;
		}

		case DiamondOutline:
		{
			const double sqrt2 = std::sqrt(2.0);
			const QPoint points[4] = {center + w * sqrt2, center - h * sqrt2, center - w * sqrt2, center + h * sqrt2};
			const QPoint pointsInner[4] = {center + w * sqrt2 / 2, center - h * sqrt2 / 2, center - w * sqrt2 / 2, center + h * sqrt2 / 2};
			painter.setBrush(color);
			painter.setPen(Qt::NoPen);
			painter.drawConvexPolygon(points, 4);
			painter.setBrush(backgroundColor);
			painter.setPen(Qt::NoPen);
			painter.drawConvexPolygon(pointsInner, 4);
			break;
		}

		case Cross:
		{
			painter.setBrush(Qt::NoBrush);
			painter.setPen(QPen(color, size));
			painter.drawLine(center - w / 2 - h / 2, center + w / 2 + h / 2);
			painter.drawLine(center + w / 2 - h / 2, center - w / 2 + h / 2);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
