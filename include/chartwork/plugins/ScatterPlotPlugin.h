#ifndef __PLUGINS__SCATTER_PLOT_PLUGIN_H
#define __PLUGINS__SCATTER_PLOT_PLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ScatterPlotPlugin
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ScatterPlotPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

	public:
		ScatterPlotPlugin(QObject *parent = nullptr);

		bool isContainer() const;
		bool isInitialized() const;
		QIcon icon() const;
		QString domXml() const;
		QString group() const;
		QString includeFile() const;
		QString name() const;
		QString toolTip() const;
		QString whatsThis() const;
		QWidget *createWidget(QWidget *parent);
		void initialize(QDesignerFormEditorInterface *core);

	private:
		bool m_isInitialized;
};

#endif
