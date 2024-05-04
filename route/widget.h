#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollArea>

#include "ui_widget.h"
#include "organizationtree.h"
#include "flownode.h"
#include "flowuiarea.h"
//#include "uicontent.h"
#include "workflowmanager.h"
#include "tools.h"
#include <QScrollArea>

namespace Ui {
class Widget;
}

class Widget : public QWidget, public WorkflowManager::Listener
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void setUpConfig();


/* inherited from WorkflowMaster::Listener */
public:
    NodeMap* GetNodeMap();
    QList<FlowNode *>& GetNodesOnBoard();
    UiContent* GetUiContent();



private:
    Ui::Widget *ui;
    WorkflowManager *wfManager {nullptr};
};



#endif // WIDGET_H
