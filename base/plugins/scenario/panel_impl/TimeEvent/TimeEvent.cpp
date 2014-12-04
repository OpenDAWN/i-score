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

#include "TimeEvent.hpp"
#include "TimeEventModel.hpp"
#include "TimeEventPresenter.hpp"
#include "TimeEventView.hpp"
#include "../TimeBox/TimeBox.hpp"
#include "../TimeBox/TimeBoxFullView.hpp"
#include "../ScenarioContainer/ScenarioContainer.hpp"

int TimeEvent::m_staticId = 1;

TimeEvent::TimeEvent (ScenarioContainer* pParent, const QPointF& pos)
	: QObject (pParent)
{/*
	///If no name was given, we construct a name with a unique ID
	QString name = QString ("TimeEvent%1").arg (m_staticId++);
	setObjectName (name);

	_pModel = new TimeEventModel (pos.x(), pos.y(), name, this);
	_pView = new TimeEventView (this);
	_pPresenter = new TimeEventPresenter (_pModel, _pView, this);
	_pView->setPresenter(_pPresenter);
*/
	/*
	connect (_pView, SIGNAL (createTimeEventAndTimebox (QLineF) ), 
			 this, SIGNAL (createTimeEventAndTimeboxProxy (QLineF) ) );

	if (pParent != nullptr)
	{
		pParent->addChild (this);
		connect (this, SIGNAL (createTimeEventAndTimeboxProxy (QLineF) ), 
				 pParent, SLOT (createTimeEventAndTimebox (QLineF) ) );
	}*/
}

TimeEvent::~TimeEvent()
{
}

///@todo addTimeboxandTimeEvent()

