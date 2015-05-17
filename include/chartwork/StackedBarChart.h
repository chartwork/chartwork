#ifndef __STACKED_BAR_CHART_H
#define __STACKED_BAR_CHART_H

#include <chartwork/BarChart.h>
#include <chartwork/elements/Axis.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// StackedBarChart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT StackedBarChart : public BarChart
{
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(bool showKey READ showKey WRITE setShowKey)
	Q_PROPERTY(double keyScale READ keyScale WRITE setKeyScale)
	Q_PROPERTY(double yMax READ yMax WRITE setYMax)
	Q_PROPERTY(int yPrecision READ yPrecision WRITE setYPrecision)
	Q_PROPERTY(bool normalize READ normalize WRITE setNormalize)
	Q_PROPERTY(QStringList groupLabels READ groupLabels WRITE setGroupLabels)
	Q_PROPERTY(QStringList subGroupLabels READ subGroupLabels WRITE setSubGroupLabels)

	Q_PROPERTY(QColor color0 READ color0 WRITE setColor0)
	Q_PROPERTY(QColor color1 READ color1 WRITE setColor1)
	Q_PROPERTY(QColor color2 READ color2 WRITE setColor2)
	Q_PROPERTY(QColor color3 READ color3 WRITE setColor3)
	Q_PROPERTY(QColor color4 READ color4 WRITE setColor4)
	Q_PROPERTY(QColor color5 READ color5 WRITE setColor5)
	Q_PROPERTY(QColor color6 READ color6 WRITE setColor6)
	Q_PROPERTY(QColor color7 READ color7 WRITE setColor7)

	public:
		StackedBarChart(QWidget *parent = 0);

		// Get/set whether the key is shown (right of the chart)
		bool showKey() const;
		void setShowKey(bool showKey);

		double keyScale() const;
		void setKeyScale(double keyScale);

		bool normalize() const;
		void setNormalize(bool normalize);

		const QStringList &groupLabels() const;
		void setGroupLabels(const QStringList &groupLabels);

		const QStringList &subGroupLabels() const;
		void setSubGroupLabels(const QStringList &subGroupLabels);

		void setValue(int group, int subGroup, double value);
		void setValue(const QString &groupLabel, const QString &subGroupLabel, double value);
		void setAllValues(double value);
		double value(int group, int subGroup) const;
		double value(const QString &groupLabel, const QString &subGroupLabel) const;

	private:
		void resetValues();
		void generateRandomValues();

		void paintEvent(QPaintEvent *);
		void paint(QPainter &painter);

		std::shared_ptr<QStringList> m_groupLabels;
		std::shared_ptr<QStringList> m_subGroupLabels;

		std::vector<std::vector<double>> m_groups;

		elements::Key m_key;
		QRect m_chartRect;

		bool m_normalize;

		QFont m_font;
		QFontMetrics m_fontMetrics;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
