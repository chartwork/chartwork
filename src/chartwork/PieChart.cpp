#include <QtGui>
#include <QColor>

#include <cstdlib>
#include <cassert>
#include <numeric>

#include <chartwork/PieChart.h>
#include <chartwork/Design.h>
#include <chartwork/exceptions/OutOfRangeException.h>
#include <chartwork/exceptions/EmptyException.h>
#include <chartwork/exceptions/UnknownItemException.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PieChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

PieChart::PieChart(QWidget *parent)
:	Chart(parent),
	m_fontMetrics(m_font),
	m_showPercentage(true),
	m_precision(1),
	m_labels(std::shared_ptr<QStringList>(new QStringList({"A", "B", "C", "D"}))),
	m_key(m_labels, m_colors)
{
	setWindowTitle(tr("Pie Chart"));
	resize(400, 300);
	setMinimumSize(200, 150);

	generateRandomValues();

	m_title.setText("Pie Chart");

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QStringList &PieChart::labels() const
{
	return *m_labels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setLabels(const QStringList &labels)
{
	*m_labels = labels;

	m_key.updateSize();

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool PieChart::showKey() const
{
	return m_key.isVisible();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setShowKey(bool showKey)
{
	m_key.setVisible(showKey);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double PieChart::keyScale() const
{
	return m_key.scale();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setKeyScale(double keyScale)
{
	m_key.setScale(keyScale);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool PieChart::showPercentage() const
{
	return m_showPercentage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setShowPercentage(bool showPercentage)
{
	m_showPercentage = showPercentage;

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int PieChart::precision() const
{
	return m_precision;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setPrecision(int precision)
{
	m_precision = std::max(0, precision);

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::resetValues()
{
	m_values.clear();
	m_values.resize(m_labels->size(), 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::generateRandomValues()
{
	resetValues();

	std::srand(0);

	for (int i = 0; i < m_labels->size(); i++)
		m_values[i] = std::rand() / (double)RAND_MAX * 100;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double PieChart::diameter(int remainingWidth, int remainingHeight) const
{
	return std::max(0.0, std::min((double)remainingWidth, (double)remainingHeight));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::paintEvent(QPaintEvent *)
{
	if (m_labels->empty())
		return;

	if (m_values.size() < (size_t)m_labels->size())
		return;

	m_title.setWidth(width());
	m_title.moveTopLeft(QPoint(0, design::spacing - m_title.rect().height() / 4));

	int remainingWidth = width() - m_key.rect().width() - 3 * design::spacing;

	if (!m_key.isVisible())
		remainingWidth += design::spacing;

	const int remainingHeight = height() - m_title.rect().height() - 2 * design::spacing;

	m_diameter = diameter(remainingWidth, remainingHeight);

	m_chartRect.setSize(QSize(m_diameter, m_diameter));

	const int outerSpacing = (remainingWidth - m_diameter) / 2;

	m_chartRect.moveLeft(design::spacing + outerSpacing);
	m_chartRect.moveTop(design::spacing + m_title.rect().height() + (remainingHeight - m_diameter) / 2);

	m_key.moveTopRight(QPoint(m_chartRect.left() + m_chartRect.width() + design::spacing + m_key.rect().width(), design::spacing + m_title.rect().height() + (remainingHeight - m_key.rect().height()) / 2));

	QPainter painter(this);

	m_background.paint(painter);
	m_title.paint(painter);
	paint(painter);
	m_key.paint(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::paint(QPainter &painter)
{
	const double sumOfValues = std::accumulate(m_values.begin(),
		m_values.end(), 0.0, [](double sum, double v){return sum + std::max(0.0, v);});

	painter.setRenderHint(QPainter::Antialiasing);

	double currentAngle = 0;

	for (size_t i = 0; i < m_values.size(); i++)
	{
		const double angleValue = m_values[i] / sumOfValues * 360 * 16;

		if (angleValue <= 0.0)
			continue;

		const QColor &color = (*m_colors)[i % m_colors->size()];
		painter.setBrush(color);
		painter.drawPie(m_chartRect, std::round(-currentAngle + 90 * 16 - angleValue), std::round(angleValue));
		currentAngle += angleValue;
	}

	painter.setRenderHint(QPainter::Antialiasing,false);

	if (!m_showPercentage)
		return;

	currentAngle = 0;

	for (size_t i = 0; i < m_values.size(); i++)
	{
		const double angleValue = m_values[i] / sumOfValues * 2.0 * M_PI;

		if (angleValue <= 0.0)
			continue;

		const double midAngle = -currentAngle + M_PI_2 - angleValue / 2.0f;

		painter.setPen(Qt::SolidLine);
		painter.setFont(m_font);

		const QColor &color = (*m_colors)[i % m_colors->size()];
		if (color.value() < 210)
			painter.setPen(design::superlightgray);

		const QString percentageText = QString("%1").arg(m_values[i] / sumOfValues * 100.0, 0, 'f', m_precision) + QString(" %");

		const QPoint textCenter = m_chartRect.center() + QPoint(m_diameter / 3 * std::cos(midAngle), -m_diameter / 3 * std::sin(midAngle));
		const double textWidth = m_fontMetrics.horizontalAdvance(percentageText) + 24;
		const double textHeight = m_fontMetrics.height() + 24;

		painter.drawText(QRect(textCenter.x() - textWidth / 2,
							   textCenter.y() - textHeight / 2,
							   textWidth,
							   textHeight),
							   percentageText, QTextOption(Qt::AlignCenter));
		painter.setPen(Qt::NoPen);

		currentAngle += angleValue;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setValue(int slice, double value)
{
	handleValueUpdate();

	if (m_values.empty())
		throw exceptions::EmptyException("PieChart", objectName(), "labels");

	if (slice < 0 || (size_t)slice >= m_values.size())
		throw exceptions::OutOfRangeException("PieChart", objectName(), "slice", slice, 0, (int)m_values.size() - 1);

	m_values[slice] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setValue(const QString &label, double value)
{
	if (m_values.empty())
		throw exceptions::EmptyException("PieChart", objectName(), "labels");

	const int sliceIndex = m_labels->indexOf(label);

	if (sliceIndex < 0)
		throw exceptions::UnknownItemException("PieChart", objectName(), "label", label, *m_labels);

	setValue(sliceIndex, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PieChart::setAllValues(double value)
{
	handleValueUpdate();

	m_values.clear();
	m_values.resize(m_labels->size(), value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double PieChart::value(int slice) const
{
	if (m_values.empty())
		throw exceptions::EmptyException("PieChart", objectName(), "labels");

	if (slice < 0 || (size_t)slice >= m_values.size())
		throw exceptions::OutOfRangeException("PieChart", objectName(), "slice", slice, 0, (int)m_values.size() - 1);

	return m_values[slice];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double PieChart::value(const QString &label) const
{
	if (m_values.empty())
		throw exceptions::EmptyException("PieChart", objectName(), "labels");

	const int sliceIndex = m_labels->indexOf(label);

	if (sliceIndex < 0)
		throw exceptions::UnknownItemException("PieChart", objectName(), "label", label, *m_labels);

	return value(sliceIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
