
For the header file ...

class ControlGroup : public QWidget
{
	Q_OBJECT
public:
	explicit ControlGroup( Twidget *tw, QWidget *parent=0 )
	{
		init(tw, parent);
	}
	ControlGroup(){};
	~ControlGroup(){};
	QList<QRadioButton*> widgetList;
	void init( Twidget *tw, QWidget *parent);
	Twidget* getTwidget() {return m_tw;}

private:
	Twidget *m_tw;
	QWidget *m_parent;

};


#ifndef CONTROLGROUP_CPP
#define CONTROLGROUP_CPP

#include "controlgroup.h"

////template <typename T>
////void ControlGroup<T>::init( Twidget *tw, QWidget *parent )
void ControlGroup::init( Twidget *tw, QWidget *parent )
{
	m_tw = tw;
	m_parent = parent;


	// See if the caller wants a QButtonGroup. That would make these buttons
	// mutually exclusive.
	// The QButtonGroup emits a signal with the ID of the checked button for
	// each button that is checked in an exclusive group.
	//
	if(tw->grouped)
		tw->buttonGroup = new QButtonGroup(parent);
	else
		tw->mapper = new QSignalMapper(parent);


	for (int index = 0; index < tw->objCount; index++)
	{
		////T *object = new T(parent);
		QRadioButton *object = new QRadioButton(parent);

		// Layout the objects according to the Table Configuration given by
		// the "topology" field in the Twidget struct.
		//
		int x = tw->geometry.x() + ((index % tw->topology.x()) * tw->geometry.width())
								 + ((index % tw->topology.x()) * tw->geometry.height());
		int y = tw->geometry.y() + ((index % tw->topology.y()) * tw->geometry.height())
								 + ((index % tw->topology.x()) * tw->geometry.width());

		////widgetList.append((T *)object);
		widgetList.append((QRadioButton *)object);
		object->setObjectName(QString(tw->objName % QString::number(index)));
		object->setText(QString(tw->objText[index]));
		object->setGeometry(x, y, tw->geometry.width(), tw->geometry.height());

		// If the buttons are grouped, the buttonGroup will notify the mapper.
		// In this case, the mapper only needs to be programmed at the end of
		// this loop.
		// If they are not grouped, the buttons themselves must notify the
		// mapper, and the mapper must be programmed to recognize each button
		// in the group.
		//
		if(tw->grouped)
			tw->buttonGroup->addButton(object, index);
		else
		{
			connect(object, SIGNAL(clicked()), tw->mapper, SLOT(map()));
			tw->mapper->setMapping(object, index);
		}

		//tw->labelList.append(new QLabel(parent));

	}

}



#endif
