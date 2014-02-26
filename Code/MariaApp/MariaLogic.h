#pragma once

#include <QtCore/QCoreApplication>
#include "MariaUI.h"

class MariaLogic : QApplication {
private:
	MariaUI *mariaUI;

protected:

public:
	MariaLogic(int argc, char *argv[]);
	~MariaLogic(void);
};
