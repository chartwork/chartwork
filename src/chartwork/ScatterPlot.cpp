#include <chartwork/ScatterPlot.h>

#include <QtGui>

#include <chartwork/Design.h>
#include <chartwork/elements/Symbol.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ScatterPlot
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ScatterPlot::ScatterPlot(QWidget *parent)
:	XYPlot(parent)
{
	resize(400, 300);
	setMinimumSize(200, 150);

	m_title.setText("Scatter Plot");

	m_key.setSymbols(m_symbols);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ScatterPlot::generateRandomValues()
{
	resetValues();

	std::srand(0);

	for (EntryMultiSet &dataSeries : m_values)
		for (int i = 0; i < 20; i++)
		{
			const double x = m_xMin + (std::rand() / (double)RAND_MAX) * (m_xMax - m_xMin);
			const double y = m_yMin + (std::rand() / (double)RAND_MAX) * (m_yMax - m_yMin);

			dataSeries.insert(Entry(x, y));
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ScatterPlot::paint(QPainter &painter)
{
	XYPlot::paint(painter);

	painter.setPen(Qt::NoPen);

	painter.setRenderHint(QPainter::Antialiasing);

	for (size_t i = 0; i < m_values.size(); i++)
		for (const Entry &value : m_values[i])
		{
			const QPointF positionF = positionForValue(value);
			const QPoint position(std::round(positionF.x()), std::round(positionF.y()));

			if (!m_chartRect.contains(position))
				continue;

			const int &symbol = (*m_symbols)[i % m_symbols->size()];
			const QColor &color = (*m_colors)[i % m_colors->size()];

			elements::Symbol::paint(painter, QPoint(position.x(), position.y()), symbol, color, m_symbolSize);
		}

	painter.setRenderHint(QPainter::Antialiasing, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
