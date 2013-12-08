#ifndef FORMATGROUP_H
#define FORMATGROUP_H

#include <QtGlobal>

#if QT_VERSION < 0x50000
#include <QtGui/QWidget>
#endif

#if QT_VERSION >= 0x50000
#include <QWidget>
#endif

class FormatGroup : public QWidget
{
    Q_OBJECT
public:
    explicit FormatGroup(QWidget *parent = 0);

signals:

public slots:

};

#endif // FORMATGROUP_H
