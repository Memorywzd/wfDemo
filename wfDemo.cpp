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
#include "queryDemo.h"
#include "tableDesignerDemo.h"


wfDemo::wfDemo(QWidget *parent)
	: QMainWindow(parent), tindexid(1), wfindexid(1)
	//, writeTable(true), tableName("table"), 
	//writeDirectory(false), textForm(nullptr), areaForm(nullptr), buttonForm(nullptr)
{
	ui.setupUi(this);
	this->setFixedSize(1134, 735);

	ui.listWidget->addItem("Text Form");
	ui.listWidget->addItem("Area Form");
	ui.listWidget->addItem("ComboBox Form");
	ui.listWidget->addItem("CheckBox Form");
	ui.listWidget->addItem("Radio Form");
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
		else if (item->text() == "ComboBox Form") {
			comboBoxForm = new ComboBoxForm;
			scene->addWidget(comboBoxForm);
			comboBoxForms.push_back(comboBoxForm);
		}
		else if (item->text() == "CheckBox Form") {
			checkBoxForm = new CheckBoxForm;
			scene->addWidget(checkBoxForm);
			checkBoxForms.push_back(checkBoxForm);
		}
		else if (item->text() == "Radio Form") {
			radioForm = new RadioForm;
			scene->addWidget(radioForm);
			radioForms.push_back(radioForm);
		}
	});

	connect(ui.pushButton, &QPushButton::clicked, this, &wfDemo::encodeComponents);
	connect(ui.pushButton_2, &QPushButton::clicked, this, [this]() {
		scene->clear();
	});

	ui.radioButton_2->setChecked(true);
		QButtonGroup* writeTaleGroup = new QButtonGroup(this);
	writeTaleGroup->addButton(ui.radioButton);
	writeTaleGroup->addButton(ui.radioButton_2);
	connect(writeTaleGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this](QAbstractButton* button) {
		if (button == ui.radioButton) {
			writeTable = true;
			ui.label_8->setEnabled(true);
			ui.lineEdit_4->setEnabled(true);
			ui.pushButton_4->setEnabled(true);
		}
		else if (button == ui.radioButton_2) {
			writeTable = false;
			ui.label_8->setEnabled(false);
			ui.lineEdit_4->setEnabled(false);
			ui.pushButton_4->setEnabled(false);
		}
	});

	ui.radioButton_4->setChecked(true);
	QButtonGroup* copyGroup = new QButtonGroup(this);
	copyGroup->addButton(ui.radioButton_3);
	copyGroup->addButton(ui.radioButton_4);
	connect(copyGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, [this](QAbstractButton* button) {
		if (button == ui.radioButton_3) {
			writeDirectory = true;
		}
		else if (button == ui.radioButton_4) {
			writeDirectory = false;
		}
	});

	connect(ui.lineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
		tindexid = text.toInt();
	});
	connect(ui.lineEdit_2, &QLineEdit::textChanged, this, [this](const QString& text) {
		wfindexid = text.toInt();
	});
	connect(ui.lineEdit_3, &QLineEdit::textChanged, this, [this](const QString& text) {
		tableName = text;
	});

	routeDialog = new Widget(this->parentWidget());

	connect(ui.pushButton_3, &QPushButton::clicked, this, [this]() {
		routeDialog->show();
});
	//routeDialog->show();

	queryDemo* queryDialog = new queryDemo(nullptr);
	tableDesignerDemo* tableDesignerDialog = new tableDesignerDemo(nullptr);
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
		columns.push_back(text->prop_b.toStdString());
		component.insert("v", text->prop_v);
		component.insert("permission", text->permission);
		component.insert("align", text->align);
		components.append(component);
	}

	for (auto area : areaForms) {
		QJsonObject component;
		component.insert("l", area->getText());
		component.insert("t", area->prop_T);
		component.insert("b", area->prop_b);
		columns.push_back(area->prop_b.toStdString());
		component.insert("v", area->prop_v);
		component.insert("permission", area->permission);
		component.insert("align", area->align);
		components.append(component);
	}

	for (auto button : buttonForms) {
		QJsonObject component;
		component.insert("B", button->getText());
		components.append(component);
	}

	for (auto comboBox : comboBoxForms) {
		QJsonObject component;
		component.insert("l", comboBox->getText());
		component.insert("t", comboBox->prop_T);
		component.insert("b", comboBox->prop_b);
		columns.push_back(comboBox->prop_b.toStdString());
		component.insert("v", comboBox->prop_v);
		QJsonArray items;
		QJsonObject select;
		for (auto item : comboBox->items) {
			select.insert("v", item);
			items.append(select);
		}
		component.insert("s", items);
		component.insert("permission", comboBox->permission);
		component.insert("align", comboBox->align);
		components.append(component);
	}

	for (auto checkBox : checkBoxForms) {
		QJsonObject component;
		component.insert("l", checkBox->getText());
		component.insert("t", checkBox->prop_T);
		component.insert("b", checkBox->prop_b);
		columns.push_back(checkBox->prop_b.toStdString());
		component.insert("v", checkBox->prop_v);
		component.insert("permission", checkBox->permission);
		component.insert("align", checkBox->align);
		components.append(component);
	}

	for (auto radio : radioForms) {
		QJsonObject component;
		component.insert("l", radio->getText());
		component.insert("t", radio->prop_T);
		component.insert("b", radio->prop_b);
		columns.push_back(radio->prop_b.toStdString());
		component.insert("v", radio->prop_v);
		component.insert("permission", radio->permission);
		component.insert("align", radio->align);
		components.append(component);
	}

	json.insert("temp", components);

	if (tableName.isEmpty()) {
		QMessageBox::warning(nullptr, "错误", "表名为空");
		return;
	}

	getSqlbyCode();

	//使用QMessageBox显示json
	QMessageBox::information(nullptr, "编码", QJsonDocument(json).toJson(QJsonDocument::Compact));

	//打开register
	wfDemoRegister* registerDialog = new wfDemoRegister(this->parentWidget(),
		json, sql, tableName);
	registerDialog->show();
}

void wfDemo::getSqlbyCode() {
	sql = "insert into ";
	sql += tableName + "(";
	for (string column : columns) {
		sql += QString::fromStdString(column) + ",";
	}
	sql.chop(1);
	sql += ")";
}
