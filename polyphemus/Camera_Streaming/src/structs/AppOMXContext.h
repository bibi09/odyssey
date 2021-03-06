/**
 * @file    AppOMXContext.h
 * @brief   The AppOMXContext contains all the required data to configure
 *          the Raspberry Pi camera module and encoder to compress video stream
 *          through the VideoCore IV chipset.
 * @author  Denis CARLUS
 * @version 1.0     02/01/2014
 */

#ifndef __POLYPHEMUS__IMAGE_APPOMX_CONTEXT__
#define __POLYPHEMUS__IMAGE_APPOMX_CONTEXT__

#include <stdio.h>
#include <IL/OMX_Core.h>
#include <bcm_host.h>
#include <interface/vcos/vcos_semaphore.h>
#include <interface/vmcs_host/vchost.h>

#include "StreamingServer.h"
#include "handlers/BasicOMXHandler.h"
#include "handlers/BufferOMXHandler.h"


/** @brief	Context of the application for OpenMAX. */
typedef struct AppOMXContext {
    /** DATA **/
	BufferOMXHandler    camera ;
	BufferOMXHandler    encoder ;
	BasicOMXHandler     nullSink ;
	VCOS_SEMAPHORE_T    handlerLock ;
	StreamingServer     streamingServer ;
    int                 clientSocket ;
    char                flushed ;

    /** METHODS **/
    // Configuration
    void                (*configureCamera)                  (struct AppOMXContext*) ;
    void                (*configureEncoder)                 (struct AppOMXContext*) ;

    // Getters
    OMX_HANDLETYPE*     (*getCamera)                        (struct AppOMXContext*) ;
    OMX_HANDLETYPE*     (*getEncoder)                       (struct AppOMXContext*) ;
    OMX_HANDLETYPE*     (*getNullSink)                      (struct AppOMXContext*) ;
    BufferOMXHandler*   (*getCameraHandler)                 (struct AppOMXContext*) ;
    BufferOMXHandler*   (*getEncoderHandler)                (struct AppOMXContext*) ;
    VCOS_SEMAPHORE_T*   (*getHandlerLock)                   (struct AppOMXContext*) ;
    char                (*isFlushed)                        (struct AppOMXContext*) ;

    // Setters
    void                (*setCameraReady)                   (struct AppOMXContext*) ;
    void                (*setEncoderOutputBufferAvailable)  (struct AppOMXContext*) ;
    void                (*flush)                            (struct AppOMXContext*) ;
    void                (*unflush)                          (struct AppOMXContext*) ;

    // Utilities
    void                (*capture)                          (struct AppOMXContext*) ;
} AppOMXContext ;


/** @brief  Create a new AppOMXContext. */
AppOMXContext AppOMXContext_Construct(char* ip) ;

/** @brief  memset() on an AppOMXContext with the right initialization*/
void* AppOMXContext_Memset(AppOMXContext* self, int c, size_t n) ;

#endif
