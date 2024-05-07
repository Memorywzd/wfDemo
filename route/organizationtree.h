#ifndef ORGANIZATIONTREE_H
#define ORGANIZATIONTREE_H

#include <QWidget>
#include <QTreeWidget>

#include <QObject>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QMessageBox>
#include <QMouseEvent>

#include <stack>
#include <functional>

using namespace std;

class OrganizationTree : public QTreeWidget
{
    Q_OBJECT

public:
    OrganizationTree(QWidget *parent = 0);
    void setUpConfig();
    void setUpTree();
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void leftButtonClicked(const QPoint &pos = QPoint(0,0));
    void rightButtonClicked(const QPoint &pos = QPoint(0,0));
    void addNodeToUiArea();

public slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onMoreMsg();
    void onAddNode();

public:
        static QTreeWidgetItem *activeItem;
};

#endif // ORGANIZATIONTREE_H
