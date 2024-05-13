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
	: QMainWindow(parent), tindexid(1), wfindexid(1), writeTable(true), tableName("table"), 
	writeDirectory(false), textForm(nullptr), areaForm(nullptr), buttonForm(nullptr)
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
			textForms.push_back(textForm);
		}
		else if (item->text() == "Area Form") {
			areaForm = new AreaForm;
			scene->addWidget(areaForm);
			areaForms.push_back(areaForm);
		}
		else if (item->text() == "Button Form") {
			buttonForm = new ButtonForm;
			scene->addWidget(buttonForm);
			buttonForms.push_back(buttonForm);
		}
	});
	routeDialog = new Widget(this->parentWidget());

	connect(ui.pushButton, &QPushButton::clicked, this, &wfDemo::encodeComponents);
	connect(ui.pushButton_2, &QPushButton::clicked, this, [this]() {
		scene->clear();
	});
	connect(ui.pushButton_3, &QPushButton::clicked, this, [this]() {
		routeDialog->show();
	});

	//routeDialog->show();
}

wfDemo::~wfDemo()
{}

void wfDemo::encodeComponents() {
	
	QJsonArray headers, components;
	QJsonObject header;

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
	for (auto text : textForms) {
		QJsonObject component;
		component.insert("l", text->getText());
		component.insert("t", text->prop_T);
		component.insert("b", text->prop_b);
		component.insert("v", text->prop_v);
		components.append(component);
	}

	for (auto area : areaForms) {
		QJsonObject component;
		component.insert("l", area->getText());
		component.insert("t", area->prop_T);
		component.insert("b", area->prop_b);
		component.insert("v", area->prop_v);
		components.append(component);
	}

	for (auto button : buttonForms) {
		QJsonObject component;
		component.insert("B", button->getText());
		components.append(component);
	}

	json.insert("temp", components);
	//使用QMessageBox显示json
	QMessageBox::information(nullptr, "编码", QJsonDocument(json).toJson());

	//打开register
	wfDemoRegister* registerDialog = new wfDemoRegister(this->parentWidget());
	registerDialog->show();
}
