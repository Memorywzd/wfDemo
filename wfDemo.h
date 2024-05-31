#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QListWidget>
#include <QJsonObject>

#include <vector>

#include "ui_wfDemo.h"
#include "components.h"

#include "wfDemoRegister.h"
#include "route/widget.h"

class wfDemo : public QMainWindow
{
	Q_OBJECT

public:
	wfDemo(QWidget *parent = nullptr);
	~wfDemo();

private:
	void encodeComponents();			// encode components to json
	void getSqlbyCode();				// get sql by json

private:
	Ui::wfDemoClass ui;

	Widget* routeDialog;

	QGraphicsScene* scene;

	int tindexid, wfindexid;
	bool writeTable, writeDirectory;
	QString tableName;
	vector<string> columns;

	TextForm* textForm;					// components
	AreaForm* areaForm;
	ComboBoxForm* comboBoxForm;
	CheckBoxForm* checkBoxForm;
	RadioForm* radioForm;
	ButtonForm* buttonForm;				// components

	vector<TextForm*> textForms;		// pointers to components
	vector<AreaForm*> areaForms;		
	vector<ComboBoxForm*> comboBoxForms;
	vector<CheckBoxForm*> checkBoxForms;
	vector<RadioForm*> radioForms;
	vector<ButtonForm*> buttonForms;	// pointers to components

	QJsonObject json;
	QString sql;
};
