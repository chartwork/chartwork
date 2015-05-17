#ifndef __PIE_CHART_H
#define __PIE_CHART_H

#include <QWidget>

#include <vector>
#include <memory>

#include <chartwork/Chart.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PieChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT PieChart : public Chart
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(QStringList labels READ labels WRITE setLabels)
	Q_PROPERTY(bool showKey READ showKey WRITE setShowKey)
	Q_PROPERTY(double keyScale READ keyScale WRITE setKeyScale)
	Q_PROPERTY(bool showPercentage READ showPercentage WRITE setShowPercentage)
	Q_PROPERTY(int precision READ precision WRITE setPrecision)

	Q_PROPERTY(QColor color0 READ color0 WRITE setColor0)
	Q_PROPERTY(QColor color1 READ color1 WRITE setColor1)
	Q_PROPERTY(QColor color2 READ color2 WRITE setColor2)
	Q_PROPERTY(QColor color3 READ color3 WRITE setColor3)
	Q_PROPERTY(QColor color4 READ color4 WRITE setColor4)
	Q_PROPERTY(QColor color5 READ color5 WRITE setColor5)
	Q_PROPERTY(QColor color6 READ color6 WRITE setColor6)
	Q_PROPERTY(QColor color7 READ color7 WRITE setColor7)

	public:
		PieChart(QWidget *parent = 0);

		const QStringList &labels() const;
		void setLabels(const QStringList &labels);

		// Get/set whether the key is shown (right of the chart)
		bool showKey() const;
		void setShowKey(bool showKey);

		// Get/set the displayed scale/size of the chart's key
		// 1.0 = default size, 0.5 = half the size
		double keyScale() const;
		void setKeyScale(double keyScale);

		// Get/set the precision of the pie labels (number of digits after the comma)
		int precision() const;
		void setPrecision(int precision);

		// Get/set whether the '%' sign is shown in the pie labels
		bool showPercentage() const;
		void setShowPercentage(bool showPercentage);

		// Get/set the values of individual pie slices
		void setValue(int slice, double value);
		void setValue(const QString &label, double value);
		double value(int slice) const;
		double value(const QString &label) const;

		// Set the values of all pie slices simultaneously
		void setAllValues(double value);

	private:
		void resetValues();
		void generateRandomValues();

		double diameter(int remainingWidth, int remainingHeight) const;

		void paintEvent(QPaintEvent *event);
		void paint(QPainter &painter);

		QFont m_font;
		QFontMetrics m_fontMetrics;

		bool m_showPercentage;
		int m_precision;

		std::shared_ptr<QStringList> m_labels;
		std::vector<double> m_values;

		elements::Key m_key;

		QRect m_chartRect;
		double m_diameter;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
