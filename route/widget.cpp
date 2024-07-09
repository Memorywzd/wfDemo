#include "widget.h"

#include <QPushButton>
#include <QMenu>

#include <QMessageBox>
#include <QJsonObject>
#include <QJsonarray>
#include <QJsonDocument>

#include <chrono>
using namespace std::chrono;
using namespace std;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::Widget),
	verifier(new Verifier(this))
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
	// oringinal workflow manager
    connect(verifier, &Verifier::verifyFalse, this, &Widget::onVerifyFalse);
    connect(verifier, &Verifier::verifySuccess, this, &Widget::onVerifySuccess);
	// end of original workflow manager

    connect(ui->treeWidget, &OrganizationTree::addNodeToUiArea,
            ui->uiContent, &UiContent::onAddNodeToUiContent);
	connect(ui->treeWidget, &OrganizationTree::regNodeToUiArea, 
			ui->uiContent, &UiContent::onRegNodeToUiContent);
    connect(ui->verifyBtn, &QPushButton::clicked,
            this, &Widget::onVerifyRequest);
	connect(ui->commitBtn, &QPushButton::clicked,
			this, &Widget::CommitWorkflow);

    connect(ui->tempName, QOverload<int>::of(&QComboBox::currentIndexChanged),
		    this, &Widget::onTempNameChanged);
	connect(ui->flowName, &QLineEdit::textChanged,
			this, &Widget::onFlowNameChanged);

    
    /*connect(ui->createNewWorkflow, &QPushButton::clicked,
            wfManager, &Widget::onCreateNewWorkflow);
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

// oringinal workflow manager
/* 邻接矩阵搜索验证 */
void Widget::onVerifyRequest() {
    time_point<system_clock> start = system_clock::now();
    int ret = verifier->verification(ui->uiContent->map, ui->uiContent->nodesOnBoard);
    time_point<system_clock> end = system_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    qDebug() << "验证耗时：" << elapsed.count() << "s";
    if (ret == -1) qDebug() << "验证错误！";
}

void Widget::onVerifySuccess() {
    if (QMessageBox::Ok == \
        QMessageBox::question(nullptr, \
            "来自FlowMaster：“验证通过”", \
            "验证通过， 是否提交该工作流？", \
            QMessageBox::Ok | QMessageBox::Ignore, \
            QMessageBox::Ignore
        ))
    {
        qDebug() << "用户选择的是提交";
        this->CommitWorkflow();

    }
    else {
        qDebug() << "用户选择的是忽略";
        return;
    }
}

void Widget::onCreateNewWorkflow()
{
    QString alertInfo = "请在下方绘板上绘制您的流程，在经过验证后方可提交";
    QMessageBox::information(nullptr, "来自FlowMaster：“新建流程”", alertInfo);
    ui->uiContent->map->clear();
}

void Widget::onVerifyFalse(QString errMsg) {
    //qDebug() << "验证不通过";
    QMessageBox::critical(nullptr, "来自FlowMaster：“验证不通过”", errMsg);
}

void Widget::onTempNameChanged(int index)
{
    qDebug() << "模板名称改变了，当前选择的是：" << index;
    tempName = index;
}

void Widget::onFlowNameChanged(const QString& text)
{
    qDebug() << "流程名称改变了，当前输入的是：" << text;
    flowName = text;
}

void Widget::CommitWorkflow()
{
    QJsonArray routes;
    QJsonObject json;

    int route_step = 1, router_id = 0;
    int nodesCnt = this->verifier->getSortedNodes().size();

    for (auto node : this->verifier->getSortedNodes()) {
        if (route_step == nodesCnt) router_id = -1;
        QJsonObject route, routeArr;
        route.insert("step", QString::number(node->lid + 1));
        route.insert("router_id", QString::number(router_id));
        route.insert("cn", "q7@bjfu.edu.cn");
        route.insert("ldap", node->getLdap());
        routeArr.insert(QString::number(route_step), route);
        router_id++;
        route_step++;
        routes.append(routeArr);
    }

    json.insert("R", routes);
    QMessageBox::information(nullptr, "编码", QJsonDocument(json).toJson());
    qDebug() << "编码”" << QJsonDocument(json).toJson(QJsonDocument::Compact);

    string sql;
    sql = "insert into workflowRegister(Typeid, typename, name, json, owner, last, storage, isActivate, TindexID, verifier) ";
    sql += "values(1, '多对1三节点含回执学生作业提交工作流', ";
    sql += "'" + flowName.toStdString() + "', ";
    sql += "'" + QJsonDocument(json).toJson(QJsonDocument::Compact).toStdString() + "', ";
    sql += "'q7@bjfu.edu.cn', ";
    sql += "1, ";
    sql += "'q7@bjfu.edu.cn', ";
    sql += "false, ";
    sql += QString::number(tempName).toStdString() + ", ";
    sql += "''";

    QMessageBox::information(nullptr, "注册器", QString::fromStdString(sql));
}
//end of original workflow manager
