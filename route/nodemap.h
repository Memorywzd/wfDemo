#ifndef NODEMAP_H
#define NODEMAP_H

#include <QObject>
#include <QPair>
#include <QList>

#include "flownode.h"
#include "wfline.h"


class NodeMap : public QObject
{
    Q_OBJECT

public:
    NodeMap(QObject * parent = 0);
    int size();
    int insertRoute(FlowNode * startNode,
                    FlowNode * endNode = 0,
                    QString funcName = "",
                    WfLine::function funcId = WfLine::function::UNINITIALIZED);
    int deleteRoute(FlowNode * anyNode);
    int deleteRoute(FlowNode * startNode, FlowNode * endNode);
    QList<WfLine *>& queryRoutesMap(){ return routesMap; }
    QList<WfLine *>::Iterator ClickAtWfLine(const QPointF& clickPos);
    QList<WfLine*>::iterator nops() {return routesMap.end();}
    void queryEndOfStartNode(FlowNode * startNode, QList<FlowNode *> &endClt);
    void queryStartOfEndNode(FlowNode * endNode, QList<FlowNode *> &startClt);
    void queryAllRoutes( QList<QPair<FlowNode *, FlowNode *>> &nodeRoutes);
    void queryAllRoutes( QList<QPair<QPointF, QPointF>> &posRoutes);

    QList<WfLine *>& queryAllRoutes()
    {
        return this->routesMap;
    }

    void clear()
    {
        vec.clear();
        routesMap.clear();
        emit DeleteAllNodes();
    }

    void showRoute__debug();

signals:
    void DeleteAllNodes();

private:
    QList<QPair<FlowNode *, QList<FlowNode *>>> vec;
    QList<WfLine *> routesMap;

};

#endif // NODEMAP_H
