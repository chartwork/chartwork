#include <chartwork/XYPlot.h>

#include <QtGui>
#include <QColor>
#include <QPointF>
#include <QDynamicPropertyChangeEvent>

#include <cassert>

#include <chartwork/Design.h>
#include <chartwork/elements/Key.h>
#include <chartwork/elements/Background.h>
#include <chartwork/elements/Title.h>
#include <chartwork/exceptions/OutOfRangeException.h>
#include <chartwork/exceptions/EmptyException.h>
#include <chartwork/exceptions/UnknownItemException.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XYPlot
//
////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::XYPlot(QWidget *parent)
:	Chart(parent),
	m_xAxis(elements::Axis::X_AXIS),
	m_yAxis(elements::Axis::Y_AXIS),
	m_xMin(0.0),
	m_xMax(100.0),
	m_xPrecision(0),
	m_yMin(0.0),
	m_yMax(100.0),
	m_yPrecision(0),
	m_labels(std::shared_ptr<QStringList>(new QStringList({"A", "B", "C"}))),
	m_fontMetrics(m_font),
	m_symbolSize(3),
	m_symbols(std::shared_ptr<QList<int>>(new QList<int>({Circle, Square, Diamond, Plus, CircleOutline, SquareOutline, DiamondOutline, Cross}))),
	m_key(m_labels, m_colors)
{
	m_xAxis.setMin(m_xMin);
	m_xAxis.setMax(m_xMax);
	m_xAxis.setPrecision(m_xPrecision);

	m_yAxis.setMin(m_yMin);
	m_yAxis.setMax(m_yMax);
	m_yAxis.setPrecision(m_yPrecision);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool XYPlot::showKey() const
{
	return m_key.isVisible();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setShowKey(bool showKey)
{
	m_key.setVisible(showKey);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double XYPlot::keyScale() const
{
	return m_key.scale();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setKeyScale(double keyScale)
{
	m_key.setScale(keyScale);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double XYPlot::xMin() const
{
	return m_xMin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setXMin(double xMin)
{
	m_xMin = xMin;
	m_xAxis.setMin(xMin);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double XYPlot::xMax() const
{
	return m_xMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setXMax(double xMax)
{
	m_xMax = xMax;
	m_xAxis.setMax(xMax);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int XYPlot::xPrecision() const
{
	return m_xPrecision;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setXPrecision(int xPrecision)
{
	m_xPrecision = std::max(0, xPrecision);
	m_xAxis.setPrecision(xPrecision);

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double XYPlot::yMin() const
{
	return m_yMin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setYMin(double yMin)
{
	m_yMin = yMin;
	m_yAxis.setMin(yMin);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double XYPlot::yMax() const
{
	return m_yMax;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setYMax(double yMax)
{
	m_yMax = yMax;
	m_yAxis.setMax(yMax);

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int XYPlot::yPrecision() const
{
	return m_yPrecision;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setYPrecision(int yPrecision)
{
	m_yPrecision = std::max(0, yPrecision);
	m_yAxis.setPrecision(yPrecision);

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QStringList &XYPlot::labels() const
{
	return *m_labels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setLabels(const QStringList &labels)
{
	*m_labels = labels;

	m_key.updateSize();

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	m_title.setWidth(width());
	m_title.moveTopLeft(QPoint(0, design::spacing - m_title.rect().height() / 4));

	const int remainingHeight = std::max(0, height() - m_title.rect().height() - 2 * design::spacing);
	int remainingWidth = std::max(0, width() - m_key.rect().width() - 3 * design::spacing);

	if (!m_key.isVisible())
		remainingWidth += design::spacing;

	m_xAxis.setLength(remainingWidth);
	m_yAxis.setLength(remainingHeight);

	m_chartRect.setSize(QSize(remainingWidth, remainingHeight));
	m_chartRect.moveTopLeft(QPoint(design::spacing, design::spacing + m_title.rect().height()));

	m_xAxis.moveLeft(m_chartRect.left());
	m_yAxis.moveTop(m_chartRect.top());

	m_key.moveTopRight(QPoint(width() - design::spacing, m_title.rect().height() + design::spacing + (remainingHeight - m_key.rect().height()) / 2));

	m_background.paint(painter);
	paint(painter);
	m_key.paint(painter);
	m_title.paint(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::paint(QPainter &painter)
{
	m_visibleXMin = m_xMin;
	m_visibleXMax = m_xMax;
	m_visibleYMin = m_yMin;
	m_visibleYMax = m_yMax;

	double xAxisPosition = 0.0;
	double yAxisPosition = 0.0;

	bool xAxisHasCrossing = true;
	bool yAxisHasCrossing = true;

	QPointF originF = positionForValue(std::pair<double, double>(0.0, 0.0));
	QPoint origin(std::round(originF.x()), std::round(originF.y()));

	if (origin.x() - m_yAxis.rect().width() < m_chartRect.left())
	{
		m_visibleXMin = (m_chartRect.width() * m_xMin - m_yAxis.rect().width() * m_xMax) / (m_chartRect.width() - m_yAxis.rect().width());
		yAxisPosition = m_xMin;

		m_xAxis.setLength(m_chartRect.width() - m_yAxis.rect().width());
		m_xAxis.moveLeft(m_chartRect.left() + m_yAxis.rect().width());

		yAxisHasCrossing = false;
	}
	else if (origin.x() >= m_chartRect.left() + m_chartRect.width())
		yAxisPosition = m_xMax;

	if (origin.y() + m_xAxis.rect().height() >= m_chartRect.top() + m_chartRect.height())
	{
		m_visibleYMin = (m_chartRect.height() * m_yMin - m_xAxis.rect().height() * m_yMax) / (m_chartRect.height() - m_xAxis.rect().height());
		xAxisPosition = m_yMin;

		m_yAxis.setLength(m_chartRect.height() - m_xAxis.rect().height());

		xAxisHasCrossing = false;
	}
	else if (origin.y() < m_chartRect.top())
		xAxisPosition = m_yMax;

	originF = positionForValue(std::pair<double, double>(yAxisPosition, xAxisPosition));
	origin = QPoint(std::round(originF.x()), std::round(originF.y()));

	if (xAxisHasCrossing)
		m_xAxis.setCrossing(yAxisPosition);
	else
		m_xAxis.removeCrossing();

	if (yAxisHasCrossing)
		m_yAxis.setCrossing(xAxisPosition);
	else
		m_yAxis.removeCrossing();

	m_xAxis.moveTop(origin.y());
	m_yAxis.moveLeft(origin.x() - m_yAxis.rect().width());

	m_xAxis.paint(painter);
	m_yAxis.paint(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QPointF XYPlot::positionForValue(const std::pair<double, double> &value) const
{
	const double x = m_chartRect.left() + (double)(value.first - m_visibleXMin) / (m_visibleXMax - m_visibleXMin) * m_chartRect.width();
	const double y = m_chartRect.top() + m_chartRect.height() - (double)(value.second - m_visibleYMin) / (m_visibleYMax - m_visibleYMin) * m_chartRect.height();

	return QPointF(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int XYPlot::symbolSize() const
{
	return m_symbolSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbolSize(int symbolSize)
{
	m_symbolSize = std::max(2, std::min(10, symbolSize));
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol0() const
{
	return (XYPlot::SymbolType)(*m_symbols)[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol0(XYPlot::SymbolType symbol0)
{
	(*m_symbols)[0] = symbol0;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol1() const
{
	return (XYPlot::SymbolType)(*m_symbols)[1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol1(XYPlot::SymbolType symbol1)
{
	(*m_symbols)[1] = symbol1;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol2() const
{
	return (XYPlot::SymbolType)(*m_symbols)[2];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol2(XYPlot::SymbolType symbol2)
{
	(*m_symbols)[2] = symbol2;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol3() const
{
	return (XYPlot::SymbolType)(*m_symbols)[3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol3(XYPlot::SymbolType symbol3)
{
	(*m_symbols)[3] = symbol3;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol4() const
{
	return (XYPlot::SymbolType)(*m_symbols)[4];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol4(XYPlot::SymbolType symbol4)
{
	(*m_symbols)[4] = symbol4;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol5() const
{
	return (XYPlot::SymbolType)(*m_symbols)[5];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol5(XYPlot::SymbolType symbol5)
{
	(*m_symbols)[5] = symbol5;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol6() const
{
	return (XYPlot::SymbolType)(*m_symbols)[6];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol6(XYPlot::SymbolType symbol6)
{
	(*m_symbols)[6] = symbol6;
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

XYPlot::SymbolType XYPlot::symbol7() const
{
	return (XYPlot::SymbolType)(*m_symbols)[7];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::setSymbol7(XYPlot::SymbolType symbol7)
{
	(*m_symbols)[7] = symbol7;
	update();
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::addData(int dataSeries, double x, double y)
{
	handleValueUpdate();

	if (m_labels->isEmpty())
		throw exceptions::EmptyException(metaObject()->className(), objectName(), "labels");

	if (dataSeries < 0 || dataSeries >= m_labels->size())
		throw exceptions::OutOfRangeException(metaObject()->className(), objectName(), "dataSeries", dataSeries, 0, m_labels->size() - 1);

	m_values[dataSeries].insert(Entry(x, y));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::addData(const QString &label, double x, double y)
{
	if (m_labels->isEmpty())
		throw exceptions::EmptyException(metaObject()->className(), objectName(), "labels");

	int dataSeries = m_labels->indexOf(label);

	if (dataSeries < 0)
		throw exceptions::UnknownItemException(metaObject()->className(), objectName(), "label", label, *m_labels);

	addData(dataSeries, x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::clearData(int dataSeries)
{
	handleValueUpdate();

	if (m_labels->isEmpty())
		throw exceptions::EmptyException(metaObject()->className(), objectName(), "labels");

	if (dataSeries < 0 || dataSeries >= m_labels->size())
		throw exceptions::OutOfRangeException(metaObject()->className(), objectName(), "dataSeries", dataSeries, 0, m_labels->size() - 1);

	m_values[dataSeries].clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::clearData(const QString &label)
{
	handleValueUpdate();

	if (m_labels->isEmpty())
		throw exceptions::EmptyException(metaObject()->className(), objectName(), "labels");

	int dataSeries = m_labels->indexOf(label);

	if (dataSeries < 0)
		throw exceptions::UnknownItemException(metaObject()->className(), objectName(), "label", label, *m_labels);

	clearData(dataSeries);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::clearAllData()
{
	m_values.clear();
	m_values.resize(m_labels->size(), EntryMultiSet([](const Entry &a, const Entry &b){return a.first < b.first;}));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void XYPlot::resetValues()
{
	clearAllData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
