#include <chartwork/Histogram.h>

#include <QtGui>

#include <chartwork/Design.h>
#include <chartwork/elements/Key.h>
#include <chartwork/elements/Title.h>
#include <chartwork/elements/Background.h>
#include <chartwork/exceptions/OutOfRangeException.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Histogram
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Histogram::Histogram(QWidget *parent)
:	BarChart(parent),
	m_xMin(0.0),
	m_xMax(100.0),
	m_xStep(5.0),
	m_xPrecision(0),
	m_fontMetrics(m_font)
{
	resize(400, 300);
	setMinimumSize(200, 150);

	m_title.setText("Histogram");

	updateNumberOfEntries();
	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Histogram::xMin() const
{
	return m_xMin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setXMin(double xMin)
{
	m_xMin = xMin;

	updateNumberOfEntries();
	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Histogram::xMax() const
{
	return m_xMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setXMax(double xMax)
{
	m_xMax = xMax;

	updateNumberOfEntries();
	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Histogram::xStep() const
{
	return m_xStep;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setXStep(double xStep)
{
	m_xStep = xStep;

	updateNumberOfEntries();
	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Histogram::xPrecision() const
{
	return m_xPrecision;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setXPrecision(int xPrecision)
{
	m_xPrecision = std::max(0, xPrecision);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Histogram::yMax() const
{
	return m_yMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setYMax(double yMax)
{
	m_yMax = yMax;

	m_yAxis.setMax(yMax);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::resetValues()
{
	m_values.clear();
	m_values.resize(m_numberOfEntries, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::generateRandomValues()
{
	resetValues();

	std::srand(0);

	for (double &value : m_values)
		value = (std::rand() / (double)RAND_MAX) * m_yMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	m_title.setWidth(width());
	m_title.moveTopLeft(QPoint(0, design::spacing - m_title.rect().height() / 4));

	const int textHeight = 3.0 / 4.0 * m_fontMetrics.height();
	const int remainingHeight = std::max(0, height() - m_title.rect().height() - 2 * design::spacing - textHeight - design::axisSpacing);

	m_yAxis.setLength(remainingHeight);

	const int remainingWidth = std::max(0, width() - 3 * design::spacing - m_yAxis.rect().width());

	m_chartRect.setSize(QSize(remainingWidth, remainingHeight));
	m_chartRect.moveTopLeft(QPoint(2 * design::spacing + m_yAxis.rect().width(), design::spacing + m_title.rect().height()));

	m_yAxis.moveTopLeft(QPoint(design::spacing, m_chartRect.top()));

	m_background.paint(painter);
	paint(painter);
	m_yAxis.paint(painter);
	m_title.paint(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::paint(QPainter &painter)
{
	if (m_numberOfEntries <= 0)
		return;

	const size_t labelStep = std::max((size_t)1, m_numberOfEntries / 4);
	const double textHeight = m_fontMetrics.height();
	const double barWidth = std::max(0.0, (double)m_chartRect.width() / m_numberOfEntries);

	for (size_t x = 0; x < m_numberOfEntries; x++)
	{
		const int x0 = x * barWidth;
		const int x1 = (x + 1) * barWidth;

		const double height = std::max(0.0, std::min((double)m_chartRect.height(), m_values[x] / m_yMax * m_chartRect.height()));

		painter.setBrush((*m_colors)[0]);
		painter.setPen(Qt::NoPen);
		painter.drawRect(m_chartRect.left() + x0, m_chartRect.bottom(), x1 - x0, -height);

		if (x % labelStep != 0)
			continue;

		painter.setFont(m_font);
		painter.setPen(Qt::SolidLine);
		const double textY = m_chartRect.bottom() + design::axisSpacing;
		painter.drawText(QRect(m_chartRect.left() + x0 - barWidth * (labelStep - 1) / 2.0, textY, labelStep * barWidth, textHeight), QString::number(m_xMin + m_xStep * x, 'f', m_xPrecision), QTextOption(Qt::AlignCenter));
		painter.setPen(Qt::NoPen);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::updateNumberOfEntries()
{
	if (m_xStep == 0)
	{
		m_numberOfEntries = 0;
		return;
	}

	m_numberOfEntries = std::floor(std::max(0.0, (m_xMax - m_xMin) / m_xStep)) + 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setValue(int x, double value)
{
	handleValueUpdate();

	if (x < 0 || (size_t)x >= m_values.size())
		throw exceptions::OutOfRangeException("Histogram", objectName(), "x", x, 0, (int)m_values.size() - 1);

	m_values[x] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Histogram::setAllValues(double value)
{
	handleValueUpdate();

	m_values.clear();
	m_values.resize(m_numberOfEntries, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Histogram::value(int x) const
{
	if (x < 0 || (size_t)x >= m_values.size())
		throw exceptions::OutOfRangeException("Histogram", objectName(), "x", x, 0, (int)m_values.size() - 1);

	return m_values[x];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
