#include <QtGui>
#include <QColor>

#include <cstdlib>
#include <cassert>

#include <chartwork/GridChart.h>
#include <chartwork/Design.h>
#include <chartwork/exceptions/OutOfRangeException.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GridChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

GridChart::GridChart(QWidget *parent)
:	Chart(parent),
	m_keyFont(QFont()),
	m_keyFontMetrics(m_keyFont),
	m_antialiasing(true),
	m_gridSize(QSize(10, 10)),
	m_minValue(0.0),
	m_maxValue(100.0),
	m_precision(0),
	m_keyStrings(new QStringList),
	m_numberOfColors(5),
	m_background(this),
	m_key(m_keyStrings, m_colors)
{
	setWindowTitle(tr("Grid Chart"));
	resize(400, 300);
	setMinimumSize(200, 150);

	m_title.setText("Grid Chart");

	(*m_colors)[0] = design::red;
	(*m_colors)[1] = design::yellow;
	(*m_colors)[2] = design::green;
	(*m_colors)[3] = design::blue;
	(*m_colors)[4] = design::purple;
	(*m_colors)[5] = design::gray;
	(*m_colors)[6] = design::brown;
	(*m_colors)[7] = design::orange;

	generateRandomValues();
	updateKeyStrings();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool GridChart::showKey() const
{
	return m_key.isVisible();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setShowKey(bool showKey)
{
	m_key.setVisible(showKey);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GridChart::keyScale() const
{
	return m_key.scale();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setKeyScale(double keyScale)
{
	m_key.setScale(keyScale);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool GridChart::antialiasing() const
{
	return m_antialiasing;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setAntialiasing(bool antialiasing)
{
	m_antialiasing = antialiasing;

	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QSize GridChart::gridSize() const
{
	return m_gridSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setGridSize(QSize gridSize)
{
	m_gridSize = QSize(std::max(1, gridSize.width()), std::max(1, gridSize.height()));

	generateRandomValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GridChart::minValue() const
{
	return m_minValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setMinValue(double minValue)
{
	m_minValue = minValue;

	generateRandomValues();
	updateKeyStrings();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GridChart::maxValue() const
{
	return m_maxValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setMaxValue(double maxValue)
{
	m_maxValue = maxValue;

	generateRandomValues();
	updateKeyStrings();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int GridChart::precision() const
{
	return m_precision;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setPrecision(int precision)
{
	m_precision = std::max(0, precision);
	updateKeyStrings();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::resetValues()
{
	m_values.clear();
	m_values.resize(m_gridSize.width(), std::vector<double>(m_gridSize.height(), m_minValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::generateRandomValues()
{
	resetValues();

	std::srand(0);

	for (std::vector<double> &col : m_values)
		for (double &value : col)
			value = m_minValue + std::rand() / (double)RAND_MAX * (m_maxValue - m_minValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::updateKeyStrings()
{
	m_keyStrings->clear();

	for (size_t i = 0; i < m_numberOfColors; i++)
	{
		const double value = m_minValue + (m_maxValue - m_minValue) * i / (double)(m_numberOfColors - 1);
		m_keyStrings->push_back(QString::number(value, 'f', m_precision).replace("-", "−"));
	}

	m_key.updateSize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GridChart::blockSize(int remainingWidth, int remainingHeight) const
{
	return std::max(0.0, std::min((double)remainingWidth / m_gridSize.width(), (double)remainingHeight / m_gridSize.height()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QSize GridChart::gridSize(int remainingWidth, int remainingHeight) const
{
	return QSize(m_gridSize) * blockSize(remainingWidth, remainingHeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::paintEvent(QPaintEvent *)
{
	assert(m_values.size() == (size_t)m_gridSize.width());
	assert(!m_values.empty());
	assert(m_values.front().size() == (size_t)m_gridSize.height());
	assert(!m_values.front().empty());

	m_title.setWidth(width());
	m_title.moveTopLeft(QPoint(0, design::spacing - m_title.rect().height() / 4));

	int remainingWidth = width() - m_key.rect().width() - 3 * design::spacing;

	if (!m_key.isVisible())
		remainingWidth += design::spacing;

	const int remainingHeight = height() - m_title.rect().height() - 2 * design::spacing;
	m_blockSize = GridChart::blockSize(remainingWidth, remainingHeight);

	m_chartRect.setSize(gridSize(remainingWidth, remainingHeight));

	const int outerSpacing = (remainingWidth - m_chartRect.width()) / 2;

	m_chartRect.moveLeft(design::spacing + outerSpacing);
	m_chartRect.moveTop(design::spacing + m_title.rect().height() + (remainingHeight - m_chartRect.height()) / 2);

	m_key.moveTopRight(QPoint(m_chartRect.left() + m_chartRect.width() + design::spacing + m_key.rect().width(), design::spacing + m_title.rect().height() + (remainingHeight - m_key.rect().height()) / 2));

	QPainter painter(this);

	m_background.paint(painter);
	m_title.paint(painter);
	paint(painter);
	m_key.paint(painter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::paint(QPainter &painter)
{
	// Interpolation for HSV
	auto interpolate = [](const QColor &color0, const QColor &color1, double value) -> QColor
	{
		qreal h0, s0, v0, a0;
		qreal h1, s1, v1, a1;

		color0.getHsvF(&h0, &s0, &v0, &a0);
		color1.getHsvF(&h1, &s1, &v1, &a1);

		const qreal d = std::abs(h0 - h1);

		qreal h;

		if (d > 0.5 && h0 < h1)
			h0 += 1.0;
		else if (d > 0.5)
			h1 += 1.0;

		h = h0 * (1 - value) + h1 * (value);

		if (h > 1.0)
			h -= 1.0;

		const qreal s = s0 * (1 - value) + s1 * value;
		const qreal v = v0 * (1 - value) + v1 * value;
		const qreal a = a0 * (1 - value) + a1 * value;

		return QColor::fromHsvF(h, s, v, a);
	};

	if (m_antialiasing)
		painter.setRenderHint(QPainter::Antialiasing);

	// Draw grid
	for (size_t x = 0; x < (size_t)m_gridSize.width(); x++)
		for (size_t y = 0; y < (size_t)m_gridSize.height(); y++)
		{
			const double value = std::max(0.0, std::min(1.0, (m_values[x][y] - m_minValue) / (m_maxValue - m_minValue))) * (m_numberOfColors - 1);

			if (m_numberOfColors > 1)
			{
				for (int i = 1; i <= m_numberOfColors - 1; i++)
					if (value <= i)
					{
						painter.setBrush(interpolate((*m_colors)[i - 1], (*m_colors)[i], value - (i - 1)));
						break;
					}
			}
			else
				painter.setBrush((*m_colors)[0]);

			double antialiasingXOffset = 0;
			double antialiasingYOffset = 0;

			if (m_antialiasing)
			{
				antialiasingXOffset = (x == 0 ? 0.0 : 0.5);
				antialiasingYOffset = (y == 0 ? 0.0 : 0.5);
			}

			const double x0 = m_chartRect.left() + x * m_blockSize - antialiasingXOffset;
			const double y0 = m_chartRect.top() + y * m_blockSize - antialiasingYOffset;
			const double x1 = m_chartRect.left() + (x + 1) * m_blockSize;
			const double y1 = m_chartRect.top() + (y + 1) * m_blockSize;

			const QRectF rectangle(x0, y0, x1 - x0, y1 - y0);

			painter.drawRect(rectangle);
		}

	if (m_antialiasing)
		painter.setRenderHint(QPainter::Antialiasing, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int GridChart::numberOfColors() const
{
	return m_numberOfColors;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setNumberOfColors(int numberOfColors)
{
	m_numberOfColors = std::min(8, std::max(2, (int)numberOfColors));

	updateKeyStrings();
	update();
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setValue(int x, int y, double value)
{
	handleValueUpdate();

	if (x < 0 || x >= m_gridSize.width())
		throw exceptions::OutOfRangeException("GridChart", objectName(), "x", x, 0, m_gridSize.width() - 1);

	if (y < 0 || y >= m_gridSize.height())
		throw exceptions::OutOfRangeException("GridChart", objectName(), "y", y, 0, m_gridSize.height() - 1);

	m_values[x][y] = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChart::setAllValues(double value)
{
	handleValueUpdate();

	m_values.clear();
	m_values.resize(m_gridSize.width(), std::vector<double>(m_gridSize.height(), value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

double GridChart::value(int x, int y) const
{
	if (x < 0 || x >= m_gridSize.width())
		throw exceptions::OutOfRangeException("GridChart", objectName(), "x", x, 0, m_gridSize.width() - 1);

	if (y < 0 || y >= m_gridSize.height())
		throw exceptions::OutOfRangeException("GridChart", objectName(), "y", y, 0, m_gridSize.height() - 1);

	return m_values[x][y];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
