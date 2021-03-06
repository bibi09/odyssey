#ifndef __JUPITER__STREAMING_CLIENT__
#define __JUPITER__STREAMING_CLIENT__

extern "C" {
    #include <Log.h>
    #include <Security_Client.h>
    #include <Robot.h>
}

#include <easysocket/comm.h>
#include <easysocket/tools.h>
#include "../picture/H264Decoder.h"

class StreamingClient : public es::Client, private NoCopy {
    private:
        /** @brief  Unique instance of the streaming client. */
        static StreamingClient* Instance ;

		/** @brief	IP of the server to connect to. */
		static std::string IP ;
		/** @brief	Port of the server to connect to. */
		static unsigned short Port ;
		

        /** @brief  Decoder used by the client to get uncompressed picture data. */
        H264Decoder m_decoder ;


        /** @brief  Create the StreamingClient. */
        StreamingClient() ;

        /** @brief  Destroy the StreamingClient. */
        virtual ~StreamingClient() throw() ;


    public:
        /** @brief  Get the StreamingClient instance. */
        static StreamingClient* getInstance() ;

		/**
		 * @brief	Set the IP and Port of the server to connect to.
		 * @param	ip		IP of the server.
		 * @param	port	Port of the server.
		 * @return	TRUE of successful configuration, FALSE else.
		 */
		static bool configure(const std::string& ip = IP,
							  const unsigned short port = Port) ;


    protected:
        /** @brief  Intermediate function to easily create a thread. */
        static void* manageThread(void* param) ;

        /**
         * @brief  Function to implement in the specific client to create
         *         the background client thread.
         */
        virtual void createThread() ;

        /** @brief  Run the client thread. Call stop() to stop it. */
        virtual void run() ;
} ;

#endif
