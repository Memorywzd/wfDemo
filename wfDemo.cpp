#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QGraphicsItem>
#include <QJsonObject>
#include <QJsonarray>
#include <QJsonDocument>
#include <QMessageBox>

#include "wfDemo.h"


wfDemo::wfDemo(QWidget *parent)
	: QMainWindow(parent), tindexid(1), wfindexid(1), writeTable(true), tableName("table"), writeDirectory(false)
{
	ui.setupUi(this);

	ui.listWidget->addItem("Text Form");
	ui.listWidget->addItem("Area Form");
	ui.listWidget->addItem("Button Form");

	scene = new QGraphicsScene;
	ui.graphicsView->setScene(scene);
	scene->setSceneRect(-20, -20, 800, 450);

	connect(ui.listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item) {
		if (item->text() == "Text Form") {
			textForm = new TextForm;
			scene->addWidget(textForm);
		}
		else if (item->text() == "Area Form") {
			areaForm = new AreaForm;
			scene->addWidget(areaForm);
		}
		else if (item->text() == "Button Form") {
			buttonForm = new ButtonForm;
			scene->addWidget(buttonForm);
		}
	});

	connect(ui.pushButton, &QPushButton::clicked, this, &wfDemo::encodeComponents);
	connect(ui.pushButton_2, &QPushButton::clicked, this, [this]() {
		scene->clear();
	});

	/*QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

	//左侧组件列表
	componentsList = new QListWidget;
	createLeftComponents();
	
	QVBoxLayout* leftLayout = new QVBoxLayout;
	QLabel* label = new QLabel("预定义组件列表");
	leftLayout->addWidget(label);
	leftLayout->addWidget(componentsList);
	mainLayout->addLayout(leftLayout);

	//右侧画布
	QVBoxLayout* rightLayout = new QVBoxLayout;

	view = new QGraphicsView;
	scene = new QGraphicsScene;
	view->setScene(scene);
	scene->setSceneRect(-20, -20, 800, 600);

	rightLayout->addWidget(view);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	QPushButton* encodeButton = new QPushButton("编码");
	connect(encodeButton, &QPushButton::clicked, this, &wfDemo::encodeComponents);
	buttonLayout->addWidget(encodeButton);
	QPushButton* resetButton = new QPushButton("重置");
	connect(resetButton, &QPushButton::clicked, this, &wfDemo::resetComponents);
	buttonLayout->addWidget(resetButton);
	rightLayout->addLayout(buttonLayout);
	mainLayout->addLayout(rightLayout);*/
}

wfDemo::~wfDemo()
{}

void wfDemo::encodeComponents() {
	
	QJsonArray headers, components;
	QJsonObject header, component;

	header.insert("start", "1");
	header.insert("tindexid", tindexid);
	header.insert("wfindexid", wfindexid);
	header.insert("writeTable", writeTable);
	header.insert("tableName", tableName);
	header.insert("writeDirectory", writeDirectory);
	header.insert("directory", "");
	header.insert("timeofStart", "");
	header.insert("timeofStop", "");
	headers.append(header);
	json.insert("head", headers);

	//遍历scene中的所有组件
	for (auto item : scene->items()) {
		component.insert("type", item->type());
		component.insert("x", item->pos().x());
		component.insert("y", item->pos().y());
		component.insert("width", item->boundingRect().width());
		component.insert("height", item->boundingRect().height());
		components.append(component);
	}

	json.insert("temp", components);
	//使用QMessageBox显示json
	QMessageBox::information(this, "编码", QJsonDocument(json).toJson());
}
