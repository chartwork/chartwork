#ifndef __ELEMENTS__AXIS_H
#define __ELEMENTS__AXIS_H

#include <QRect>
#include <QFont>
#include <QFontMetrics>
#include <QStringList>

class QPainter;

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Axis
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Axis
{
	public:
		enum Type
		{
			X_AXIS,
			Y_AXIS
		};

		Axis(Type type);

		void setMin(double min);
		void setMax(double max);
		void setPrecision(int precision);

		bool normalized() const;
		void setNormalized(bool normalized);

		void setLength(int length);
		void moveTopLeft(QPoint p);
		void moveTop(int top);
		void moveLeft(int left);

		const QRect &rect() const;

		void setCrossing(double crossing);
		void removeCrossing();

		void paint(QPainter &painter);

	private:
		static double computeStep(double rangeWidth);

		void updateStepStrings();
		void updateSize();

		void paintX(QPainter &painter);
		void paintY(QPainter &painter);

		int maxStringWidth() const;

		QRect m_rect;

		QFont m_font;
		QFontMetrics m_fontMetrics;

		Type m_type;
		double m_min;
		double m_max;
		int m_precision;

		bool m_normalized;

		QStringList m_stepStrings;
		int m_numberOfSteps;
		double m_startValue;
		double m_stepSize;

		double m_crossing;
		bool m_hasCrossing;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
