#pragma once

#include <QWidget>
#include "ui_tableDesignerDemo.h"

class tableDesignerDemo : public QWidget
{
	Q_OBJECT

public:
	tableDesignerDemo(QWidget *parent = nullptr);
	~tableDesignerDemo();

private:
	void setTable(int row);

	Ui::tableDesignerDemoClass ui;
};
