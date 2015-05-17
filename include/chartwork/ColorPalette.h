#ifndef __COLOR_PALETTE_H
#define __COLOR_PALETTE_H

#include <QtDesigner/QDesignerExportWidget>
#include <QList>
#include <QColor>

#include <memory>

namespace chartwork
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ColorPalette
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class QDESIGNER_WIDGET_EXPORT ColorPalette
{
	public:
		ColorPalette();

		// Get/set the different colors in the color palette
		const QColor &color0() const;
		void setColor0(const QColor &color0);

		const QColor &color1() const;
		void setColor1(const QColor &color1);

		const QColor &color2() const;
		void setColor2(const QColor &color2);

		const QColor &color3() const;
		void setColor3(const QColor &color3);

		const QColor &color4() const;
		void setColor4(const QColor &color4);

		const QColor &color5() const;
		void setColor5(const QColor &color5);

		const QColor &color6() const;
		void setColor6(const QColor &color6);

		const QColor &color7() const;
		void setColor7(const QColor &color7);

	protected:
		virtual void handleColorUpdate() = 0;

		std::shared_ptr<QList<QColor>> m_colors;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
