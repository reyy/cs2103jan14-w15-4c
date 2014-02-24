#pragma once
#include <QtCore/QCoreApplication>
#include "MariaUI.h"

class MariaLogic : QApplication
{
private:
	MariaUI *mariaUI;

public:
	MariaLogic(int argc, char *argv[]);
	~MariaLogic(void);
};

