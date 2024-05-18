#include "widget.h"

#include <QPushButton>
#include <QMenu>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    wfManager(new WorkflowManager(this, this))
{
    ui->setupUi(this);
    setUpConfig();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setUpConfig()
{
    connect(ui->treeWidget, &OrganizationTree::addNodeToUiArea,
            ui->uiContent, &UiContent::onAddNodeToUiContent);
    connect(ui->verifyBtn, &QPushButton::clicked,
            wfManager, &WorkflowManager::onVerifyRequest);
	connect(ui->commitBtn, &QPushButton::clicked,
			wfManager, &WorkflowManager::CommitWorkflow);
    /*connect(ui->createNewWorkflow, &QPushButton::clicked,
            wfManager, &WorkflowManager::onCreateNewWorkflow);
	connect(ui->createNewWorkflow, &QPushButton::clicked,
            ui->uiContent, &UiContent::onAddEmptyNode);*/

	QMenu* flowMenu = new QMenu(this);
	flowMenu->addAction("多对一三节点", ui->uiContent, &UiContent::onAddEmptyNode);
	flowMenu->addAction("一对多三节点");

	ui->createNewWorkflow->setMenu(flowMenu);
	ui->createNewWorkflow->setDefaultAction(flowMenu->actions().at(0));


	//setAttribute(Qt::WA_QuitOnClose, false);
    setFixedSize(this->width(), this->height());
    ui->flowUiArea->setWidget(ui->uiContent);
    //ui->uiContent->setFixedSize(1400,1400);
}

NodeMap* Widget::GetNodeMap()
{
    return ui->uiContent->map;
}

QList<FlowNode *>& Widget::GetNodesOnBoard()
{
    return ui->uiContent->nodesOnBoard;
}

UiContent* Widget::GetUiContent()
{
    return this->ui->uiContent;
}
