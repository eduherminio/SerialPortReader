#include "SerialCommunication.h"
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
const std::string defaultPort = "COM4";
const int default_baudrate = CBR_9600;

void PrintInfo();
void ParseArgs(int argc, char** argv, std::string& port, int& baudRate);
void PrintConnectionInfo(std::string port, int baudrate);
std::string ParsePort(char* argv);
int ParseBaudrate(char* argv);

int main(int args, char* argv[])
{
	uint8_t incoming_data[MAX_BUFFER_SIZE];
	std::stringstream ss;

	std::string parsedPort;
	int baud_rate = CBR_9600;

	ParseArgs(args, argv, parsedPort, baud_rate);

	std::string chosen_port = parsedPort.empty() ? defaultPort : parsedPort;
	PrintConnectionInfo(chosen_port, baud_rate);

	LPCSTR port = chosen_port.c_str();

	SerialPort serialDevice(port, baud_rate);

	if (serialDevice.isConnected())
	{
		std::cout << "Successfully established connection!" << std::endl; //check connection
	}

	while (serialDevice.isConnected())
	{
		int bytes_read = serialDevice.read(incoming_data, MAX_BUFFER_SIZE);

		for (uint8_t i = 0; i < bytes_read; i++)
		{
			ss << std::hex << incoming_data[i]; //Print in hexadecimal packs
		}
		ss << std::endl;

		if (bytes_read > 0)
		{
			std::cout << ss.str();
		}

		ss.str(std::string()); //empty string stream
		Sleep(100);
	}

	exit(EXIT_SUCCESS);
}

void ParseArgs(int argc, char** argv, std::string& port, int& baudRate)
{
	if (argc == 1)
	{
		PrintInfo();
	}
	if (argc > 1)
	{
		port = ParsePort(argv[1]);
	}
	if (argc > 2)
	{
		baudRate = ParseBaudrate(argv[2]);
	}
}

std::string ParsePort(char* argv)
{
	std::string port = std::string(argv);

	std::size_t found = port.find("COM");

	return found != std::string::npos
		? port.substr(found, 4)
		: std::string();
}

int ParseBaudrate(char* argv)
{
	std::string baudrate_str = std::string(argv);

	return atoi(baudrate_str.c_str());
}

void PrintInfo()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "arg1: port\t(default: COM4)" << std::endl;
	std::cout << "arg2: baud rate\t(default: 9600)" << std::endl << std::endl;
}

void PrintConnectionInfo(std::string port, int baudrate)
{
	std::cout << "Trying to read from port ";
	std::cout << port;
	std::cout << " at baud rate " << baudrate << std::endl << std::endl<<std::endl;
}