#pragma once

#include "stdafx.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

class TelnetClient {
	asio::io_service& mIOService;
	tcp::socket mSocket;
	asio::streambuf mReceiveBuff;

	std::string mHost = {};
	int mPort = 0;

public:
	TelnetClient(asio::io_service& io_service)
		: mIOService(io_service),
		mSocket(io_service)
	{}

	void SetHOST(std::string pHost) {
		mHost = pHost;
	}

	void SetPORT(int pPort) {
		mPort = pPort;
	}


	void Connect()
	{
		mSocket.async_connect(
			tcp::endpoint(asio::ip::address::from_string(mHost), mPort),
			boost::bind(&TelnetClient::onConnect, this, asio::placeholders::error));
	}

	void onConnect(const boost::system::error_code& error)
	{
		if (error) {
			std::cout << "connect failed : " << error.message() << std::endl;
		}
		else {
			std::cout << "connected" << std::endl;
			//startReceive();
		}
	}

	// メッセージ受信
	void StartReceive()
	{
		boost::asio::async_read(
			mSocket,
			mReceiveBuff,
			asio::transfer_exactly(100),//asio::transfer_all(),
			boost::bind(&TelnetClient::onReceive, this,
				asio::placeholders::error, asio::placeholders::bytes_transferred));
	}


	// 受信完了
	// error : エラー情報
	// bytes_transferred : 受信したバイト数
	void onReceive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error && error != boost::asio::error::eof) {
			std::cout << "receive failed: " << error.message() << std::endl;
		}
		else {
			const char* data = asio::buffer_cast<const char*>(mReceiveBuff.data());
			std::cout << data;

			mReceiveBuff.consume(mReceiveBuff.size());
		}

		StartReceive();//再起呼び出し

	}



};