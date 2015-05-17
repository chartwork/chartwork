#include <chartwork/GroupedBarChart.h>

#include <QtGui>

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
// GroupedBarChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

GroupedBarChart::GroupedBarChart(QWidget *parent)
:	BarChart(parent),
	m_groupLabels(std::shared_ptr<QStringList>(new QStringList({"Group 1", "Group 2", "Group 3"}))),
	m_subGroupLabels(std::shared_ptr<QStringList>(new QStringList({"A", "B", "C"}))),
	m_key(m_subGroupLabels, m_colors),
	m_fontMetrics(m_font)
{
	resize(400, 300);
	setMinimumSize(200, 150);

	m_title.setText("Grouped Bar Chart");

	m_yAxis.setMin(0.0);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool GroupedBarChart::showKey() const
{
	return m_key.isVisible();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::setShowKey(bool showKey)
{
	m_key.setVisible(showKey);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GroupedBarChart::keyScale() const
{
	return m_key.scale();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::setKeyScale(double keyScale)
{
	m_key.setScale(keyScale);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QStringList &GroupedBarChart::groupLabels() const
{
	return *m_groupLabels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::setGroupLabels(const QStringList &groupLabels)
{
	*m_groupLabels = groupLabels;

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QStringList &GroupedBarChart::subGroupLabels() const
{
	return *m_subGroupLabels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::setSubGroupLabels(const QStringList &subGroupLabels)
{
	*m_subGroupLabels = subGroupLabels;

	m_key.updateSize();

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::resetValues()
{
	m_groups.clear();
	m_groups.resize(m_groupLabels->size(), std::vector<double>(m_subGroupLabels->size(), 0.0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::generateRandomValues()
{
	resetValues();

	std::srand(0);

	for (std::vector<double> &group : m_groups)
		for (double &value : group)
			value = (std::rand() / (double)RAND_MAX) * m_yMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::paintEvent(QPaintEvent *)
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

void GroupedBarChart::paint(QPainter &painter)
{
	if (m_groupLabels->empty())
		return;

	const int textHeight = m_fontMetrics.height();
	const double groupWidth = std::max(0.0, (double)(m_chartRect.width() - (m_groupLabels->size() - 1) * design::spacing) / m_groupLabels->size());
	const double barWidth = groupWidth / m_subGroupLabels->size();

	for (int g = 0; g < m_groupLabels->size(); g++)
	{
		const double groupX = g * (groupWidth + design::spacing);

		for (int e = 0; e < m_subGroupLabels->size(); e++)
		{
			const int x0 = groupX + e * barWidth;
			const int x1 = groupX + (e + 1) * barWidth;

			const double height = std::max(0.0, std::min((double)m_chartRect.height(), m_groups[g][e] / m_yMax * m_chartRect.height()));

			painter.setBrush((*m_colors)[e % m_colors->size()]);
			painter.setPen(Qt::NoPen);
			painter.drawRect(m_chartRect.left() + x0, m_chartRect.top() + m_chartRect.height() - height, x1 - x0, height);
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

void GroupedBarChart::setValue(int group, int subGroup, double value)
{
	handleValueUpdate();

	if (m_groups.empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "groups");

	if (group < 0 || (size_t)group >= m_groups.size())
		throw exceptions::OutOfRangeException("GroupedBarChart", objectName(), "group", group, 0, (int)m_groups.size() - 1);

	if (m_groups[group].empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "subGroups");

	if (subGroup < 0 || (size_t)subGroup >= m_groups[group].size())
		throw exceptions::OutOfRangeException("GroupedBarChart", objectName(), "subGroup", subGroup, 0, (int)m_groups[group].size() - 1);

	m_groups[group][subGroup] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::setValue(const QString &groupLabel, const QString &subGroupLabel, double value)
{
	if (m_groups.empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "groups");

	const int groupIndex = m_groupLabels->indexOf(groupLabel);

	if (groupIndex < 0)
		throw exceptions::UnknownItemException("GroupedBarChart", objectName(), "group", groupLabel, *m_groupLabels);

	if (m_groups[groupIndex].empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "subGroups");

	const int subGroupIndex = m_subGroupLabels->indexOf(subGroupLabel);

	if (subGroupIndex < 0)
		throw exceptions::UnknownItemException("GroupedBarChart", objectName(), "subGroup", subGroupLabel, *m_subGroupLabels);

	setValue(groupIndex, subGroupIndex, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroupedBarChart::setAllValues(double value)
{
	handleValueUpdate();

	m_groups.clear();
	m_groups.resize(m_groupLabels->size(), std::vector<double>(m_subGroupLabels->size(), value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GroupedBarChart::value(int group, int subGroup) const
{
	if (m_groups.empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "groups");

	if (group < 0 || (size_t)group >= m_groups.size())
		throw exceptions::OutOfRangeException("GroupedBarChart", objectName(), "group", group, 0, (int)m_groups.size() - 1);

	if (m_groups[group].empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "subGroups");

	if (subGroup < 0 || (size_t)subGroup >= m_groups[group].size())
		throw exceptions::OutOfRangeException("GroupedBarChart", objectName(), "subGroup", subGroup, 0, (int)m_groups[group].size() - 1);

	return m_groups[group][subGroup];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GroupedBarChart::value(const QString &groupLabel, const QString &subGroupLabel) const
{
	if (m_groups.empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "groups");

	const int groupIndex = m_groupLabels->indexOf(groupLabel);

	if (groupIndex < 0)
		throw exceptions::UnknownItemException("GroupedBarChart", objectName(), "group", groupLabel, *m_groupLabels);

	if (m_groups[groupIndex].empty())
		throw exceptions::EmptyException("GroupedBarChart", objectName(), "subGroups");

	const int subGroupIndex = m_subGroupLabels->indexOf(subGroupLabel);

	if (subGroupIndex < 0)
		throw exceptions::UnknownItemException("GroupedBarChart", objectName(), "subGroup", subGroupLabel, *m_subGroupLabels);

	return value(groupIndex, subGroupIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
