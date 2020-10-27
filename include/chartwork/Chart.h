#ifndef __CHART_H
#define __CHART_H

#include <QtUiPlugin/QDesignerExportWidget>
#include <QWidget>

#include <chartwork/ColorPalette.h>
#include <chartwork/elements/Title.h>
#include <chartwork/elements/Background.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Chart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT Chart : public QWidget, public ColorPalette
{
	Q_OBJECT

	public:
		Chart(QWidget *parent = 0);

		// Get/set the title of the chart (shown above the chart)
		const QString &title() const;
		void setTitle(const QString &title);

	public slots:
		void showContextMenu(const QPoint&);

	protected:
		virtual void paintEvent(QPaintEvent *event) = 0;
		void handleColorUpdate();

		virtual void handleValueUpdate();
		virtual void resetValues() = 0;

		elements::Background m_background;
		elements::Title m_title;

		bool m_isUntouched;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
