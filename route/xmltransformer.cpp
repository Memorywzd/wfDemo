#include "xmltransformer.h"


XMLTransformer::XMLTransformer(Listener* listener ,QObject *parent) : QObject(parent), callBackListener(listener)
{

}


/*QDomDocument XMLTransformer::TransformUiIntoXML(NodeMap* map, QVector<FlowNode*>& sortedNodes, QString& widStr)
{
    QDomDocument doc;

    // 创建根元素
    QDomElement root = doc.createElement("workflow");
    doc.appendChild(root);

    // 设置workflow属性
    // 添加 <wid> 子元素
    QDomElement wid = doc.createElement("wid");
    widStr = this->GetUuid();
    QDomText widText = doc.createTextNode(widStr);
    wid.appendChild(widText);
    root.appendChild(wid);

    // 添加 <tid> 子元素
    QDomElement tid = doc.createElement("tid");
    QDomText tidText = doc.createTextNode("xxx");
    tid.appendChild(tidText);
    root.appendChild(tid);

    // 添加 <timestap> 子元素 -- UTC毫秒级时间戳
    QDomElement timestap = doc.createElement("timestap");
    QDomText timestapText = doc.createTextNode(this->GetTimeMsStr());
    timestap.appendChild(timestapText);
    root.appendChild(timestap);

    // 添加 <date_time>
    QDomElement dateTime = doc.createElement("date_time");
    QDomText dateTimeText = doc.createTextNode(this->GetDateTime());
    dateTime.appendChild(dateTimeText);
    root.appendChild(dateTime);

    // 添加 <version> 子元素 --- 该工作流版本号
    QDomElement version = doc.createElement("version");
    QDomText versionText = doc.createTextNode("xxx");
    version.appendChild(versionText);
    root.appendChild(version);

    // 添加节点元素 node

    // 获取所有的节点，按排序获取

    int nodesCnt = sortedNodes.size();
    for(int idx = 0; idx < nodesCnt; idx ++)
    {
        // 添加 <node> 子元素
        QDomElement node = doc.createElement("node");
        node.setAttribute("index", idx);
        root.appendChild(node);

        // 添加 <oid> 子元素
        QDomElement oid = doc.createElement("oid");
        QDomText oidText = doc.createTextNode("xxx");
        oid.appendChild(oidText);
        node.appendChild(oid);

        // 添加 <ouid> 子元素
        QDomElement ouid = doc.createElement("ouid");
        QDomText ouidText = doc.createTextNode("xxx");
        ouid.appendChild(ouidText);
        node.appendChild(ouid);

        // 添加 <uid> 子元素
        QDomElement uid = doc.createElement("uid");
        QDomText uidText = doc.createTextNode("xxx");
        uid.appendChild(uidText);
        node.appendChild(uid);

        // 添加 <name> 子元素
        QDomElement name = doc.createElement("name");
        name.appendChild(doc.createTextNode(sortedNodes[idx]->getName()));
        node.appendChild(name);

        // 添加 <rid> 子元素 --- 该节点在图中的重复出现序号
        QDomElement rid = doc.createElement("rid");
        rid.appendChild(doc.createTextNode(QString::number(sortedNodes[idx]->rid)));
        node.appendChild(rid);

        // 添加 <lid> 子元素 --- 所在层次 / 深度
        QDomElement lid = doc.createElement("lid");
        lid.appendChild(doc.createTextNode(QString::number(sortedNodes[idx]->lid)));
        node.appendChild(lid);

        // 添加 <lseq> 子元素 --- 所在当前层中的排序序号
        QDomElement lseq = doc.createElement("lseq");
        lseq.appendChild(doc.createTextNode(QString::number(sortedNodes[idx]->lseq)));
        node.appendChild(lseq);


        // 添加 <seq> 子元素 --- 对图排序后节点的序号
        QDomElement seq = doc.createElement("seq");
        seq.appendChild(doc.createTextNode(QString::number(sortedNodes[idx]->seq)));
        node.appendChild(seq);

        // 添加 <prototype_id> 子元素
        QDomElement prototype_id = doc.createElement("prototype_id");
        QDomText prototype_idText;
        if( idx == 0 ) prototype_idText = doc.createTextNode("start");
        else if( idx == nodesCnt-1 ) prototype_idText = doc.createTextNode("end");
        else prototype_idText = doc.createTextNode("relay");
        prototype_id.appendChild(prototype_idText);
        node.appendChild(prototype_id);

        QList<FlowNode *> nextNodes;
        map->queryEndOfStartNode(sortedNodes[idx], nextNodes);
        int nextCnt = nextNodes.size();

        for( int nidx = 0; nidx < nextCnt; nidx ++ )
        {
            QDomElement next_node = doc.createElement("next_node");
            next_node.setAttribute("index", nidx);
            node.appendChild(next_node);

            // 添加 <uuid> 子元素
            QDomElement next_uuid = doc.createElement("uuid");
            next_uuid.appendChild(doc.createTextNode("xxx"));
            next_node.appendChild(next_uuid);

            // 添加 <seq> 子元素
            QDomElement next_seq = doc.createElement("seq");
            next_seq.appendChild(doc.createTextNode(QString::number(nextNodes[nidx]->seq)));
            next_node.appendChild(next_seq);

            // 添加 <name> 子元素
            QDomElement next_name = doc.createElement("name");
            next_name.appendChild(doc.createTextNode(nextNodes[nidx]->getName()));
            next_node.appendChild(next_name);

            auto func = callBackListener->OnXMLTransformerGetFunc(sortedNodes[idx], nextNodes[nidx]);
            QString funcName = func.first;
            int funcId = func.second;

            // 添加 <function_id> 子元素
            QDomElement function_id = doc.createElement("function_id");
            function_id.appendChild(doc.createTextNode(QString::number(funcId)));
            next_node.appendChild(function_id);

            // 添加 <function_name> 子元素
            QDomElement function_name = doc.createElement("function_name");
            function_name.appendChild(doc.createTextNode(funcName));
            next_node.appendChild(function_name);

        }
    }
    return doc;
}

bool XMLTransformer::TransformXMLIntoUi(NodeMap *map, UiContent *uiContent, const QDomDocument &doc)
{
    map->clear();
    QList<FlowNode *> sortedNodes;
    
    QDomNodeList nodes = doc.elementsByTagName("node");
    if(nodes.isEmpty()) return false;

    int nodesCnt = nodes.count();
    sortedNodes.reserve(nodesCnt);

    // 实例化节点 && 排序
    for(int i = 0; i < nodesCnt; i++)
    {
        QDomElement nodeElem = nodes.at(i).toElement();
        QString uid  = nodeElem.firstChildElement("uid").text();
        QString name = nodeElem.firstChildElement("name").text();
        QString seq  = nodeElem.firstChildElement("seq").text();
        QString lid  = nodeElem.firstChildElement("lid").text();
        QString lseq = nodeElem.firstChildElement("lseq").text();

        FlowNode *node = new FlowNode(name, uiContent, true, lid.toInt(), lseq.toInt());
        uiContent->SetUpNewNodeAction(node);
        // 要将treeItem和node建立联系, 以便从treeItem里面拿oid、ouid、uid等基本信息，但不属于本人毕设范畴，先预留接口
        uiContent->RegisterNewNodeByOrgTreeItem(node);

        sortedNodes.append(node);
    }

    for(int i = 0; i < nodesCnt; i++)
    {
        QDomElement nodeElem = nodes.at(i).toElement();
        QString seq = nodeElem.firstChildElement("seq").text();

        //读取next_node字段
        QDomNodeList nextNodes = nodeElem.elementsByTagName("next_node");

        int nextNodesCnt = nextNodes.count();
        sortedNodes[seq.toInt()]->outDgr = nextNodesCnt;
        for(int j = 0; j < nextNodesCnt; j++)
        {
            QDomElement nextNodeElem = nextNodes.at(j).toElement();
            QString nextSeq  = nextNodeElem.firstChildElement("seq").text();
            QString funcName = nextNodeElem.firstChildElement("function_name").text();
            int funcId = nextNodeElem.firstChildElement("function_id").text().toInt();

            qDebug() << "Nextseq = " << seq << " --- "<< __func__ << funcName <<": " << funcId;

            map->insertRoute(sortedNodes[seq.toInt()], sortedNodes[nextSeq.toInt()],
                                funcName, static_cast<WfLine::function>(funcId));

            sortedNodes[nextSeq.toInt()]->inDgr ++;
        }
    }
    qDebug() << "map.size() = " << map->size();
    map->showRoute__debug();
    return true;
}*/

QPair<QString, int> XMLTransformer::GetFunc(FlowNode *start, FlowNode *end)
{
    return callBackListener->OnXMLTransformerGetFunc(start, end);
}
