#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

class MariaUI :	public QMainWindow
{
public:
	MariaUI(void);
	~MariaUI(void);
	QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi()
    {
        if (this->objectName().isEmpty())
            this->setObjectName(QStringLiteral("MariaUI"));

        this->resize(480, 160);

        menuBar = new QMenuBar(this);
        this->menuBar->setObjectName(QStringLiteral("menuBar"));
        this->setMenuBar(menuBar);

        mainToolBar = new QToolBar(this);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        this->addToolBar(mainToolBar);

        centralWidget = new QWidget(this);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        this->setCentralWidget(centralWidget);

        statusBar = new QStatusBar(this);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        this->setStatusBar(statusBar);

        retranslateUi();

        QMetaObject::connectSlotsByName(this);
    } // setupUi

private:
    void retranslateUi()
    {
        this->setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));
    } // retranslateUi

};

