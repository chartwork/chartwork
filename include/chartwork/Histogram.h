#ifndef __HISTOGRAM_H
#define __HISTOGRAM_H

#include <chartwork/BarChart.h>
#include <chartwork/elements/Axis.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Histogram
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT Histogram : public BarChart
{
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(double xMin READ xMin WRITE setXMin)
	Q_PROPERTY(double xMax READ xMax WRITE setXMax)
	Q_PROPERTY(double xStep READ xStep WRITE setXStep)
	Q_PROPERTY(int xPrecision READ xPrecision WRITE setXPrecision)
	Q_PROPERTY(double yMax READ yMax WRITE setYMax)
	Q_PROPERTY(int yPrecision READ yPrecision WRITE setYPrecision)

	Q_PROPERTY(QColor color READ color0 WRITE setColor0)

	public:
		Histogram(QWidget *parent = 0);

		// Get/set the displayed minimum value of the chart's x axis
		double xMin() const;
		void setXMin(double xMin);

		// Get/set the displayed maximum value of the chart's x axis
		double xMax() const;
		void setXMax(double xMax);

		// Get/set the displayed step of the chart's x axis
		// The labels of two consecutive bars have a distance of xStep
		double xStep() const;
		void setXStep(double xStep);

		// Get/set the precision of the chart's x axis (number of digits after the comma)
		int xPrecision() const;
		void setXPrecision(int xPrecision);

		// Get/set the displayed maximum value of the chart's y axis
		double yMax() const;
		void setYMax(double yMax);

		// Get/set the values of individual bars
		void setValue(int x, double value);
		double value(int x) const;

		// Set the values of all bars simultaneously
		void setAllValues(double value);

	private:
		void resetValues();
		void generateRandomValues();

		void updateNumberOfEntries();
		void paintEvent(QPaintEvent *);
		void paint(QPainter &painter);

		double m_xMin;
		double m_xMax;
		double m_xStep;
		int m_xPrecision;

		size_t m_numberOfEntries;

		std::vector<double> m_values;

		QRect m_chartRect;

		QFont m_font;
		QFontMetrics m_fontMetrics;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
