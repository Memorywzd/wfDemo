#ifndef FLOWNODE_H
#define FLOWNODE_H

//#include "flowuiarea.h"

#include <QWidget>
#include <QScrollArea>
#include <QDebug>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QDialog>

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>

#include "ui_flownode.h"


namespace Ui {
class FlowNode;
}


class FlowNode : public QWidget
{
    Q_OBJECT

public:
    explicit FlowNode(QWidget *parent = 0);
    FlowNode(QString name, QWidget *parent = 0, bool format = 0, int lid = 0, int lseq = 0);
    FlowNode(FlowNode &&node);
    ~FlowNode();

    static void setDraingNode(FlowNode *node);
    void setUpConfig();
	void setLdap(QString ldap);
    void setName(QString str);
    void setItem(QTreeWidgetItem *item = nullptr);
    QString  getName();
    QString getLdap();
    QTreeWidgetItem * getItem();

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
    void dragMoveEvent(QDragMoveEvent *ev);
    void dragLeaveEvent(QDragLeaveEvent *ev);
    void dropEvent(QDropEvent *ev);

    void onRightButtonClicked();

signals:
    void rightButtonClicked();
    void lineStart(FlowNode *startNode);
    void lineEnd(FlowNode *endNode);
    void requestForDeleteNode(FlowNode *endNode);

	void registerNode(FlowNode* node);

public slots:
	void onEditNode();
    void onDrawLine();
    void onMoreMsg();
    void onDelNode();
    void ondelLine();
    void onPermitDeleteNode(FlowNode *node);

public:
    static QPoint dragStartCursorPos;
    static QPoint dragStartWidgetPos;
    static FlowNode *dragingNode;

public:
    int lid   {-1};      // layer  id after sorted
    int lseq  {0};
    int rid   {-1};      // repeat id after sorted
    int seq   {-1};      // sequence after sorted

public:
    int outDgr  {0};
    int inDgr   {0};
    QPoint centerPos;

private:
    int oid     {-1};
    int ouid    {-1};
    int uid     {-1};
    QString ldap;
    Ui::FlowNode *ui;
    QTreeWidgetItem *selfItem = nullptr;
};


#endif // FLOWNODE_H
