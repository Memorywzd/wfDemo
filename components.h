#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

#include <QDebug>


using namespace std;

//组件类的父类
class Components : public QWidget
{
	Q_OBJECT
public:
	explicit Components(QWidget* parent = nullptr);
	virtual void setText(const QString& text) = 0;
	virtual QString getText() const = 0;

	string prop_L;
	string prop_T;
	string prop_b;
	string prop_v;

protected:
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	
	QPoint offset;
};

//继承自组件类的若干子类
//todo: 向父类构造函数传递参数时，应初始化TBv等属性
class TextForm : public Components
{
	Q_OBJECT
public:
	explicit TextForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;

	static int Type;

private:
	QLineEdit* lineEdit;
};

class AreaForm : public Components
{
	Q_OBJECT
public:
	explicit AreaForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;

	static int Type;

private:
	QTextEdit* textEdit;
};

class ButtonForm : public Components
{
	Q_OBJECT
public:
	explicit ButtonForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;

	static int Type;

private:
	QPushButton* button;
};
