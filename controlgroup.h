#ifndef CONTROLGROUP_H
#define CONTROLGROUP_H

#include <QtCore>
#include <QtGui>
#include <QtGui/QWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QButtonGroup>

enum direction_t { go_left, go_right, go_up,  go_down };
typedef void (*pSlot_t)(int);

/*
** class Twidget
**
** This class is only used by instances of the ControlGroup template class
** for creation and management of the QWidget based objects that it creates.
**
** Because we don't know the type of the objects that will be created by
** ControlGroup, we use a class template and pass the object type to be
** expanded by the compliler when the template is invoked.
**
** The QList of Objects will be maintained by each instance of ControlGroup
** for the object types that it creates.
**
** These objects must all have QWidget as their base class, since ControlGroup
** will operate on member functions from that class.
**
** The following fields must be initialized by the caller.
**
** objName		- stem object name. The index of each object in the list will
**				  be added to the name for each object
** objCount		- the number of objects to be created
** objText		- text to be displayed on each object. If each object has a
**				  different text, then the text is separated with "$$". An
**				  empty string indicates no text for display.
** labelNames	- Text to be displayed on labels, delimited by "$$". If this
**				  string is empty, then there are no labels to create.
** geometry		- x and y coordinates of the first object, as well as its width
**				  and height, in a QRect object (x, y, w, h);
** topology		- number of columns, number of rows, column width, row height
** increment	- the number of pixels between each object
** direction	- which way the objects are layed out
** labelGeom	- geometry of the labels. They will be layed out in the same
**				  direction as the objects.
** grouped		- whether these buttons will have a QButtonGroup
**
** These fields will be initialized by the ControlGroup.
**
** widgetlist	- pointer to the widgetlist created when the caller invoked
**				  an instance of the ControlGroup class template. Only the
**				  caller knows the type of the objects stored in this list
**				  though they must have QWidget as a base class.
** labelList	- List of QLabel objects
** slotList		- list of slot function pointers
** mapper		- The signal mapper for these objects
** butonGroup	- The place where these objects will be linked
**				- : together as a group.
**
*/
class Twidget
{
public:
	// These fields are must be initialized by the caller to ControlGroup
	//
	QString objName;		// stem object name of widget
	int objCount;			// Number of widgets
	const char** objText;	// An array of text displayed in objects,
	const char** labelText;	// Text displayed labels, delimited by "$$"
	QRect geometry;			// starting x,y coordinates and size in pixels
	QRect topology;			// x = cols, y = rows, w = col width, h = row height
	int increment;			// .. by how many pixels
	direction_t direction;	// go_left or go_right
	QRect labelGeom;		// geometry of the labels, if any
	bool grouped;			// whether these buttons will have a QButtonGroup
	//
	// These fields will be initialized by ControlGroup
	//
	//QList *widgets;				// Pointer to the widget list
	QList <QLabel> *labelList;	// List of QLabel objects
	QList <pSlot_t> slotList;	// list of slot function pointers
	QSignalMapper *mapper;		// The signal mapper for these objects
	QButtonGroup *buttonGroup;	// The place where these objects will be linked
								// : together as a group.
};

template <typename T>
class ControlGroup : public QWidget
{
public:
	explicit ControlGroup( Twidget *tw, QWidget *parent=0 )
	{
		init(tw, parent);
	}
	ControlGroup(){};
	~ControlGroup(){};
	QList<T*> widgetList;
	void init( Twidget *tw, QWidget *parent);

private:
	Twidget *m_tw;
	QWidget *m_parent;

};

template <typename T>
void ControlGroup<T>::init( Twidget *tw, QWidget *parent )
{
	m_tw = tw;
	m_parent = parent;

	tw->mapper = new QSignalMapper(parent);

	// See if the caller wants a QButtonGroup. That would make these buttons
	// mutually exclusive.
	//
	if(tw->grouped)
		tw->buttonGroup = new QButtonGroup(parent);

	for (int index = 0; index < tw->objCount; index++)
	{
		T *object = new T(parent);

		// Layout the objects according to the Table Configuration given by
		// the "topology" field in the Twidget struct.
		//
		int x = tw->geometry.x() + ((index % tw->topology.x()) * tw->geometry.width())
								 + ((index % tw->topology.x()) * tw->geometry.height());
		int y = tw->geometry.y() + ((index % tw->topology.y()) * tw->geometry.height())
								 + ((index % tw->topology.x()) * tw->geometry.width());

		widgetList.append((T *)object);
		object->setObjectName(QString(tw->objName % QString::number(index)));
		object->setText(QString(tw->objText[index]));
		object->setGeometry(x, y, tw->geometry.width(), tw->geometry.height());

		tw->mapper->setMapping(object, index);
		connect(object, SIGNAL(activated(int)), tw->mapper, SLOT(map()));

		if(tw->grouped)
			tw->buttonGroup->addButton(object, index);

		//tw->labelList.append(new QLabel(parent));

	}
}


#endif // CONTROLGROUP_H
