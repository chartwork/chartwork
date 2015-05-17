#ifndef __XY_PLOT_H
#define __XY_PLOT_H

#include <set>

#include <chartwork/Chart.h>
#include <chartwork/elements/Axis.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XYPlot
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT XYPlot : public Chart
{
	Q_OBJECT

	Q_ENUMS(SymbolType)

	public:
		enum SymbolType
		{
			Circle,
			Square,
			Diamond,
			Plus,
			CircleOutline,
			SquareOutline,
			DiamondOutline,
			Cross
		};

		XYPlot(QWidget *parent = 0);

		// Get/set whether the key is shown (right of the chart)
		bool showKey() const;
		void setShowKey(bool showKey);

		// Get/set the displayed scale/size of the chart's key
		// 1.0 = default size, 0.5 = half the size
		double keyScale() const;
		void setKeyScale(double keyScale);

		// Get/set the displayed minimum value of the chart's x axis
		double xMin() const;
		void setXMin(double xMin);

		// Get/set the displayed maximum value of the chart's x axis
		double xMax() const;
		void setXMax(double xMax);

		// Get/set the precision of the chart's x axis (number of digits after the comma)
		int xPrecision() const;
		void setXPrecision(int xPrecision);

		// Get/set the displayed minimum value of the chart's y axis
		double yMin() const;
		void setYMin(double yMin);

		// Get/set the displayed maximum value of the chart's y axis
		double yMax() const;
		void setYMax(double yMax);

		// Get/set the precision of the chart's y axis (number of digits after the comma)
		int yPrecision() const;
		void setYPrecision(int yPrecision);

		// Get/set the labels of the data series in the chart
		const QStringList &labels() const;
		void setLabels(const QStringList &labels);

		// Get/set the size of the symbols shown at each data point
		int symbolSize() const;
		void setSymbolSize(int symbolSize);

		// Get/set the symbols for the different data series
		SymbolType symbol0() const;
		void setSymbol0(SymbolType symbol0);

		SymbolType symbol1() const;
		void setSymbol1(SymbolType symbol1);

		SymbolType symbol2() const;
		void setSymbol2(SymbolType symbol2);

		SymbolType symbol3() const;
		void setSymbol3(SymbolType symbol3);

		SymbolType symbol4() const;
		void setSymbol4(SymbolType symbol4);

		SymbolType symbol5() const;
		void setSymbol5(SymbolType symbol5);

		SymbolType symbol6() const;
		void setSymbol6(SymbolType symbol6);

		SymbolType symbol7() const;
		void setSymbol7(SymbolType symbol7);

		// Add data points to the individual data series
		void addData(int dataSeries, double x, double y);
		void addData(const QString &label, double x, double y);

		// Clear the data of an entire data series
		void clearData(int dataSeries);
		void clearData(const QString &label);

		// Clear the whole chart
		void clearAllData();

	protected:
		virtual void resetValues();
		virtual void generateRandomValues() = 0;

		void paintEvent(QPaintEvent *);
		virtual void paint(QPainter &painter);

		QPointF positionForValue(const std::pair<double, double> &value) const;

		elements::Axis m_xAxis;
		elements::Axis m_yAxis;

		double m_xMin;
		double m_xMax;
		int m_xPrecision;

		double m_yMin;
		double m_yMax;
		int m_yPrecision;

		double m_visibleXMin;
		double m_visibleXMax;
		double m_visibleYMin;
		double m_visibleYMax;

		std::shared_ptr<QStringList> m_labels;

		QRect m_chartRect;

		QFont m_font;
		QFontMetrics m_fontMetrics;

		int m_symbolSize;
		std::shared_ptr<QList<int>> m_symbols;

		elements::Key m_key;

		using Entry = std::pair<double, double>;
		using EntryMultiSet = std::multiset<Entry, bool (*)(const Entry &, const Entry &)>;

		std::vector<EntryMultiSet> m_values;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
