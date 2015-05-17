#ifndef __DESIGN_H
#define __DESIGN_H

#include <QColor>
#include <QFont>

namespace chartwork
{
namespace design
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Design
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const int keySpacing = 8;
const int keyItemSize = 16;
const int spacing = 16;
const int titleHeight = 24;
const int markWidth = 6;
const int axisSpacing = 8;

// Colors
const QColor darkergrey, darkergray			= QColor(46, 52, 54);
const QColor darkgrey, darkgray				= QColor(85, 87, 83);
const QColor grey, gray						= QColor(136, 138, 133);
const QColor lightgrey, lightgray			= QColor(186, 189, 182);
const QColor lightergrey, lightergray		= QColor(211, 215, 207);
const QColor superlightgrey, superlightgray = QColor(238, 238, 236);

const QColor darkred						= QColor(146, 0, 0);
const QColor red							= QColor(204, 0, 0);
const QColor lightred						= QColor(239, 41, 41);

const QColor darkbrown						= QColor(143, 89, 2);
const QColor brown							= QColor(193, 125, 17);
const QColor lightbrown						= QColor(233, 185, 110);

const QColor darkpurple						= QColor(92, 53, 102);
const QColor purple							= QColor(117, 80, 123);
const QColor lightpurple					= QColor(173, 127, 168);

const QColor darkblue						= QColor(32, 74, 135);
const QColor blue							= QColor(52, 101, 164);
const QColor lightblue						= QColor(114, 159, 207);

const QColor darkorange						= QColor(206, 92, 0);
const QColor orange							= QColor(245, 121, 0);
const QColor lightorange					= QColor(252, 175, 62);

const QColor darkgreen						= QColor(78, 154, 6);
const QColor green							= QColor(115, 210, 22);
const QColor lightgreen						= QColor(138, 226, 52);

const QColor darkyellow						= QColor(169, 160, 0);
const QColor yellow							= QColor(237, 212, 0);
const QColor lightyellow					= QColor(252, 233, 79);

inline QColor colorPalette(int index)
{
	switch (index % 6)
	{
		default:
			return blue;
		case 1:
			return orange;
		case 2:
			return green;
		case 3:
			return purple;
		case 4:
			return red;
		case 5:
			return yellow;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
