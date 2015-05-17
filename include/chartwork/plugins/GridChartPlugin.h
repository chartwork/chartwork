#ifndef __PLUGINS__GRID_CHART_PLUGIN_H
#define __PLUGINS__GRID_CHART_PLUGIN_H

#include <QDesignerCustomWidgetInterface>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GridChartPlugin
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class GridChartPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

	public:
		GridChartPlugin(QObject *parent = nullptr);

		virtual bool isContainer() const;
		virtual bool isInitialized() const;
		virtual QIcon icon() const;
		virtual QString domXml() const;
		virtual QString group() const;
		virtual QString includeFile() const;
		virtual QString name() const;
		virtual QString toolTip() const;
		virtual QString whatsThis() const;
		virtual QWidget *createWidget(QWidget *parent);
		virtual void initialize(QDesignerFormEditorInterface *core);

	private:
		bool m_isInitialized;
};

#endif
