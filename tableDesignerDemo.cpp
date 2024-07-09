#include "tableDesignerDemo.h"

#include <QTableWidgetItem>
#include <QCheckBox>
#include <QComboBox>

void tableDesignerDemo::setTable(int row)
{
	//将表格的第二列设置为下拉菜单
	QComboBox* comboBox = new QComboBox();
	comboBox->addItem("int");
	comboBox->addItem("varchar");
	comboBox->addItem("text");
	ui.table->setCellWidget(row, 1, comboBox);

	//将表格的第三列设置为单选框
	QTableWidgetItem* notNull = new QTableWidgetItem();
	notNull->setCheckState(Qt::Unchecked);
	ui.table->setItem(row, 2, notNull);

	//将表格的第四列设置为单选框
	QTableWidgetItem* key = new QTableWidgetItem();
	key->setCheckState(Qt::Unchecked);
	ui.table->setItem(row, 3, key);

	//将表格的第五列设置为单选框
	QTableWidgetItem* autoIncrement = new QTableWidgetItem();
	autoIncrement->setCheckState(Qt::Unchecked);
	ui.table->setItem(row, 4, autoIncrement);
}

tableDesignerDemo::tableDesignerDemo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.addLine, &QPushButton::clicked, [this]() {
		ui.table->insertRow(ui.table->rowCount());
		setTable(ui.table->rowCount() - 1);
	});

	connect(ui.removeLine, &QPushButton::clicked, [this]() {
		ui.table->removeRow(ui.table->currentRow());
	});

	ui.table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应列宽


	//show();
}

tableDesignerDemo::~tableDesignerDemo()
{}
