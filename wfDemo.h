#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QListWidget>
#include <QJsonObject>

#include "ui_wfDemo.h"
#include "components.h"

class wfDemo : public QMainWindow
{
	Q_OBJECT

public:
	wfDemo(QWidget *parent = nullptr);
	~wfDemo();

private:

	void encodeComponents();
	void getSqlbyCode();

private:
	Ui::wfDemoClass ui;

	QGraphicsScene* scene;

	int tindexid, wfindexid;
	bool writeTable, writeDirectory;
	QString tableName;

	TextForm* textForm;
	AreaForm* areaForm;
	ButtonForm* buttonForm;

	QJsonObject json;
};
