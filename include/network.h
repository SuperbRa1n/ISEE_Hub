#ifndef NETWORK_H
#define NETWORK_H


#include <HTTPClient.h>
#include <WiFi.h>
 
class Network
{
private:
	 
public:
	void init(const char* ssid,const char* password);
	void SmartConfig();
	unsigned int getBilibiliFans(String uid);
	unsigned int getWendu(String url);
	unsigned int getTianqi(String url);
	unsigned int getTime(void);
	unsigned int getData(void);
	unsigned int getxinzhiweatherdata(String key, String location);
	void setClock();
};

#endif