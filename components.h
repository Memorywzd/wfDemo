#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>

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
	virtual QString getDescribe() const = 0;

	int permission = 22;
	int align = 0;

	QString prop_L;
	QString prop_T;
	QString prop_b;
	QString prop_v;

public slots:
	void edit_L();
	void edit_T();
	void edit_b();
	void edit_permission();
	void edit_align();

protected:
	//鼠标事件
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
	QString getDescribe() const override;

private:
	QLabel* label;
	QLineEdit* lineEdit;
};

class AreaForm : public Components
{
	Q_OBJECT
public:
	explicit AreaForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;
	QString getDescribe() const override;

private:
	QLabel* label;
	QTextEdit* textEdit;
};

class ComboBoxForm : public Components
{
	Q_OBJECT
public:
	explicit ComboBoxForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;
	QString getDescribe() const override;

	void setItems();
	QString getItems();
	void mousePressEvent(QMouseEvent* event) override;

	QStringList items;

private:
	QLabel* label;
	QComboBox* comboBox;
	
};

class CheckBoxForm : public Components
{
	Q_OBJECT
public:
	explicit CheckBoxForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;
	QString getDescribe() const override;

	void setItems();
	void mousePressEvent(QMouseEvent* event) override;

private:
	QLabel* label;
	QCheckBox* checkBox;
};

class RadioForm : public Components
{
	Q_OBJECT
public:
	explicit RadioForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;
	QString getDescribe() const override;
	
	void setItems();
	void mousePressEvent(QMouseEvent* event) override;

private:
	QLabel* label;
	QRadioButton* radioButton;
};

class ButtonForm : public Components
{
	Q_OBJECT
public:
	explicit ButtonForm(QWidget* parent = nullptr);
	void setText(const QString& text) override;
	QString getText() const override;
	QString getDescribe() const override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	QPushButton* button;
};
