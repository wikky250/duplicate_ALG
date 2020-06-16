#pragma once

//#include "Rockey1_API.h"
#include <QThread>
#include <QObject>

class QTLiveUSBkey : public QObject
{
	Q_OBJECT
public:
	signals :
			void ERRORCONNECT();
private:
	bool m_stop;
	void* m_keyHandle;

public:
	QTLiveUSBkey(void* parent);
	~QTLiveUSBkey();
	public slots:
	bool onLiveCheckKey(void* hand);
};
