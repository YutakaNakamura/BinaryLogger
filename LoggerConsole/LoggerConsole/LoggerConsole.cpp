// LoggerConsole.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
//#include <boost/version.hpp>
//#include <boost/lexical_cast.hpp>
#include <string>


#include "TelnetClient.hpp"

int main()
{
	std::locale::global(std::locale("japanese"));


	boost::asio::io_service IOService;
	TelnetClient telnetclient(IOService);

	telnetclient.SetHOST("127.0.0.1");
	telnetclient.SetPORT(19021);

	telnetclient.Connect();
	telnetclient.StartReceive();

	IOService.run();

	system("pause");
}

