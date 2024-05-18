#include "workflowmanager.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonarray>
#include <QJsonDocument>

WorkflowManager::WorkflowManager(Listener* listener, QObject *parent) :
    QObject(parent),
    callBacklistenr(listener),
    verifier(new Verifier(this))
    //transformer(new XMLTransformer(this, this)),
    //outputter(new Outputter(this)),
    //importer(new Importer(this))
{
    connect(verifier, &Verifier::verifyFalse, this, &WorkflowManager::onVerifyFalse);
    connect(verifier, &Verifier::verifySuccess, this, &WorkflowManager::onVerifySuccess);

}

/*QPair<QString, int> WorkflowManager::OnXMLTransformerGetFunc(FlowNode* start, FlowNode* end)
{
    auto &routeMaps = callBacklistenr->GetNodeMap()->queryRoutesMap();
    for( auto it = routeMaps.begin(); it != routeMaps.end(); it ++ )
    {
        WfLine* line = (*it);
        if(line->startNode == start && line->endNode == end)
        {
            return QPair<QString, int>(line->funcName, line->funcId);
        }
    }
    return QPair<QString, int>("", -1);
}*/


/* 邻接矩阵搜索验证 */
void WorkflowManager::onVerifyRequest() {
    int ret = verifier->verification(callBacklistenr->GetNodeMap(), callBacklistenr->GetNodesOnBoard());
    if( ret == -1 ) qDebug() << "验证错误！";
}

void WorkflowManager::onVerifySuccess() {
    if(QMessageBox::Ok == \
        QMessageBox::question(nullptr, \
                              "来自FlowMaster：“验证通过”", \
                              "验证通过， 是否提交该工作流？", \
                              QMessageBox::Ok | QMessageBox::Ignore, \
                              QMessageBox::Ignore
                              ))
    {
        qDebug() << "用户选择的是提交";
        this->CommitWorkflow();

    } else {
        qDebug() << "用户选择的是忽略";
        return;
    }
}

void WorkflowManager::onCreateNewWorkflow()
{
    QString alertInfo = "请在下方绘板上绘制您的流程，在经过验证后方可提交";
    QMessageBox::information(nullptr, "来自FlowMaster：“新建流程”", alertInfo);
    this->callBacklistenr->GetNodeMap()->clear();
}

/*void WorkflowManager::OnSelectWorkflowTemplate()
{
    QString fileName = this->callBacklistenr->GetUiContent()->ChooseXML();
    QDomDocument &&doc = this->importer->importXML(fileName);
    bool ret = this->transformer->TransformXMLIntoUi(this->callBacklistenr->GetNodeMap(),
                                          this->callBacklistenr->GetUiContent(),
                                          doc);

    if(ret) qDebug() << "导出工作流成功！";

}*/

void WorkflowManager::onVerifyFalse(QString errMsg){
    QMessageBox::critical(nullptr, "来自FlowMaster：“验证不通过”", errMsg);
}

void WorkflowManager::CommitWorkflow()
{
    /*QString wid = "";
    QDomDocument &&doc = this->transformer->TransformUiIntoXML(callBacklistenr->GetNodeMap(),
                                                               this->verifier->sortedNodes,
                                                               wid);
    this->outputter->SaveXML(doc, wid);*/

    QJsonArray routes;
    QJsonObject json;

    int route_step = 1, router_id = 0;
    int nodesCnt = this->verifier->sortedNodes.size();

    for (auto node : this->verifier->sortedNodes) {
        if (route_step == nodesCnt) router_id = -1;
        QJsonObject route, routeArr;
        route.insert("step", QString::number(node->lid));
        route.insert("router_id", QString::number(router_id));
		route.insert("cn", "jb@bjfu.edu.cn");
        route.insert("ldap", node->getName());
        routeArr.insert(QString::number(route_step), route);
        router_id++;
		route_step++;
        routes.append(routeArr);
    }

	json.insert("R", routes);
	QMessageBox::information(nullptr, "编码”", QJsonDocument(json).toJson());

    string sql;
	sql = "insert into workflowRegister(Typeid, typename, name, json, owner, last, storage, isActivate, TindexID, verifier) ";

}
