#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>

class MariaUIStatus : QWidget {
	Q_OBJECT
public:
	enum STATUS_TYPE {
		OK, INVALID, WAIT, UNKNOWN, NONE
	};

	MariaUIStatus(QMainWindow *qmainWindow);
	~MariaUIStatus();

	void setStatus(STATUS_TYPE type);
	STATUS_TYPE getStatus();
	void updateGUI(QPointF statePosition);

protected slots:
		void updateStatusAnimation();	

private:
	static const int TEXTBOX_X_OFFSET = 30;
	static const int AMOUNT_OF_IMAGES = 6;
	static const int IMAGE_INDEX_OK = 0;
	static const int IMAGE_INDEX_UNKNOWN = 1;

	//Image index assigned for animation must be consequtive.
	static const int IMAGE_INDEX_WAIT_0 = 2;
	static const int IMAGE_INDEX_WAIT_1 = 3;
	static const int IMAGE_INDEX_WAIT_2 = 4;
	static const int IMAGE_INDEX_WAIT_3 = 5;

	//Animation speed in milliseconds.
	static const int ANIMATION_SPEED = 500;

	QMainWindow *_qmainWindow;

	STATUS_TYPE _currentStatus;
	int _statusImageIndex;
	QPixmap *_imageHandle[AMOUNT_OF_IMAGES];
	QLabel *_statusIcon;
	QTimer *_statusAnimationTimer;

	void loadImages();
};
