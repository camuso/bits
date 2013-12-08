#ifndef SHIFTOP_H
#define SHIFTOP_H

//#include <QtCore>
//#include <QtGui>

#include <QtGlobal>

#if QT_VERSION < 0x50000
#include <QtGui/QCheckBox>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#endif

#if QT_VERSION >= 0x50000
#include <QCheckBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#endif

#include "controlgroup.h"
#include <QtCore/QStringBuilder>
#include <QtDesigner/QDesignerCustomWidgetInterface>

/*////////////////////////////////////////////////////////////////////////////
*/
class ShiftOpGroup : public QWidget
{
    Q_OBJECT
public:
    explicit ShiftOpGroup(QPoint *start, QWidget *parent = 0);

    QLineEdit *shiftBox;
    ControlGroup <QPushButton> *pShiftButtons;
    QCheckBox *chkRotate;

    int getCurrentShiftVal();
    void setCurrentShiftVal(int val);

signals:
    void shift(int);

public slots:
    void onTextEdited(QString text);

private:
    int currentShiftVal;
    Twidget *tw;            // Twidget for "<<" and ">>" PushButtons
};



#endif // SHIFTOP_H
