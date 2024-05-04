#ifndef ORGTREE_H
#define ORGTREE_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QLabel>

#include <stack>
#include <functional>

#include "widget.h"
#include "ui_widget.h"

namespace Ui {
    class OrgTree;
}

class OrgTree : public QObject
{
public:
    OrgTree();
    void generate(Ui::Widget *ui);

public slots:
    void onCustomContextMenuRequested(const QPoint &pos);

};

#endif // ORGTREE_H
