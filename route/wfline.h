#ifndef WFLINE_H
#define WFLINE_H

#include <QObject>
#include <QLine>
#include <QDebug>

class FlowNode;

class WfLine : public QObject, public QLine
{
    Q_OBJECT

public:
    enum function {
        UNINITIALIZED = 0,
        CONTINUE,
        APPLICATION,
        APPROVAL,
        DECLARATION,
        SEAL
        // ....
    };

public:
    explicit WfLine(QObject *parent = nullptr);
    WfLine(FlowNode *start, FlowNode *end ,QObject *parent = nullptr);
    void SetFunc(const QString& funcName);
    void Parse(const QString& funcName);

signals:

public slots:
    
public:
    FlowNode *startNode {nullptr};
    FlowNode *endNode {nullptr};
    WfLine::function funcId {UNINITIALIZED};
    QString funcName;
};

#endif // WFLINE_H
