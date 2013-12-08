#ifndef BITBUTTON_H
#define BITBUTTON_H

#include <QtGlobal>

#if QT_VERSION < 0x50000
#include <QtGui/QFrame>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#endif

#if QT_VERSION >= 0x50000
#include <QFrame>
#include <QPushButton>
#include <QWidget>
#endif

//#include <QtDesigner/QDesignerCustomWidgetInterface>

class BitButton : public QPushButton
{
    Q_OBJECT
public:
    BitButton(int number, QWidget *parent);
    void bbToggle();

    int bbId;
    int bbState;
    void setState(int state);

signals:
    void bbClicked();

private slots:

private:
    QString bbStyle[2];
    QFrame *bbFrame;
    QColor foreground;
    QColor background;
    QColor highlight;
    QColor HighText;
    QList <QColor> bbColors;
};


#endif
