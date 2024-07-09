#include "verifier.h"

Verifier::Verifier(QObject *parent) :
    QObject(parent)
{

}

QVector<FlowNode*> Verifier::getSortedNodes() const {
	return sortedNodes;
}

/* 选举工作流的起始结点和结束结点 */
int Verifier::electBE( const QList<FlowNode *> & nodesList,
                       FlowNode* & flowBegin,
                       FlowNode* & flowEnd )
{
    for( auto node : nodesList ) {
        if( node->inDgr == 0 ) {
            if( flowBegin != nullptr ) {
                emit verifyFalse("非法！该工作流存在多个起点！");
                return -1;
            }
            flowBegin = node;
        }
        else if( node->outDgr == 0 ) {
            if( flowEnd != nullptr ) {
                emit verifyFalse("非法！该工作流存在多个终点！");
                return -1;
            }
            flowEnd = node;
        }
    }

    if( flowBegin == nullptr )  {
        emit verifyFalse("非法！该工作流缺少起点！");
        return -1;
    }
    if( flowEnd == nullptr ) {
        emit verifyFalse("非法！该工作流缺少终点！");
        return -1;
    }
    return 0;
}

/* 查看工作流结点顺序 */
void Verifier::showSeq() {
    int seq = 0;
    qDebug() << " ---------- sequence ----------";
    qDebug() << "sortedNodes.size() = " << sortedNodes.size();
    for( int i = 0; i < sortedNodes.size(); i ++ ) {
        if(!sortedNodes[i]) qDebug() << "seq = " << i << "  is nullptr";
        qDebug() << "seq[" << i << "] --- " << sortedNodes[i]->getName();
    }
    qDebug() << "done";
}

/* 给工作流的结点排序 */
bool Verifier::sort(NodeMap* & nodeMap, FlowNode* &flowBegin, FlowNode* &flowEnd, int n)
{
    qDebug() << "begin() = " << flowBegin->getName();
    qDebug() << "end() = " << flowEnd->getName();

    this->sortedNodes.resize(n);

    int seq = 0;
    int layerCounter = 0;
    int layerSize = 1;

    /* 记忆化广度优先遍历（层序遍历思想） */
    QMap<FlowNode *, int> visited;
    QMap<FlowNode *, int> stored;
    stored[flowBegin] = 1;

    QList<FlowNode *> endClt;
    QList<FlowNode *> layer;
    QMap<QString, int> repeatCounter;
    layer.push_back(flowBegin);

    // 开始排序
    while( seq < n && !layer.empty() )
    {
        for(int lseq = 0; /*seq < n && */ lseq < layerSize; lseq ++ , seq++)
        {
            qDebug() << "-------------------";
            qDebug() << "seq = " << seq;
            FlowNode * node = layer.front();

            qDebug() << node->getName() << " -- visited = " <<  visited[node] << " -- stored = " << stored[node];
            if( visited[node] == 1 && stored[node] == 1 )
            {
                qDebug() << "死循环";
                emit verifyFalse("存在死循环！");
                return false;
            }
            if( visited[node] == 0 )
            {
                sortedNodes[seq] = node;
                visited[node] = 1;

                /* 记录flowNode中的 seq、lid、rid */
                node->seq  = seq;
                node->lid  = layerCounter;
                node->lseq = lseq;

                qDebug() << node->getName() << " --- " << " seq = " << node->seq
                             << " |  lid = " << node->lid
                             << " |  lseq = " << node->lseq;

                if(repeatCounter.contains(node->getName()))
                {
                    repeatCounter[node->getName()] ++;
                    node->rid = repeatCounter[node->getName()];
                }
                else
                {
                    repeatCounter[node->getName()] = 0;
                    node->rid = 0;
                }

                nodeMap->queryEndOfStartNode(node, endClt);
                for(auto endCltIt : endClt)
                {
                    if(stored[endCltIt] == 0)
                    {
                        layer.append(endCltIt);
                        stored[endCltIt] = 1;
                    }
                }
            }

            layer.pop_front();
        }
        layerSize = layer.size();
        layerCounter ++;
    }

    showSeq();
    qDebug() << "move the flowEnd to the last seq";
    int idx = sortedNodes.indexOf(flowEnd);
    if(idx == -1){
        qDebug() << "flowEnd not found";
        return false;
    }
    // 冒泡，将flowEnd节点挪到最后面去
    for(int i = idx + 1; i < n; i ++)
    {
        sortedNodes[i]->seq -= 1;
        sortedNodes[i-1] = sortedNodes[i];
    }
    sortedNodes[n-1] = flowEnd;
    flowEnd->seq = n-1;
    showSeq();

    return true;
}

/* 建立序号映射 */
void Verifier::seqMapping(){
    for( int i = 0; i < sortedNodes.size(); i ++ ) {
        seqMap.insert(sortedNodes[i], i);
    }
}

/* 展示邻接矩阵 */
void Verifier::showMatrix() {
qDebug() << " ---------- matrix ----------";
    for(auto const &row : matrix){
        QString rowOutput;
        for(int const &col : row){
            rowOutput += QString::number(col) + "\t";
        }
        qDebug().noquote() << rowOutput;
    }
}

/* 初始化工作流图的邻接矩阵 */
void Verifier::generateMatrix(NodeMap * nodeMap, int n) {
    //seqMapping();
    matrix.clear();

    matrix.resize(n);
    for(int i = 0; i < n; i ++) {
        matrix[i].resize(n);
        QList<FlowNode *> endClt;
        nodeMap->queryEndOfStartNode(sortedNodes[i], endClt);
        for( FlowNode * node : endClt ) {
            //int j = seqMap[node];
            int j = node->seq;
            matrix[i][j] = 1;
        }
    }
    showMatrix();
}

/* 验证邻接矩阵可达性 */
int Verifier::accessVrfc(){

    std::function<int(const QVector<int> &)> exam = [&](const QVector<int> &ret)->int{
        for( int e : ret ) if( e == 0 ) return -1;
        return 0;
    };

    int n = matrix.size();
    QVector<int> visitedNode;
    visitedNode.resize(n);

    accessRule1Vrfc(0, n, visitedNode);
    if( exam(visitedNode) == -1 ) {
        emit verifyFalse("非法！该工作流存在可达性错误，至少存在一个节点由起点出发无法到达！");
        qDebug() << "非法！该工作流存在可达性错误，至少存在一个节点由起点出发无法到达！;";
        return -1;
    }

    visitedNode.clear();
    visitedNode.resize(n);
    accessRule2Vrfc(n-1, n, visitedNode);
    if( exam(visitedNode) == -1 ) {
        emit verifyFalse("非法！该工作流存在可达性错误，至少存在一个结点无法到达终点！");
        qDebug() << "非法！该工作流存在可达性错误，至少存在一个结点无法到达终点！";
        return -1;
    }

    return 0;
}

/* 可达性规则1的验证 */
int Verifier::accessRule1Vrfc(int seq, const int & n, QVector<int>& visitedNode){
    visitedNode[seq] = 1;
    qDebug() << __func__;
    for( int i = 1; i < n; i ++ ) {
        if( matrix[seq][i] == 1  && visitedNode[i] == 0  ) {
            visitedNode[i] = 1;
            accessRule1Vrfc(i, n, visitedNode);
        }
    }
    return 0;
}


/* 可达性规则2的验证 */
int Verifier::accessRule2Vrfc(int seq, const int & n, QVector<int>& visitedNode){
    visitedNode[seq] = 1;
    qDebug() << __func__;
    for( int i = n-2; i >= 0; i -- ) {
        if( matrix[i][seq] == 1  && visitedNode[i] == 0  ) {
                visitedNode[i] = 1;
                accessRule2Vrfc(i, n, visitedNode);
        }
    }
    return 0;
}

/* 验证非法并发单元结构验证算法 */
int Verifier::illStrcVrfc(NodeMap * nodeMap) {
    QStack<int> stk;
    QMap<int, int> dist;
    QVector<QVector<int>> visit(sortedNodes.size(), QVector<int>(sortedNodes.size()));
    QVector<QPair<int, QString>> errMsg;

    illStrcRec(0, nodeMap, stk, dist, visit, errMsg);

    for (auto key : dist.keys()) {
        int value = dist.value(key);
        qDebug() << "key:" << key << "value:" << value;
        if(value == -1){
            QString errStr = "存在死循环";
            qDebug() << errStr;
            errMsg.push_back(QPair<int, QString>(key,errStr));
        }
    }


    if(errMsg.size()) {
        QString errStr = "";
        for( auto e : errMsg ) {
            errStr += ("结点【" + sortedNodes[e.first]->getName() + "】:  " + e.second + "\n");
            qDebug() << "结点【" << sortedNodes[e.first]->getName() << "】" << ":  " << e.second;
        }
        emit verifyFalse(errStr);
        return -1;
    }
	return 0;
}


/* 验证非法并发单元结构验证算法 递归验证 */
int Verifier::illStrcRec( int seq,
                          NodeMap * nodeMap,
                          QStack<int> stk,
                          QMap<int, int> & dist,
                          QVector<QVector<int>> & visit,
                          QVector<QPair<int, QString>> & errMsg )
{
    int entrance = -1, currentExport = -1;

    if( sortedNodes[seq]->inDgr > 1 ) {
        currentExport = seq;
        int currentEntrance = stk.empty()? -1:stk.top();
        //int currentEntrance = entrance;
        if( dist.count(currentEntrance) ) {

            if( dist[currentEntrance] == -1 ) {
                dist[currentEntrance] = currentExport;
                qDebug() << "匹配并发出口：" << currentEntrance << "：" << sortedNodes[currentEntrance]->getName()
                         << " -- " << currentExport << "：" << sortedNodes[currentExport]->getName();
                stk.pop();

                if( matrix[currentEntrance][currentExport] ||
                    matrix[currentExport][currentEntrance] ) {
                    errMsg.push_back(QPair<int, QString>(currentEntrance,
                                                         QString("非法，与并发出口直接相连")));
                    errMsg.push_back(QPair<int, QString>(currentExport,
                                                         QString("非法，与并发入口直接相连")));
                    return -1;
               }

            } else if( dist[currentEntrance] != currentExport ) {
                errMsg.push_back(QPair<int, QString>(currentEntrance,
                                                     QString("非法，该结点存在多个并发出口")));
                qDebug() << "节点[" << currentEntrance << "]: " << sortedNodes[currentEntrance]->getName() << "存在多个并发出口！";
                qDebug() << "先行并发出口 -- 节点[" << dist[currentEntrance] << "]: " << sortedNodes[dist[currentEntrance]]->getName();
                qDebug() << "后继并发出口 -- 节点[" << currentExport << "]: " << sortedNodes[currentExport]->getName();
                return -1;
            }

        } else {
            errMsg.push_back(QPair<int, QString>(currentExport,
                                                 QString("非法，该节点作为并发出口, 没有与其匹配的并发入口, 可能存在死循环")));
            return -1;
        }
    }

    if( sortedNodes[seq]->outDgr > 1 ) {
        entrance = seq;
        if( !dist.count(entrance) ) {
            dist.insert(entrance, -1);
            qDebug() << "记录并发入口：" << entrance << "：" << sortedNodes[entrance]->getName();
            stk.push(entrance);
        }
    }

    qDebug() << " --------- check dist ----------";
    for (auto key : dist.keys()) {
        int value = dist.value(key);
        qDebug() << "key:" << key << "value:" << value;
    }

    QList<FlowNode *> endClt;
    nodeMap->queryEndOfStartNode(sortedNodes[seq], endClt);
    for( FlowNode * node : endClt ) {
        //int i = seqMap[node];
        int i = node->seq;
        if( !visit[seq][i] ) {
            visit[seq][i] = 1;
            illStrcRec(i, nodeMap, stk, dist, visit, errMsg);
        }
    }
    return 0;
}


/* 验证 */
int Verifier::verification(NodeMap * nodeMap, QList<FlowNode *> nodesList) {

    // 选举工作流的起始结点和结束结点
    FlowNode * flowBegin = nullptr, * flowEnd = nullptr;

    if( electBE(nodesList, flowBegin, flowEnd) == -1 ) {
        qDebug() << "非法！工作流图缺少起点或终点！";
        return -1;
    }

    // 给工作流的结点排序;
    bool sortRet = sort(nodeMap, flowBegin, flowEnd, nodesList.size());
    if(sortRet == false) return -1;

    // 建立工作流图的邻接矩阵
    generateMatrix(nodeMap, sortedNodes.size());

    // 进行可达性验证;
    int ret = accessVrfc();
    qDebug() << ret;
    if( ret == -1 ) {
        qDebug() << "可达性验证不通过！存在不可达路径！";
        return -1;
    } else {
        qDebug() << "可达性验证通过！";
    }

    // 进行非法并发单元验证
    if( illStrcVrfc(nodeMap) == -1){
        qDebug() << "非法并发单元验证不通过！存在非法并发单元！";
        return -1;
    }else {
        qDebug() << "非法并发单元验证通过！";
    }

    qDebug() << "工作流图验证通过！";
    emit verifySuccess();
    return 0;
}

