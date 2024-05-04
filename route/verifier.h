#ifndef VERIFIER_H
#define VERIFIER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QStack>
#include <QQueue>
#include <QtGlobal>
#include <iostream>
#include <functional>
#include "nodemap.h"

class Verifier : public QObject
{
    Q_OBJECT

public:
    Verifier(QObject *parent = 0);

public slots:
    int verification(NodeMap * nodeMap, QList<FlowNode *> nodesList);

private:
    int electBE( const QList<FlowNode *> & nodesList,
                 FlowNode* & flowBegin,
                 FlowNode* & flowEnd );

    void showSeq();
    void seqMapping();
    void generateMatrix(NodeMap * nodeMap, int n);
    void showMatrix();

    bool sort(NodeMap* & nodeMap,
              FlowNode* &flowBegin,
              FlowNode* &flowEnd,
              int n);

    int accessVrfc();
    int accessRule1Vrfc(int seq, const int & n, QVector<int>& visitedNode);
    int accessRule2Vrfc(int seq, const int & n, QVector<int>& visitedNode);
    int illStrcVrfc(NodeMap * nodeMap);
    int illStrcRec( int seq,
                    NodeMap * nodeMap,
                    QStack<int> stk,
                    QMap<int, int> & dist,
                    QVector<QVector<int>> & visit,
                    QVector<QPair<int, QString>> & errMsg );

signals:
    void verifyFalse(QString errMsg);
    void verifySuccess();


private:
    QVector<FlowNode *> sortedNodes;
    QMap<FlowNode *, int> seqMap;
    QVector<QVector<int>> matrix;

    friend class WorkflowManager;
    friend class XMLTransformer;


};

#endif // VERIFIER_H
