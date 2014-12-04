/*
  Copyright: LaBRI / SCRIME

  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software.  You can  use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and,  more generally, to use and operate it in the
  same conditions as regards security.

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#include "AutomationView.hpp"
#include "../TimeBox/TimeBoxStoreyBar.hpp"
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPainter>

AutomationView::AutomationView (QGraphicsObject* parent)
	: PluginView (parent), _points()
{
	QPainterPath path (QPointF (0, parentItem()->boundingRect().height() - TimeboxStoreyBar::HEIGHT) );
	path.lineTo (parentItem()->boundingRect().width() - 2, 0);
	_pLine = new QGraphicsPathItem (this);
	_pLine->setPath (path);
}


QPainterPath AutomationView::shape() const
{
	return _pLine->shape();
}

void AutomationView::paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED (option)
	Q_UNUSED (widget)

	painter->setRenderHint (QPainter::Antialiasing, true);
	_pLine->paint (painter, option, widget);
}

QRectF AutomationView::boundingRect() const
{
	return _pLine->boundingRect();
}
