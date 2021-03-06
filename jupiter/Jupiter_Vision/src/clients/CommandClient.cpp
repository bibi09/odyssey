#include "CommandClient.h"
#include <assert.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include <iostream>

using namespace std ;
using namespace es ;

CommandClient* CommandClient::Instance = 0 ;
std::string CommandClient::IP = "192.168.100.1" ;
unsigned short CommandClient::Port = 1234 ;


CommandClient::CommandClient() : Client(CommandClient::IP, CommandClient::Port) {
    assert(SecurityClientAuthenticateServer(m_socket.getSocket())) ;
    cout << "Connected to Mercury" << endl ;

    m_isLEDOn = false ;
    sleep(5) ;
}

CommandClient::~CommandClient() throw() {
    sendMoveCommand(ROBOT_COMMAND_STOP) ;
    SecurityClientQuit() ;
}

CommandClient* CommandClient::getInstance() {
    if (Instance == 0)
        Instance = new CommandClient() ;
    return Instance ;
}

bool CommandClient::configure(const std::string& ip, const unsigned short port) {
	if (Instance == 0) {
		CommandClient::IP = ip ;
		CommandClient::Port = port ;
		return true ;
		
	}

	return false ;
}


void* CommandClient::manageThread(void* param) {
    return 0 ;
}

void CommandClient::createThread() {}

void CommandClient::run() {}


void CommandClient::sendMoveCommand(TRobotCommand command) {
    static int socketServer = m_socket.getSocket() ;

    if ((command >= ROBOT_COMMAND_STOP) && (command <= ROBOT_COMMAND_RIGHT)) {
        #ifdef DEBUG_JUPITER
            printf("Sending command %i\n", command) ;
        #endif

        SecurityClientSendRobotCommand(socketServer, command) ;
    }
}

void CommandClient::sendLEDCommand() {
    m_isLEDOn = !m_isLEDOn ;
    if (m_isLEDOn)
        SecurityClientSendRobotCommand(m_socket.getSocket(), ROBOT_COMMAND_LED_ON) ;
    else
        SecurityClientSendRobotCommand(m_socket.getSocket(), ROBOT_COMMAND_LED_OFF) ;
}

char CommandClient::getBatteryLoadRate() {
    int loadRate ;
    SecurityClientSendRobotCommand(m_socket.getSocket(), ROBOT_COMMAND_READ_BATTERY_VOLTAGE) ;
    SecurityClientReceiveRobotData(m_socket.getSocket(), &loadRate) ;
    cout << dec << "Battery: " << loadRate << "%" << endl ;
    return (char) loadRate ;
}
