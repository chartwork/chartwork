#include <chartwork/plugins/ChartworkPluginCollection.h>

#include <chartwork/plugins/GroupedBarChartPlugin.h>
#include <chartwork/plugins/StackedBarChartPlugin.h>
#include <chartwork/plugins/HistogramPlugin.h>
#include <chartwork/plugins/GridChartPlugin.h>
#include <chartwork/plugins/PieChartPlugin.h>
#include <chartwork/plugins/ScatterPlotPlugin.h>
#include <chartwork/plugins/LinePlotPlugin.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChartworkPluginCollection
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ChartworkPluginCollection::ChartworkPluginCollection(QObject *parent)
:	QObject(parent)
{
	m_plugins.push_back(new PieChartPlugin(this));
	m_plugins.push_back(new GridChartPlugin(this));
	m_plugins.push_back(new GroupedBarChartPlugin(this));
	m_plugins.push_back(new StackedBarChartPlugin(this));
	m_plugins.push_back(new HistogramPlugin(this));
	m_plugins.push_back(new ScatterPlotPlugin(this));
	m_plugins.push_back(new LinePlotPlugin(this));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QList<QDesignerCustomWidgetInterface *> ChartworkPluginCollection::customWidgets() const
{
	return m_plugins;
}
