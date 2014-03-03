#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>

#define AMOUNT_OF_IMAGES 6

class MariaUIStatus : QWidget {
	Q_OBJECT
public:
	enum STATUS_TYPE {
		OK, INVALID, WAIT, UNKNOWN, NONE
	};

private:
	QMainWindow *_parent;
	
	STATUS_TYPE _currentStatus;
	int _statusImageIndex;
	QPixmap *_imageHandle[AMOUNT_OF_IMAGES];
	QLabel *_statusIcon;
	QTimer *_statusAnimationTimer;

	void loadImages();

protected slots:
	void updateStatusAnimation();

public:
	MariaUIStatus(QMainWindow *parent);
	~MariaUIStatus();
	void setStatus(STATUS_TYPE type);
	STATUS_TYPE getStatus();
	void updateGUI(float rollingX, float rollingY);
};
