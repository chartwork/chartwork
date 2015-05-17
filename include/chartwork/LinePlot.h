#ifndef __LINE_PLOT_H
#define __LINE_PLOT_H

#include <chartwork/XYPlot.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LinePlot
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT LinePlot : public XYPlot
{
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(bool showKey READ showKey WRITE setShowKey)
	Q_PROPERTY(double keyScale READ keyScale WRITE setKeyScale)

	Q_PROPERTY(double xMin READ xMin WRITE setXMin)
	Q_PROPERTY(double xMax READ xMax WRITE setXMax)
	Q_PROPERTY(int xPrecision READ xPrecision WRITE setXPrecision)

	Q_PROPERTY(double yMin READ yMin WRITE setYMin)
	Q_PROPERTY(double yMax READ yMax WRITE setYMax)
	Q_PROPERTY(int yPrecision READ yPrecision WRITE setYPrecision)

	Q_PROPERTY(QStringList labels READ labels WRITE setLabels)

	Q_PROPERTY(double lineWidth READ lineWidth WRITE setLineWidth)

	Q_PROPERTY(QColor color0 READ color0 WRITE setColor0)
	Q_PROPERTY(QColor color1 READ color1 WRITE setColor1)
	Q_PROPERTY(QColor color2 READ color2 WRITE setColor2)
	Q_PROPERTY(QColor color3 READ color3 WRITE setColor3)
	Q_PROPERTY(QColor color4 READ color4 WRITE setColor4)
	Q_PROPERTY(QColor color5 READ color5 WRITE setColor5)
	Q_PROPERTY(QColor color6 READ color6 WRITE setColor6)
	Q_PROPERTY(QColor color7 READ color7 WRITE setColor7)

	public:
		LinePlot(QWidget *parent = 0);

		// Get/set the width of the lines in the chart
		double lineWidth() const;
		void setLineWidth(double lineWidth);

	private:
		void generateRandomValues();

		void paint(QPainter &painter);

		double m_lineWidth;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
