#include <chartwork/LinePlot.h>

#include <QtGui>

#include <chartwork/Design.h>
#include <chartwork/elements/Symbol.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LinePlot
//
////////////////////////////////////////////////////////////////////////////////////////////////////

LinePlot::LinePlot(QWidget *parent)
:	XYPlot(parent),
	m_lineWidth(2.0)
{
	resize(400, 300);
	setMinimumSize(200, 150);

	m_title.setText("Line Plot");

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void LinePlot::generateRandomValues()
{
	resetValues();

	std::srand(0);

	const size_t numberOfSamples = 50;

	auto randomFunction = [](int function, double value)
	{
		switch (function)
		{
			default:
			case 0:
				return value * value;
			case 1:
				return std::sqrt(value);
			case 2:
				return value;
			case 3:
				return std::cos(value * M_PI_2);
		}
	};

	for (EntryMultiSet &dataSeries : m_values)
	{
		const int function = rand() % 4;
		const double yMax = m_yMin + (std::rand() / (double)RAND_MAX) * (m_yMax - m_yMin);

		for (size_t i = 0; i < numberOfSamples; i++)
		{
			const double x = m_xMin + (m_xMax - m_xMin) * (double)i / (numberOfSamples - 1);
			const double y = m_yMin + randomFunction(function, (double)i / (numberOfSamples - 1)) * (yMax - m_yMin);

			dataSeries.insert(Entry(x, y));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void LinePlot::paint(QPainter &painter)
{
	XYPlot::paint(painter);

	painter.setRenderHint(QPainter::Antialiasing);

	for (size_t dataSeriesIndex = 0; dataSeriesIndex < m_values.size(); dataSeriesIndex++)
	{
		const EntryMultiSet &dataSeries = m_values[dataSeriesIndex];

		if (dataSeries.size() < 2)
			continue;

		const QColor &color = (*m_colors)[dataSeriesIndex % m_colors->size()];
		painter.setPen(QPen(color, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

		QPolygonF path;

		for (const Entry &entry : dataSeries)
		{
			const QPointF position = positionForValue(entry);
			path << position;
		}

		painter.drawPolyline(path);
	}

	painter.setRenderHint(QPainter::Antialiasing, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double LinePlot::lineWidth() const
{
	return m_lineWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void LinePlot::setLineWidth(double lineWidth)
{
	m_lineWidth = std::max(0.5, std::min(10.0, lineWidth));
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
