#include <chartwork/elements/Axis.h>

#include <QPainter>

#include <cmath>

#include <chartwork/Design.h>

namespace chartwork
{
namespace elements
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Axis
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Axis::Axis(Type type)
:	m_fontMetrics(m_font),
	m_type(type),
	m_min(0),
	m_max(100),
	m_precision(0),
	m_normalized(false),
	m_hasCrossing(false)
{
	updateStepStrings();
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::setMin(double min)
{
	m_min = min;

	updateStepStrings();
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::setMax(double max)
{
	m_max = max;

	updateStepStrings();
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::setPrecision(int precision)
{
	m_precision = std::max(0, precision);

	updateStepStrings();
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Axis::normalized() const
{
	return m_normalized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::setNormalized(bool normalized)
{
	m_normalized = normalized;

	updateStepStrings();
	updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::setLength(int length)
{
	switch (m_type)
	{
		case X_AXIS:
			m_rect.setWidth(length);
			break;

		case Y_AXIS:
			m_rect.setHeight(length);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::moveTopLeft(QPoint p)
{
	m_rect.moveTopLeft(p);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::moveTop(int top)
{
	m_rect.moveTop(top);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::moveLeft(int left)
{
	m_rect.moveLeft(left);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::paint(QPainter &painter)
{
	switch (m_type)
	{
		case X_AXIS:
			paintX(painter);
			break;

		case Y_AXIS:
			paintY(painter);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::paintX(QPainter &painter)
{
	const int textHeight = m_fontMetrics.height();

	painter.setPen(design::gray);
	painter.drawLine(m_rect.left(), m_rect.top(), m_rect.right(), m_rect.top());

	auto positionForValue = [&](double value)
	{
		return std::round(m_rect.left() + (value - m_min) / (m_max - m_min) * m_rect.width());
	};

	for (int i = 0; i < m_numberOfSteps; i++)
	{
		const double value = m_startValue + i * m_stepSize;

		const int textWidth = m_fontMetrics.width(m_stepStrings[i]);
		const int x = positionForValue(value);

		const int crossingPosition = positionForValue(m_crossing);

		painter.drawLine(x, m_rect.top(), x, m_rect.top() + design::markWidth);

		if (m_hasCrossing && x - textWidth / 2 < crossingPosition && crossingPosition < x + textWidth / 2)
			continue;

		painter.setFont(m_font);
		painter.drawText(QRect(x - textWidth / 2, m_rect.top() + design::markWidth + design::axisSpacing, textWidth, textHeight), m_stepStrings[i], QTextOption(Qt::AlignTop | Qt::AlignHCenter));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::paintY(QPainter &painter)
{
	const int textHeight = m_fontMetrics.height();

	painter.setPen(design::gray);
	painter.drawLine(m_rect.left() + m_rect.width(), m_rect.top(), m_rect.left() + m_rect.width(), m_rect.top() + m_rect.height());

	auto positionForValue = [&](double value)
	{
		return std::round(m_rect.top() + m_rect.height() - (value - m_min) / (m_max - m_min) * m_rect.height());
	};

	for (int i = 0; i < m_numberOfSteps; i++)
	{
		const double value = m_startValue + i * m_stepSize;

		const int y = positionForValue(value);

		const int crossingPosition = positionForValue(m_crossing);

		if (m_hasCrossing && y - textHeight / 2 < crossingPosition && crossingPosition < y + textHeight / 2)
			continue;

		painter.drawLine(m_rect.right() - design::markWidth, y, m_rect.right(), y);

		painter.setFont(m_font);
		painter.drawText(QRect(m_rect.left(), y - textHeight / 2, m_rect.width() - design::markWidth - design::axisSpacing, textHeight), m_stepStrings[i], QTextOption(Qt::AlignRight | Qt::AlignVCenter));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QRect &Axis::rect() const
{
	return m_rect;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Axis::maxStringWidth() const
{
	int maxStringWidth = 0;

	for (const QString &stepString : m_stepStrings)
		maxStringWidth = std::max(maxStringWidth, m_fontMetrics.width(stepString));

	return maxStringWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::updateStepStrings()
{
	int precision = m_precision;

	m_stepStrings.clear();

	double firstMark = 0;
	double lastMark = 0;

	const double min = std::min(m_min, m_max);
	const double max = std::max(m_min, m_max);

	if (!m_normalized)
	{
		m_stepSize = std::max(computeStep(std::abs(min)), computeStep(std::abs(max)));

		firstMark = std::ceil(min / m_stepSize);
		lastMark = std::floor(max / m_stepSize);

		m_startValue = firstMark * m_stepSize;
		m_numberOfSteps = lastMark - firstMark + 1;
	}
	else
	{
		m_stepSize = (max - min) / 4.0;
		m_startValue = min;
		m_numberOfSteps = 5;
	}

	for (int i = 0; i < m_numberOfSteps; i++)
	{
		QString number;

		if (!m_normalized)
			number = QString::number(m_startValue + m_stepSize * i, 'f', precision).replace("-", QChar(0x2212));
		else
			number = QString::number(25 * i) + QChar(0x202f) + "%";

		m_stepStrings.push_back(number);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::updateSize()
{
	switch (m_type)
	{
		case X_AXIS:
		{
			const int textHeight = m_fontMetrics.height();
			m_rect.setHeight(design::axisSpacing + design::markWidth + textHeight);
			break;
		}

		case Y_AXIS:
			m_rect.setWidth(design::axisSpacing + design::markWidth + maxStringWidth());
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::setCrossing(double crossing)
{
	m_crossing = crossing;
	m_hasCrossing = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Axis::removeCrossing()
{
	m_hasCrossing = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double Axis::computeStep(double rangeWidth)
{
	if (rangeWidth <= 0.0)
		return 0.0;

	const double lastPowerOf10 = std::pow(10.0, std::floor(std::round(std::log(rangeWidth) / std::log(10.0) * 1.0e6) / 1.0e6));

	for (const auto stepRelative : {2.5, 2.0, 1.0, 0.5, 0.25})
	{
		const double step = stepRelative * lastPowerOf10;

		if (std::ceil(rangeWidth / step) >= 3 && std::ceil(rangeWidth / step) < 6)
			return step;
	}

	return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
