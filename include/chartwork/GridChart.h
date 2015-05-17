#ifndef __GRID_CHART_H
#define __GRID_CHART_H

#include <QWidget>
#include <QtDesigner/QDesignerExportWidget>

#include <vector>
#include <memory>

#include <chartwork/Chart.h>
#include <chartwork/elements/Background.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GridChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT GridChart : public Chart
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(bool showKey READ showKey WRITE setShowKey)
	Q_PROPERTY(double keyScale READ keyScale WRITE setKeyScale)
	Q_PROPERTY(bool antialiasing READ antialiasing WRITE setAntialiasing)

	Q_PROPERTY(QSize gridSize READ gridSize WRITE setGridSize)
	Q_PROPERTY(double minValue READ minValue WRITE setMinValue)
	Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue)
	Q_PROPERTY(int precision READ precision WRITE setPrecision)

	Q_PROPERTY(int numberOfColors READ numberOfColors WRITE setNumberOfColors)

	Q_PROPERTY(QColor color0 READ color0 WRITE setColor0)
	Q_PROPERTY(QColor color1 READ color1 WRITE setColor1)
	Q_PROPERTY(QColor color2 READ color2 WRITE setColor2)
	Q_PROPERTY(QColor color3 READ color3 WRITE setColor3)
	Q_PROPERTY(QColor color4 READ color4 WRITE setColor4)
	Q_PROPERTY(QColor color5 READ color5 WRITE setColor5)
	Q_PROPERTY(QColor color6 READ color6 WRITE setColor6)
	Q_PROPERTY(QColor color7 READ color7 WRITE setColor7)

	public:
		GridChart(QWidget *parent = 0);

		// Get/set whether the key is shown (right of the chart)
		bool showKey() const;
		void setShowKey(bool showKey);

		// Get/set the displayed scale/size of the chart's key
		// 1.0 = default size, 0.5 = half the size
		double keyScale() const;
		void setKeyScale(double keyScale);

		// Get/set whether the grid cells are antialiased
		bool antialiasing() const;
		void setAntialiasing(bool antialiasing);

		// Get/set the grid size (width = columns, height = rows)
		QSize gridSize() const;
		void setGridSize(QSize gridSize);

		// Get/set the minimum value; will be mapped to the first color in the color palette
		double minValue() const;
		void setMinValue(double minValue);

		// Get/set the maximum value; will be mapped to the last color in the color palette
		double maxValue() const;
		void setMaxValue(double maxValue);

		// Get/set the precision of the numbers shown in the chart's key
		int precision() const;
		void setPrecision(int precision);

		// Get/set how many colors in the palette are used (always starts at the first color)
		int numberOfColors() const;
		void setNumberOfColors(int numberOfColors);

		// Get/set the values of individual cells
		void setValue(int x, int y, double value);
		double value(int x, int y) const;

		// Set the value of all grids simultaneously
		void setAllValues(double value);

	private:
		void resetValues();
		void generateRandomValues();

		void updateKeyStrings();
		double blockSize(int remainingWidth, int remainingHeight) const;
		QSize gridSize(int remainingWidth, int remainingHeight) const;

		void paintEvent(QPaintEvent *event);
		void paint(QPainter &painter);

		QFont m_titleFont;
		QFont m_keyFont;
		QFontMetrics m_keyFontMetrics;

		bool m_antialiasing;

		QSize m_gridSize;

		double m_minValue;
		double m_maxValue;
		int m_precision;
		std::shared_ptr<QStringList> m_keyStrings;

		int m_numberOfColors;

		std::vector<std::vector<double>> m_values;

		elements::Background m_background;
		elements::Key m_key;

		QRect m_chartRect;
		double m_blockSize;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
