#ifndef __SCATTER_PLOT_H
#define __SCATTER_PLOT_H

#include <QtUiPlugin/QDesignerExportWidget>

#include <chartwork/XYPlot.h>
#include <chartwork/elements/Key.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ScatterPlot
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT ScatterPlot : public XYPlot
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

	Q_ENUMS(SymbolType)

	Q_PROPERTY(int symbolSize READ symbolSize WRITE setSymbolSize)

	Q_PROPERTY(QColor color0 READ color0 WRITE setColor0)
	Q_PROPERTY(SymbolType symbol0 READ symbol0 WRITE setSymbol0)
	Q_PROPERTY(QColor color1 READ color1 WRITE setColor1)
	Q_PROPERTY(SymbolType symbol1 READ symbol1 WRITE setSymbol1)
	Q_PROPERTY(QColor color2 READ color2 WRITE setColor2)
	Q_PROPERTY(SymbolType symbol2 READ symbol2 WRITE setSymbol2)
	Q_PROPERTY(QColor color3 READ color3 WRITE setColor3)
	Q_PROPERTY(SymbolType symbol3 READ symbol3 WRITE setSymbol3)
	Q_PROPERTY(QColor color4 READ color4 WRITE setColor4)
	Q_PROPERTY(SymbolType symbol4 READ symbol4 WRITE setSymbol4)
	Q_PROPERTY(QColor color5 READ color5 WRITE setColor5)
	Q_PROPERTY(SymbolType symbol5 READ symbol5 WRITE setSymbol5)
	Q_PROPERTY(QColor color6 READ color6 WRITE setColor6)
	Q_PROPERTY(SymbolType symbol6 READ symbol6 WRITE setSymbol6)
	Q_PROPERTY(QColor color7 READ color7 WRITE setColor7)
	Q_PROPERTY(SymbolType symbol7 READ symbol7 WRITE setSymbol7)

	public:
		ScatterPlot(QWidget *parent = 0);

	private:
		void generateRandomValues();

		void paint(QPainter &painter);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
