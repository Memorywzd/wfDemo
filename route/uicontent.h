#ifndef UICONTENT_H
#define UICONTENT_H

#include <QObject>
#include <QWidget>
#include <QPointF>
#include <QFloat16>
#include <QList>
#include <QtCore/qmath.h>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QComboBox>
#include "organizationtree.h"
#include "nodemap.h"

class UiContent : public QWidget
{
    Q_OBJECT

    enum ArrowMode {
        middle = 0,
        hover
    };

public:
    UiContent(QWidget *parent = nullptr);
    ~UiContent();

private:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
    void dragMoveEvent(QDragMoveEvent *ev);
    void dragLeaveEvent(QDragLeaveEvent *ev);
    void dropEvent(QDropEvent *ev);
    void contextMenuEvent(QContextMenuEvent *event);

    void resetTmpLine(QPoint startPos, QPoint endPos);

    void calcVertexes( const QPointF &startPos,
                       const QPointF &endPos,
                       QPointF &wingPos1,
                       QPointF &wingPos2);

    QPointF calArrowPos(FlowNode* startNode,
                        FlowNode* endNode,
                        float scale = 5.0/10);

    void SetupNodesMapAction();

public:
    void SetUpNewNodeAction(FlowNode *newNode);
    void RegisterNewNodeByOrgTreeItem(FlowNode *newNode);
    QString ChooseXML();
    QString SelectProcess();


signals:
    void permitDeleteNode(FlowNode* node);

public slots:
    void onLineStart(FlowNode *startNode);
    void onLineEnd(FlowNode *endNode);
    void onRequestForDeleteNode(FlowNode *node);
    void onAddNodeToUiContent();
    void onDeleteAllNodesOnBoard();

    //add by memory
	void onAddEmptyNode();

public:
    static QPoint dragStartCursorPos;
    static bool isDrawing;

private:
    QPoint tmpLineStartPos;
    QPoint tmpLineEndPos;
    FlowNode * curStartNode     {nullptr};

private:
    NodeMap * map               {nullptr};
    QList<FlowNode *> nodesOnBoard;

    friend class Widget;
    friend class WorkflowManager;

    ArrowMode arrowMode {hover};

};

#endif // UICONTENT_H


