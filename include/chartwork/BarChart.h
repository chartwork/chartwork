#ifndef __BAR_CHART_H
#define __BAR_CHART_H

#include <chartwork/Chart.h>
#include <chartwork/elements/Axis.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// BarChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT BarChart : public Chart
{
	Q_OBJECT

	public:
		BarChart(QWidget *parent = 0);

		// Get/set the displayed maximum value of the chart's y axis
		double yMax() const;
		void setYMax(double yMax);

		// Get/set the precision of the chart's y axis (number of digits after the comma)
		int yPrecision() const;
		void setYPrecision(int yPrecision);

	protected:
		virtual void generateRandomValues() = 0;

		virtual void paintEvent(QPaintEvent *event) = 0;

		elements::Axis m_yAxis;

		double m_yMax;
		int m_yPrecision;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
