#ifndef WORKFLOWMANAGER_H
#define WORKFLOWMANAGER_H

#include <QObject>
#include <QWidget>
#include "verifier.h"

class Widget;

class WorkflowManager : public QObject
{
    Q_OBJECT

public:

public:
    explicit WorkflowManager(QObject *parent = nullptr);
    void CommitWorkflow();
    
	void onTempNameChanged(int index);
	void onFlowNameChanged(const QString& text);

public slots:
    void onVerifyRequest();
    void onVerifyFalse(QString errMsg);
    void onVerifySuccess();
    void onCreateNewWorkflow();

private:
    // 这些指针，析构的时候记得释放，否则内存泄露
    Verifier *verifier          {nullptr};

	int tempName = 0;
	QString flowName;
};

#endif // WORKFLOWMANAGER_H
