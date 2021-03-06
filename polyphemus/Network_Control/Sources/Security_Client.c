/** @file Security_Client.c
 * @see Security_Client.h for description.
 * @author Adrien RICCIARDI
 */
#include <gmp.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Crypto/Elliptic_Curves.h"
#include "Crypto/Point.h"
#include "Crypto/Utils.h"
#include "Crypto/AES.h"
#include "Crypto/DSA.h"
#include "Log.h"
#include "Robot.h"
#include "Security.h"
#include "Security_Client.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
/** Anti replay counter used during asymetric cryptography steps. */
static int Counter;

/** This machine public key. */
static TPoint Point_Own_Public_Key;
/** This machine private key. */
static mpz_t Private_Key;

/** The server public key. */
static TPoint Point_Server_Public_Key;

/** The elliptic curve used for all asymetric cryptography. */
static TEllipticCurve Elliptic_Curve;

/** AES OpenSSL context used to send robot commands. */
static EVP_CIPHER_CTX *Pointer_AES_Context_Output_Commands;
/** AES OpenSSL context used to receive robot data. */
static EVP_CIPHER_CTX *Pointer_AES_Context_Input_Data;
/** AES OpenSSL context used to receive video. */
static EVP_CIPHER_CTX *Pointer_AES_Context_Input_Video;

/** AES secret key. */
static unsigned char AES_Key[AES_KEY_SIZE_BYTES];
/** AES initialization vector (IV). */
static unsigned char AES_Initialization_Vector[AES_BLOCK_SIZE_BYTES];

/** Pre allocated video buffer in data segment to avoid allocating too much data on the stack. */
static unsigned char Encrypted_Video_Buffer[SECURITY_VIDEO_BUFFER_MAXIMUM_SIZE_BYTES];

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
int SecurityClientInit(char *String_Keys_File_Path, char *String_Counter_File_Path)
{
	UtilsInitializeRandomGenerator();
	Pointer_AES_Context_Output_Commands = AESCreate();
	Pointer_AES_Context_Input_Data = AESCreate();
	Pointer_AES_Context_Input_Video = AESCreate();
	return SecurityInit(String_Keys_File_Path, String_Counter_File_Path, 1, &Elliptic_Curve, &Point_Own_Public_Key, Private_Key, &Point_Server_Public_Key, &Counter);
}

int SecurityClientAuthenticateServer(int Socket_Server)
{
	TSecurityMessageAuthentication Message;
	mpz_t Random_Number, Received_Signature_Number_U, Received_Signature_Number_V;
	int Return_Value = 0, Received_Counter;
	TPoint Point_Received_Key_Part;

	// Initialize variables
	mpz_init(Random_Number);
	mpz_init(Received_Signature_Number_U);
	mpz_init(Received_Signature_Number_V);
	PointCreate(0, 0, &Point_Received_Key_Part);

	//---------- First step : send key part message ----------
	// Fill message payload
	Counter++;
	SecurityFillAuthenticationMessage(Counter, &Elliptic_Curve, Random_Number, &Message);

	// Sign message
	SecuritySignAuthenticationMessage(&Elliptic_Curve, Private_Key, &Message);

	// Send message to server
	if (!SecuritySendAuthenticationMessage(Socket_Server, &Message)) goto Exit;

	//---------- Second step : get server key part ----------
	// Receive server message
	if (!SecurityReceiveAuthenticationMessage(Socket_Server, &Message, &Received_Counter, &Point_Received_Key_Part, Received_Signature_Number_U, Received_Signature_Number_V))
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientAuthenticateServer] Error : can't receive data from server socket.");
		goto Exit;
	}

	// Check message correctness
	if (!SecurityCheckAuthenticationMessageSignature(&Message, &Elliptic_Curve, &Point_Server_Public_Key, Received_Signature_Number_U, Received_Signature_Number_V))
	{
		Log(LOG_ERR, "[Security_Clent.SecurityClientAuthenticateServer] Error : server's message has a bad signature.");
		goto Exit;
	}

	// Check anti replay counter
	Log(LOG_DEBUG, "[Security_Client.SecurityClientAuthenticateServer] Current counter = %d, Received counter = %d.", Counter, Received_Counter);
	if (Received_Counter <= Counter)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientAuthenticateServer] Error : received counter is too small.");
		goto Exit;
	}
	Counter = Received_Counter;

	// Get AES IV from signature U number
	//memcpy(AES_Initialization_Vector, Message.Signature_Number_U, sizeof(AES_Initialization_Vector));
	memcpy(AES_Initialization_Vector, Message.Signature_Number_U, sizeof(AES_Initialization_Vector) / 2);
	memset(AES_Initialization_Vector + (sizeof(AES_Initialization_Vector) / 2), 0, sizeof(AES_Initialization_Vector) / 2); // Set AES counter to zero
	#if LOG_ENABLE == 1
	{
		char String_Initialization_Vector[256] = {0};
		int i;

		for (i = 0; i < sizeof(AES_Initialization_Vector); i++) sprintf(String_Initialization_Vector, "%s%02X ", String_Initialization_Vector, AES_Initialization_Vector[i]);
		Log(LOG_DEBUG, "[Security_Client.SecurityClientAuthenticateServer] AES initialization vector : %s.", String_Initialization_Vector);
	}
	#endif

	//---------- Third step : generate session key ----------
	// Compute Diffie-Hellman shared key
	ECMultiplication(&Elliptic_Curve, &Point_Received_Key_Part, Random_Number, &Point_Received_Key_Part);
	UTILS_MPZ_EXPORT(Point_Received_Key_Part.X, AES_Key);
	#if LOG_ENABLE == 1
	{
		char String_Key[256];

		gmp_sprintf(String_Key, "%Zd", Point_Received_Key_Part.X);
		Log(LOG_DEBUG, "[Security_Client.SecurityClientAuthenticateServer] AES secret key : %s.", String_Key);
	}
	#endif

	//---------- Fourth step : create AES links ----------
	// Robot output commands link
	if (AESCipherInit(Pointer_AES_Context_Output_Commands, AES_Key, AES_Initialization_Vector) == -1)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientAuthenticateServer] Error : can't create AES output link for robot commands.");
		goto Exit;
	}

	// Robot input data link
	if (AESDecipherInit(Pointer_AES_Context_Input_Data, AES_Key, AES_Initialization_Vector) == -1)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientAuthenticateServer] Error : can't create AES input link for robot data.");
		goto Exit;
	}

	// Robot input video link
	if (AESDecipherInit(Pointer_AES_Context_Input_Video, AES_Key, AES_Initialization_Vector) == -1)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientAuthenticateServer] Error : can't create AES input link for robot video.");
		goto Exit;
	}

	SecuritySaveCounters(&Counter, 1);

	// All successful
	Return_Value = 1;

Exit:
	mpz_clear(Random_Number);
	mpz_clear(Received_Signature_Number_U);
	mpz_clear(Received_Signature_Number_V);
	PointFree(&Point_Received_Key_Part);
	return Return_Value;
}

int SecurityClientSendRobotCommand(int Socket_Server, TRobotCommand Command)
{
	return SecuritySendRobotControlMessage(Socket_Server, Pointer_AES_Context_Output_Commands, (int) Command);
}

int SecurityClientReceiveRobotData(int Socket_Server, int *Pointer_Data)
{
	return SecurityReceiveRobotControlMessage(Socket_Server, Pointer_AES_Context_Input_Data, Pointer_Data);
}

int SecurityClientReceiveVideoBuffer(int Socket_Server, void *Pointer_Received_Video_Buffer, int *Pointer_Received_Video_Buffer_Size)
{
	TSecurityVideoMessageHeader Header;
	unsigned int Temp_Int;
	int Bytes_To_Receive_Count, Received_Buffer_Size, Decrypted_Bytes_Count;

	// Receive encrypted header
	Received_Buffer_Size = recv(Socket_Server, Encrypted_Video_Buffer, AES_BLOCK_SIZE_BYTES, MSG_WAITALL);
	if (Received_Buffer_Size != AES_BLOCK_SIZE_BYTES)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientReceiveVideoBuffer] Error : could not receive encrypted header from socket (received %d/%d bytes).", Received_Buffer_Size, AES_BLOCK_SIZE_BYTES);
		return 0;
	}

	// Decipher header
	if (EVP_DecryptUpdate(Pointer_AES_Context_Input_Video, (unsigned char *) &Header, &Decrypted_Bytes_Count, Encrypted_Video_Buffer, AES_BLOCK_SIZE_BYTES) == 0)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientReceiveVideoBuffer] Error : could not decipher header.");
		return 0;
	}
	// Check decrypted size
	if (Decrypted_Bytes_Count != AES_BLOCK_SIZE_BYTES)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientReceiveVideoBuffer] Error : only %d/%d bytes of the header were deciphered.", Decrypted_Bytes_Count, AES_BLOCK_SIZE_BYTES);
		return 0;
	}

	// Convert header to local machine endianess
	Temp_Int = ntohl(Header.Payload_Size);
	Header.Payload_Size = Temp_Int;
	Temp_Int = ntohl(Header.Padding_Size);
	Header.Padding_Size = Temp_Int;

	// Receive video buffer and padding
	Bytes_To_Receive_Count = Header.Payload_Size + Header.Padding_Size;
	Received_Buffer_Size = recv(Socket_Server, Encrypted_Video_Buffer, Bytes_To_Receive_Count, MSG_WAITALL); // Need to wait for all the data
	if (Received_Buffer_Size != Bytes_To_Receive_Count)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientReceiveVideoBuffer] Error : could not receive encrypted video data and padding from socket (received %d/%d bytes)", Received_Buffer_Size, Bytes_To_Receive_Count);
		return 0;
	}

	// Decipher video data
	if (EVP_DecryptUpdate(Pointer_AES_Context_Input_Video, Pointer_Received_Video_Buffer, &Decrypted_Bytes_Count, Encrypted_Video_Buffer, Bytes_To_Receive_Count) == 0)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientReceiveVideoBuffer] Error : could not decipher video and padding.");
		return 0;
	}
	// Check decrypted size
	if (Decrypted_Bytes_Count != Bytes_To_Receive_Count)
	{
		Log(LOG_ERR, "[Security_Client.SecurityClientReceiveVideoBuffer] Error : only %d/%d bytes of the video and padding were deciphered.", Decrypted_Bytes_Count, Bytes_To_Receive_Count);
		return 0;
	}

	*Pointer_Received_Video_Buffer_Size = Header.Payload_Size;
	return 1;
}

void SecurityClientQuit(void)
{
	UtilsFreeRandomGenerator();
	AESCleanUp(Pointer_AES_Context_Output_Commands);
	AESCleanUp(Pointer_AES_Context_Input_Data);
	AESCleanUp(Pointer_AES_Context_Input_Video);
}
