#include "QTLiveUSBkey.h"

QTLiveUSBkey::QTLiveUSBkey(void*parent)/*(QObject *parent)
	: QObject(parent)*/
{
	m_stop = false;
	m_keyHandle = false;
}

QTLiveUSBkey::~QTLiveUSBkey()
{
}

bool QTLiveUSBkey::onLiveCheckKey(void* hand)
{
	/*
	bool m_bled = false;
	m_keyHandle = hand;
	while (!m_stop)
	{
		if (R1_S_SUCCESS == R1_LEDControl(m_keyHandle, !m_bled))
		{
			m_bled = !m_bled;
			QThread::sleep(1);
		}
		else
		{
			emit ERRORCONNECT();
			return false;
		}
	}
	*/
	return true;
}
