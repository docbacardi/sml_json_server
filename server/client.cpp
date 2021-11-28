#include <stdio.h>
#include "zmq.h"

int main (int argc, char *argv [])
{
	int iZmqMajor, iZmqMinor, iZmqPatch;
	void *ptZmqContext;
	void *ptZmqSubscriber;
	int iResult;
	int iExitCode;
	int iMessageCounter;
	char acBuffer[65536];


	iExitCode = 0;
	iMessageCounter = 0;

	zmq_version(&iZmqMajor, &iZmqMinor, &iZmqPatch);
	printf ("Using ØMQ v%d.%d.%d\n", iZmqMajor, iZmqMinor, iZmqPatch);

	ptZmqContext = zmq_ctx_new();
	ptZmqSubscriber = zmq_socket(ptZmqContext, ZMQ_SUB);
	iResult = zmq_connect(ptZmqSubscriber, "tcp://localhost:5556");
	if( iResult!=0 )
	{
		fprintf(stderr, "Failed to connect the ZQM socket.\n");
		iExitCode = 1;
	}
	else
	{
		printf("Connected...\n");

		/* Subscribe to all messages. */
		iResult = zmq_setsockopt(ptZmqSubscriber, ZMQ_SUBSCRIBE, NULL, 0);
		if( iResult!=0 )
		{
			fprintf(stderr, "Failed to subscribe.\n");
			iExitCode = 1;
		}
		else
		{
			while( iExitCode==0 )
			{
				iResult = zmq_recv(ptZmqSubscriber, acBuffer, sizeof(acBuffer), 0);
				printf("Received with result %d.\n", iResult);
				if( iResult<0 )
				{
					fprintf(stderr, "Failed to received a message.\n");
					iExitCode = 1;
				}
				else if( iResult==0 )
				{
					fprintf(stderr, "Received an empty message.\n");
				}
				else if( iResult>sizeof(acBuffer) )
				{
					fprintf(stderr, "The received message is too large for the buffer!\n");
					iExitCode = 1;
				}
				else
				{
					++iMessageCounter;
					/* Terminate the message. */
					acBuffer[iResult-1] = 0;
					printf("Message %d: ***%s***\n", iMessageCounter, acBuffer);
				}
			}
		}
	}

	zmq_close(ptZmqSubscriber);
	zmq_ctx_destroy(ptZmqContext);
	return iExitCode;
}
