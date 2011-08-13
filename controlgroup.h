#ifndef CONTROLGROUP_H
#define CONTROLGROUP_H

#include <QtCore>
#include <QtGui>
#include <QtGui/QWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QButtonGroup>

enum signal_t {st_clicked, st_pressed, st_released, st_activated};

typedef	void (*pSlot_t)(int);

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
** objText		- QList of strings to be displayed on each object.
** sizes		- QList of QSize gives height & width of each object in pixels.
**				  A single-item list means they are all the same size
** layout		- QList of QPoint for the location of each object in pixels.
** labelNames	- *char[] array of strings to be displayed on each label.
**				- An empty list indicates no labels.
** grouped		- if true, button clicked will be mutually exclusive
** labelText	- *char[] array of strings for each label
** labelSizes	- QList of QSize for height and width of each label
** labelLayout	- QList of QPoint for the label layout
** slotList		- QList of function pointers for the caller's slots
**				  (Not implemented yet)
**
** These fields will be initialized by the ControlGroup.
**
** widgetlist	- QList of pointers to the objects created when by invoking
**				  an instance of the ControlGroup class template. Only the
**				  caller knows the type of the objects stored in this list
**				  though they must have QWidget as a base class.
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
	QString objName;			// stem object name of widget
	QList <QString> objText;	// text displayed on objects,
	QList <QSize> sizes;		// A list of sizes for the objects
	QList <QPoint> layout;		// A list of locations for the objects
	bool grouped;				// group these in a QButtonGroup
	QList <QString> labelText;	// Text displayed labels
	QList <QSize> labelSizes;	// list of sizes for the labels
	QList <QPoint> labelLayout;	// A list of locations for the objects
	QList <pSlot_t> slotList;	// list of the caller's slot function pointers
	//
	// These fields will be initialized by ControlGroup
	//
	QList <QLabel> *labelList;	// List of QLabel objects
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
	Twidget* getTwidget() {return m_tw;}

private:
	Twidget *m_tw;
	QWidget *m_parent;

};

template <typename T>
void ControlGroup<T>::init( Twidget *tw, QWidget *parent )
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

	int objCount = tw->layout.size();

	for (int index = 0; index < objCount; index++)
	{
		T *object = new T(parent);
		widgetList.append((T *)object);

		// Layout the objects according to the Table Configuration given by
		// the "topology" field in the Twidget struct.
		//
		object->setObjectName(QString(tw->objName % QString::number(index)));
		object->setText(tw->objText[index]);

		int x = tw->layout[index].x();
		int y = tw->layout[index].y();

		// If the caller only passes one size, then one size fits all.
		// Be sure that we can only index to the zeroth element of the
		// tw->sizes QList. When there is only one item in there, trying
		// to index beyond that throws an access violation, as well it
		// should.
		//
		int sizeIndex = (tw->sizes.size() == 1) ? 0 : index;
		int width = tw->sizes[sizeIndex].width();
		int height = tw->sizes[sizeIndex].height();

		object->setGeometry(x, y, width, height);

		// If the buttons are grouped, the buttonGroup will send signals.
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

#endif // CONTROLGROUP_H
