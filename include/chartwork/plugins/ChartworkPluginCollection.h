#ifndef __PLUGINS__CHARTWORK_PLUGIN_COLLECTION_H
#define __PLUGINS__CHARTWORK_PLUGIN_COLLECTION_H

#include <QDesignerCustomWidgetCollectionInterface>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChartworkPluginCollection
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ChartworkPluginCollection : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.chartwork.ChartworkCollection")
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

	public:
		ChartworkPluginCollection(QObject *parent = nullptr);

		QList<QDesignerCustomWidgetInterface *> customWidgets() const;

	private:
		QList<QDesignerCustomWidgetInterface *> m_plugins;
};

#endif
