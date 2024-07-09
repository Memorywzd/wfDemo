#pragma once

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>

#include "ui_queryDemo.h"


class queryDemo : public QWidget
{
	Q_OBJECT

public:
	queryDemo(QWidget *parent = nullptr);
	void initializeModel(QSqlTableModel* model);
	QTableView* createView(QSqlTableModel* model, const QString& title = "");
	~queryDemo();

private:
	//Ui::queryDemoClass ui;
};
