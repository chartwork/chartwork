#include <chartwork/plugins/GridChartPlugin.h>

#include <QtPlugin>

#include <chartwork/GridChart.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GridChartPlugin
//
////////////////////////////////////////////////////////////////////////////////////////////////////

GridChartPlugin::GridChartPlugin(QObject *parent)
:	QObject(parent)
{
	m_isInitialized = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GridChartPlugin::initialize(QDesignerFormEditorInterface *)
{
	if (m_isInitialized)
		return;

	m_isInitialized = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool GridChartPlugin::isInitialized() const
{
	return m_isInitialized;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QWidget *GridChartPlugin::createWidget(QWidget *parent)
{
	return new chartwork::GridChart(parent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString GridChartPlugin::name() const
{
	return "chartwork::GridChart";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString GridChartPlugin::group() const
{
	return "Chartwork";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QIcon GridChartPlugin::icon() const
{
	return QIcon("://icons/icon-grid-chart.png");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString GridChartPlugin::toolTip() const
{
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString GridChartPlugin::whatsThis() const
{
	return "Grid Chart";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool GridChartPlugin::isContainer() const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString GridChartPlugin::domXml() const
{
	return QLatin1String
		("<ui language=\"c++\" displayname=\"GridChart\">\
			<widget class=\"chartwork::GridChart\" name=\"gridChart\">\
				<property name=\"geometry\">\
					<rect>\
						<x>0</x>\
						<y>0</y>\
						<width>400</width>\
						<height>300</height>\
					</rect>\
				</property>\
			</widget>\
		</ui>");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString	GridChartPlugin::includeFile() const
{
	return QLatin1String("<chartwork/GridChart.h>");
}