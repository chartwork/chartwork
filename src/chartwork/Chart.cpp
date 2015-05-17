#include <chartwork/Chart.h>

#include <QMenu>
#include <QFileDialog>

#include <chartwork/elements/Background.h>
#include <chartwork/elements/Title.h>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Chart
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Chart::Chart(QWidget *parent)
:	QWidget(parent),
	m_background(elements::Background(this)),
	m_isUntouched(true)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(showContextMenu(const QPoint&)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const QString &Chart::title() const
{
	return m_title.text();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chart::setTitle(const QString &title)
{
	m_title.setText(title);
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chart::showContextMenu(const QPoint &eventLocation)
{
	const QPoint globalPosition = mapToGlobal(eventLocation);

	QMenu contextMenu;
	contextMenu.addAction("Save As …");

	QAction *selectedItem = contextMenu.exec(globalPosition);

	if (selectedItem)
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "chart.png", tr("*.png;;*.jpg;;*.bmp"));

		const QPixmap image = grab();
		image.save(fileName);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chart::handleColorUpdate()
{
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Chart::handleValueUpdate()
{
	if (!m_isUntouched)
		return;

	m_isUntouched = false;

	resetValues();
	update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
