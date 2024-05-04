#include "wfline.h"

WfLine::WfLine(QObject *parent) : QObject(parent)
{

}


WfLine::WfLine(FlowNode *start, FlowNode *end ,QObject *parent) :
    QObject(parent), startNode(start), endNode(end)
{

}

void WfLine::SetFunc(const QString& funcName)
{
    this->funcName = funcName;
    this->Parse(funcName);
}

void WfLine::Parse(const QString& funcName)
{
    if(funcName.contains("跟进")) {
        this->funcId = function::CONTINUE;
    } else if(funcName.contains("申请")) {
        this->funcId = function::APPLICATION;
    } else if(funcName.contains("审批")) {
        this->funcId = function::APPROVAL;
    } else if(funcName.contains("申报")) {
        this->funcId = function::DECLARATION;
    } else if(funcName.contains("盖章")) {
        this->funcId = function::SEAL;
    } else this->funcId = function::UNINITIALIZED;


    qDebug() << __func__ << "：" << this->funcName << ": " << funcId;
}
