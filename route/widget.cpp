#include "widget.h"

#include <QPushButton>
#include <QFileDialog>

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
    connect(ui->createNewWorkflow, &QPushButton::clicked,
            wfManager, &WorkflowManager::OnCreateNewWorkflow);
	connect(ui->createNewWorkflow, &QPushButton::clicked,
            ui->uiContent, &UiContent::onAddEmptyNode);

	//setAttribute(Qt::WA_QuitOnClose, false);
    setFixedSize(this->width(), this->height());
    ui->flowUiArea->setWidget(ui->uiContent);
    ui->uiContent->setFixedSize(1400,1400);
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
