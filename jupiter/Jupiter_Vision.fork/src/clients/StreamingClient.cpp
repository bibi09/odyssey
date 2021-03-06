#include "StreamingClient.h"
#include "../gui/VideoTexture.h"
#include "../picture/SocketPicture.h"
#include <assert.h>
#include <pthread.h>
#include <sys/select.h>
#include <iostream>

using namespace std ;
using namespace es ;

StreamingClient* StreamingClient::Instance = 0 ;


    #ifdef WIFI_NETWORK
        StreamingClient::StreamingClient() : Client("192.168.100.1", 9587) {
    #else
        StreamingClient::StreamingClient() : Client("192.168.0.2", 9587) {
    #endif
    }

StreamingClient::~StreamingClient() throw() {}

StreamingClient* StreamingClient::getInstance() {
    if (Instance == 0)
        Instance = new StreamingClient() ;
    return Instance ;
}


void* StreamingClient::manageThread(void* param) {
    Instance -> run() ;
    return 0 ;
}

void StreamingClient::createThread() {
    pthread_t clientThread ;

    assert(pthread_create(&clientThread, 0, &StreamingClient::manageThread, 0) == 0) ;
//    pthread_join(clientThread, 0) ;
}

void StreamingClient::run() {
    SocketPicture pic ;
    int socketfd = m_socket.getSocket() ;
    uint8_t bufferFrame[65567] ;

    while (m_isRunning) {
        // Receive a frame from Polyphemus
        int32_t bufferSize = 0 ;
        m_socket.recv(socketfd, &bufferSize, sizeof(bufferSize), MSG_WAITALL) ;
        bufferSize = ntohl(bufferSize) ;
        int received = m_socket.recv(socketfd, bufferFrame, bufferSize, MSG_WAITALL) ;

        // Limit to handle only frames that have contents (sometimes there are
        // missing data...)
        if ((bufferSize > 256) && (received == bufferSize)) {
            // Decode the compressed frame
            uint8_t* rawBuffer = 0 ;
            if (m_decoder.setFrame(bufferFrame, bufferSize)) {
                int rawSize = m_decoder.getRawData(rawBuffer) ;
                if (rawSize > 0) {
                    int width = m_decoder.getWidth() ;
                    int height = m_decoder.getHeight() ;

                    // Set the data for display
                    VideoTexture::getInstance() -> update(rawBuffer, width, height) ;

                    // Set the data used for image processing
                    pic.setData(rawBuffer, rawSize, width, height) ;
                    pic.display() ;
                    m_decoder.freeBuffer(rawBuffer) ;
                }
            }
        }
    }
}
