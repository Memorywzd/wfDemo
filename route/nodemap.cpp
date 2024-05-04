#include "nodemap.h"

NodeMap::NodeMap(QObject * parent) :
    QObject(parent) {
    qDebug() << size();
}


int NodeMap::size() {
    return vec.size();
}


/* 插入具体路径 */
int NodeMap::insertRoute(FlowNode * startNode, FlowNode * endNode, QString funcName, WfLine::function funcId){

    if(startNode == endNode)    return 0;

    if(startNode && endNode) {
        qDebug() << funcName << ": " << funcId;
        WfLine* route = new WfLine(startNode, endNode, this);
        route->funcName = funcName;
        route->funcId = funcId;
        qDebug() << "NodeMap::insertRoute ---- " << startNode->getName() << "-->" << endNode->getName()
                 << "with function_name = " << funcName << " -- " << funcId;
        routesMap.append(route);
    }

    for(auto it = vec.begin(); it != vec.end(); it ++) {
        if( it->first == startNode ) {
            for( auto node : it->second ) {
                if( node == endNode ) return 0;
            }
            it->second.append(endNode);
            return 1;
        }
    }

    vec.append(QPair<FlowNode *, QList<FlowNode *>>(startNode, QList<FlowNode *>()));
    vec.last().second.append(endNode);
    return 1;
}

/* 删除包含某一节点的任意方向路径 */
int NodeMap::deleteRoute(FlowNode * anyNode){
    int delCnt = 0;
    auto it = vec.begin();

    while( it != vec.end() ) {
        //检查终点
        QList<FlowNode *> & endNodeList = it->second;
        for(auto it_node = endNodeList.begin(); it_node != endNodeList.end(); it_node++) {
            if(*it_node == anyNode) {
                endNodeList.erase(it_node);
                delCnt++;
                break;
            }
        }

        // 检查起点
        if( it->first == anyNode) {
            it = vec.erase(it);
            delCnt ++;
        } else {  // 注意在循环中使用迭代器失效的问题。
            it ++;
        }
    }

    for(auto route = routesMap.begin(); route != routesMap.end();)
    {
        if( (*route)->startNode == anyNode ||
            (*route)->endNode == anyNode )
        {
            route = routesMap.erase(route);
        }
        else route ++;
    }

    return delCnt;

}

/* 删除具体路径 */
int NodeMap::deleteRoute(FlowNode * startNode, FlowNode * endNode){
    auto deleteInRoutesMap = [&]()->int{
        for(auto route = routesMap.begin(); route != routesMap.end(); route ++)
        {
            if( (*route)->startNode == startNode &&
                (*route)->endNode == endNode )
            {
                routesMap.erase(route);
                startNode->outDgr -= 1;
                endNode->inDgr -= 1;
                return 1;
            }
        }
        return 0;
    };

    for(auto it = vec.begin(); it != vec.end(); it ++) {
        if( it->first == startNode) {
            for( auto endNodeItem = it->second.begin(); endNodeItem != it->second.end(); endNodeItem ++ ) {
                if( *endNodeItem == endNode ) {
                    it->second.erase(endNodeItem);
                    return deleteInRoutesMap();
                }
            }
        }
    }
    return 0;
}

QList<WfLine *>::Iterator NodeMap::ClickAtWfLine(const QPointF& clickPos)
{
    auto HitLine = [](const QPointF& start, const QPointF& end, const QPointF& target) -> bool {
        qreal x1 = start.x();
        qreal y1 = start.y();
        qreal x2 = end.x();
        qreal y2 = end.y();
        qreal xt = target.x();
        qreal yt = target.y();
        qreal k = (y2 - y1) / (x2 - x1);
        qreal b = y1 - k * x1;
        qreal yt1 = k * xt + b;
        qreal minx = qMin(x1, x2) - 10;
        qreal maxx = qMax(x1, x2) + 10;
        qreal miny = qMin(y1, y2) - 10;
        qreal maxy = qMax(y1, y2) + 10;
//        qDebug() << " ---------------- ";
//        qDebug() <<"x1 = " << x1 << " -- y1 = " << y1;
//        qDebug() <<"x2 = " << x2 << " -- y2 = " << y2;

//        qDebug() <<"k = " << k << " -- b = " << b;

//        qDebug() <<"xt = " << xt << " -- yt = " << yt;
//        qDebug() << "yt1 = " << yt1;

        if (xt >= minx && xt <= maxx && yt >= miny && yt <= maxy && qAbs(yt - yt1) < 12) {
            return true;
        }
        return false;
    };

    for(auto route = routesMap.begin(); route != routesMap.end(); route ++)
    {
        if(HitLine((*route)->startNode->centerPos,
                   (*route)->endNode->centerPos,
                   clickPos))
        {
            qDebug() << "HitLine = true" ;
            return route;
        }
    }
    qDebug() << "HitLine = false" ;
    return routesMap.end();
}

/* 用起始点查询有哪些结束点 */
void NodeMap::queryEndOfStartNode(FlowNode * startNode, QList<FlowNode *> &endClt) {
    endClt.clear();
    for( auto it : vec ) {
        if( it.first == startNode ) {
            endClt = it.second;
        }
    }
}

/* 用结束点查询有哪些起始点 */
void NodeMap::queryStartOfEndNode(FlowNode * endNode, QList<FlowNode *> &startClt) {
    startClt.clear();
    for( auto it = vec.begin(); it != vec.end(); it ++ ) {
        QList<FlowNode *> & endNodeList = it->second;
        for(auto it_node = endNodeList.begin(); it_node != endNodeList.end(); it_node++) {
            if(*it_node == endNode) {
                startClt.append(it->first);
                break;
            }
        }
    }
}

/* 获取所有的线段始末节点 */
void NodeMap::queryAllRoutes( QList<QPair<FlowNode *, FlowNode *>> &nodeRoutes) {
    if(this->vec.empty()) return;

    for( auto it : vec ) {
        FlowNode* & startNode = it.first;
        QList<FlowNode *> & endNodeList = it.second;
        if(endNodeList.empty()) return;
        for(auto endNode : endNodeList) {
            nodeRoutes.append(QPair<FlowNode *, FlowNode *>(startNode, endNode));
        }
    }

    qDebug("----------- check routesMap ------------ ");
    for(auto it = routesMap.begin(); it != routesMap.end(); it ++)
    {
        qDebug() << (*it)->startNode->getName() << " ---> " << (*it)->endNode->getName();
    }

}


/* 获取所有的线段始末点位置 */
void NodeMap::queryAllRoutes( QList<QPair<QPointF, QPointF>> &posRoutes) {
    if(this->vec.empty()) return;

    for( auto it : vec ) {
        FlowNode* & startNode = it.first;
        QList<FlowNode *> & endNodeList = it.second;
        if(endNodeList.empty()) return;
        for(auto endNode : endNodeList) {
        posRoutes.append(QPair<QPointF, QPointF>(
                              QPoint(startNode->x() + startNode->width()/2, startNode->y() + startNode->height()/2),
                              QPoint(endNode->x() + endNode->width()/2, endNode->y() + endNode->height()/2)
                              ));
        }
    }

    qDebug("----------- check routesMap ------------ ");
    for(auto it = routesMap.begin(); it != routesMap.end(); it ++)
    {
        qDebug() << (*it)->startNode->getName() << " ---> " << (*it)->endNode->getName();
    }
}

///* 查看所有路径，用于debug */
void NodeMap::showRoute__debug() {

    qDebug() << " ------------- 所有路径 -------------" << endl;
    for( auto it : vec ) {
        FlowNode* & startNode = it.first;
        QList<FlowNode *> & endNodeList = it.second;

        qDebug() << " route start from 【 " << startNode->getName() << " 】";
        for(auto endNode : endNodeList) {
            qDebug()<< "            to 【 " << endNode->getName() << " 】";
        }
    }


}

