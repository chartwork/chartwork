#include <chartwork/StackedBarChart.h>

#include <QtGui>

#include <numeric>

#include <chartwork/Design.h>
#include <chartwork/elements/Key.h>
#include <chartwork/elements/Title.h>
#include <chartwork/elements/Background.h>
#include <chartwork/exceptions/OutOfRangeException.h>
#include <chartwork/exceptions/EmptyException.h>
#include <chartwork/exceptions/UnknownItemException.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// StackedBarChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

StackedBarChart::StackedBarChart(QWidget *parent)
:	BarChart(parent),
	m_groupLabels(std::shared_ptr<QStringList>(new QStringList({"G1", "G2", "G3", "G4", "G5", "G6"}))),
	m_subGroupLabels(std::shared_ptr<QStringList>(new QStringList({"A", "B", "C"}))),
	m_key(m_subGroupLabels, m_colors),
	m_normalize(true),
	m_fontMetrics(m_font)
{
	resize(400, 300);
	setMinimumSize(200, 150);

	m_title.setText("Stacked Bar Chart");

	m_key.setInvertOrder(true);
	m_yAxis.setNormalized(m_normalize);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool StackedBarChart::showKey() const
{
	return m_key.isVisible();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setShowKey(bool showKey)
{
	m_key.setVisible(showKey);

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double StackedBarChart::keyScale() const
{
	return m_key.scale();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setKeyScale(double keyScale)
{
	m_key.setScale(keyScale);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QStringList &StackedBarChart::groupLabels() const
{
	return *m_groupLabels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setGroupLabels(const QStringList &groupLabels)
{
	*m_groupLabels = groupLabels;

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QStringList &StackedBarChart::subGroupLabels() const
{
	return *m_subGroupLabels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setSubGroupLabels(const QStringList &subGroupLabels)
{
	*m_subGroupLabels = subGroupLabels;

	m_key.updateSize();

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool StackedBarChart::normalize() const
{
	return m_normalize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setNormalize(bool normalize)
{
	m_normalize = normalize;

	m_yAxis.setNormalized(m_normalize);

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::resetValues()
{
	m_groups.clear();
	m_groups.resize(m_groupLabels->size(), std::vector<double>(m_subGroupLabels->size(), 0.0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::generateRandomValues()
{
	resetValues();

	std::srand(0);

	for (std::vector<double> &group : m_groups)
		for (double &value : group)
			value = (std::rand() / (double)RAND_MAX) * m_yMax / m_subGroupLabels->size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	m_title.setWidth(width());
	m_title.moveTopLeft(QPoint(0, design::spacing - m_title.rect().height() / 4));

	const int textHeight = 3.0 / 4.0 * m_fontMetrics.height();
	const int remainingHeight = std::max(0, height() - m_title.rect().height() - 2 * design::spacing - textHeight - design::axisSpacing);

	m_yAxis.setLength(remainingHeight);

	int remainingWidth = std::max(0, width() - m_key.rect().width() - 4 * design::spacing - m_yAxis.rect().width());

	if (!m_key.isVisible())
		remainingWidth += design::spacing;

	m_chartRect.setSize(QSize(remainingWidth, remainingHeight));
	m_chartRect.moveTopLeft(QPoint(2 * design::spacing + m_yAxis.rect().width(), design::spacing + m_title.rect().height()));

	m_key.moveTopRight(QPoint(width() - design::spacing, m_title.rect().height() + design::spacing + (remainingHeight - m_key.rect().height()) / 2));
	m_yAxis.moveTopLeft(QPoint(design::spacing, m_chartRect.top()));

	m_background.paint(painter);
	paint(painter);
	m_key.paint(painter);
	m_yAxis.paint(painter);
	m_title.paint(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::paint(QPainter &painter)
{
	if (m_groupLabels->empty())
		return;

	const int textHeight = m_fontMetrics.height();
	const double groupWidth = std::max(0.0, (double)(m_chartRect.width() - (m_groupLabels->size() - 1) * design::spacing) / m_groupLabels->size());

	for (int g = 0; g < m_groupLabels->size(); g++)
	{
		const double groupX = g * (groupWidth + design::spacing);
		double stackedHeight = 0;

		const double groupSum = std::accumulate(m_groups[g].begin(),
			m_groups[g].end(),
			0.0,
			[](double sum, double v){return sum + std::max(0.0, v);});

		for (int e = 0; e < m_subGroupLabels->size(); e++)
		{
			const int x0 = groupX;
			const int x1 = (g + 1) * (groupWidth + design::spacing) - design::spacing;

			double partHeight;

			if (m_normalize)
				partHeight = std::max(0.0, m_groups[g][e] / groupSum * m_chartRect.height());
			else
				partHeight = std::max(0.0, m_groups[g][e] / m_yMax * m_chartRect.height());

			const double newStackedHeight = std::min((double)m_chartRect.height(), stackedHeight + partHeight);

			painter.setBrush((*m_colors)[e % m_colors->size()]);
			painter.setPen(Qt::NoPen);
			painter.drawRect(m_chartRect.left() + x0, m_chartRect.top() + (m_chartRect.height() - std::round(newStackedHeight)), x1 - x0, std::round(newStackedHeight) - (int)stackedHeight);

			stackedHeight = newStackedHeight;
		}

		painter.setFont(m_font);
		painter.setPen(Qt::SolidLine);
		const QString elidedText = m_fontMetrics.elidedText((*m_groupLabels)[g], Qt::ElideMiddle, groupWidth);
		const double textY = m_chartRect.bottom() + design::axisSpacing;
		painter.drawText(QRect(m_chartRect.left() + groupX, textY, groupWidth, textHeight), elidedText, QTextOption(Qt::AlignCenter));
		painter.setPen(Qt::NoPen);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setValue(int group, int subGroup, double value)
{
	handleValueUpdate();

	if (m_groups.empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "groups");

	if (group < 0 || (size_t)group >= m_groups.size())
		throw exceptions::OutOfRangeException("StackedBarChart", objectName(), "group", group, 0, (int)m_groups.size() - 1);

	if (m_groups[group].empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "subGroups");

	if (subGroup < 0 || (size_t)subGroup >= m_groups[group].size())
		throw exceptions::OutOfRangeException("StackedBarChart", objectName(), "subGroup", subGroup, 0, (int)m_groups[group].size() - 1);

	m_groups[group][subGroup] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setValue(const QString &groupLabel, const QString &subGroupLabel, double value)
{
	if (m_groups.empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "groups");

	const int groupIndex = m_groupLabels->indexOf(groupLabel);

	if (groupIndex < 0)
		throw exceptions::UnknownItemException("StackedBarChart", objectName(), "group", groupLabel, *m_groupLabels);

	if (m_groups[groupIndex].empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "subGroups");

	const int subGroupIndex = m_subGroupLabels->indexOf(subGroupLabel);

	if (subGroupIndex < 0)
		throw exceptions::UnknownItemException("StackedBarChart", objectName(), "subGroup", subGroupLabel, *m_subGroupLabels);

	setValue(groupIndex, subGroupIndex, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void StackedBarChart::setAllValues(double value)
{
	handleValueUpdate();

	m_groups.clear();
	m_groups.resize(m_groupLabels->size(), std::vector<double>(m_subGroupLabels->size(), value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double StackedBarChart::value(int group, int subGroup) const
{
	if (m_groups.empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "groups");

	if (group < 0 || (size_t)group >= m_groups.size())
		throw exceptions::OutOfRangeException("StackedBarChart", objectName(), "group", group, 0, (int)m_groups.size() - 1);

	if (m_groups[group].empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "subGroups");

	if (subGroup < 0 || (size_t)subGroup >= m_groups[group].size())
		throw exceptions::OutOfRangeException("StackedBarChart", objectName(), "subGroup", subGroup, 0, (int)m_groups[group].size() - 1);

	return m_groups[group][subGroup];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double StackedBarChart::value(const QString &groupLabel, const QString &subGroupLabel) const
{
	if (m_groups.empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "groups");

	const int groupIndex = m_groupLabels->indexOf(groupLabel);

	if (groupIndex < 0)
		throw exceptions::UnknownItemException("StackedBarChart", objectName(), "group", groupLabel, *m_groupLabels);

	if (m_groups[groupIndex].empty())
		throw exceptions::EmptyException("StackedBarChart", objectName(), "subGroups");

	const int subGroupIndex = m_subGroupLabels->indexOf(subGroupLabel);

	if (subGroupIndex < 0)
		throw exceptions::UnknownItemException("StackedBarChart", objectName(), "subGroup", subGroupLabel, *m_subGroupLabels);

	return value(groupIndex, subGroupIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
