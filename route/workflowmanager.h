#ifndef WORKFLOWMANAGER_H
#define WORKFLOWMANAGER_H

#include <QObject>
#include <QWidget>
#include "verifier.h"
#include "uicontent.h"

class Widget;

class WorkflowManager : public QObject
{
    Q_OBJECT

public:

    class Listener
    {
    public:
        virtual NodeMap* GetNodeMap()                = 0;
        virtual QList<FlowNode *>& GetNodesOnBoard() = 0;
        virtual UiContent* GetUiContent()            = 0;
    };

public:
    explicit WorkflowManager(Listener* listener, QObject *parent = nullptr);
    void CommitWorkflow();

/* inhireted from XMLTransformer::Listener 
public:
    QPair<QString, int> OnXMLTransformerGetFunc(FlowNode *start, FlowNode *end);*/

public slots:
    void onVerifyRequest();
    void onVerifyFalse(QString errMsg);
    void onVerifySuccess();
    void onCreateNewWorkflow();
    //void OnSelectWorkflowTemplate();

private:
    // 这些指针，析构的时候记得释放，否则内存泄露
    Verifier *verifier          {nullptr};
    //XMLTransformer *transformer {nullptr};
    //Outputter *outputter        {nullptr};
    //Importer *importer          {nullptr};
    Listener *callBacklistenr   {nullptr};

};

#endif // WORKFLOWMANAGER_H
