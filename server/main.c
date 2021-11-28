#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>

#include <cjson/cJSON.h>

#include <sml/sml_file.h>
#include <sml/sml_transport.h>
#include <sml/sml_value.h>

#include "unit.h"


int serial_port_open(const char* device) {
	int bits;
	struct termios config;
	memset(&config, 0, sizeof(config));

	if (!strcmp(device, "-"))
		return 0; // read stdin when "-" is given for the device

#ifdef O_NONBLOCK
	int fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
#else
	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
#endif
	if (fd < 0) {
		fprintf(stderr, "error: open(%s): %s\n", device, strerror(errno));
		return -1;
	}

	// set RTS
	ioctl(fd, TIOCMGET, &bits);
	bits |= TIOCM_RTS;
	ioctl(fd, TIOCMSET, &bits);

	tcgetattr(fd, &config);

	// set 8-N-1
	config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR
			| ICRNL | IXON);
	config.c_oflag &= ~OPOST;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	config.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB);
	config.c_cflag |= CS8;

	// set speed to 9600 baud
	cfsetispeed(&config, B9600);
	cfsetospeed(&config, B9600);

	tcsetattr(fd, TCSANOW, &config);
	return fd;
}



void transport_receiver(unsigned char *buffer, size_t buffer_len) {
	int i;
	cJSON *ptDataSet;
	cJSON *ptEntry;
	char *pcJSON;
	u8 ucType;
	char acID[1024];
	// the buffer contains the whole message, with transport escape sequences.
	// these escape sequences are stripped here.
	sml_file *file = sml_file_parse(buffer + 8, buffer_len - 16);
	// the sml file is parsed now

	for( i=0; i<file->messages_len; i++ )
	{
		sml_message *message = file->messages[i];
		if( *message->message_body->tag==SML_MESSAGE_GET_LIST_RESPONSE )
		{
			/* Create a new JSON object. */
			ptDataSet = cJSON_CreateArray();
			if( ptDataSet==NULL )
			{
				fprintf(stderr, "Failed to create a JSON object.\n");
			}
			else
			{
				sml_list *entry;
				sml_get_list_response *body;
				body = (sml_get_list_response *) message->message_body->data;
				for( entry=body->val_list; entry!=NULL; entry=entry->next )
				{
					if( !entry->value )
					{
						/* Do not crash on null value. */
						fprintf(stderr, "Error in data stream. entry->value should not be NULL. Skipping this.\n");
					}
					else
					{
						ptEntry = cJSON_CreateObject();
						if( ptEntry==NULL )
						{
							fprintf(stderr, "Failed to create a JSON object for the entry.\n");
						}
						else
						{
							/* Set the ID of the entry. */
							snprintf(acID, sizeof(acID), "%d-%d:%d.%d.%d*%d",
								entry->obj_name->str[0], entry->obj_name->str[1],
								entry->obj_name->str[2], entry->obj_name->str[3],
								entry->obj_name->str[4], entry->obj_name->str[5]);
							cJSON_AddStringToObject(ptEntry, "id", acID);

							ucType = entry->value->type;
							if( ucType==SML_TYPE_OCTET_STRING )
							{
								char *str;
								cJSON_AddStringToObject(ptEntry, "type", "octetstring");
								cJSON_AddStringToObject(ptEntry, "value", sml_value_to_strhex(entry->value, &str, true));
								free(str);
							}
							else if( ucType==SML_TYPE_BOOLEAN )
							{
                                                                int iValue;
								cJSON_AddStringToObject(ptEntry, "type", "boolean");
                                                                iValue = *(entry->value->data.boolean);
								cJSON_AddBoolToObject(ptEntry, "value", iValue);
							}
							else if(((ucType&SML_TYPE_FIELD)==SML_TYPE_INTEGER) ||
								((ucType&SML_TYPE_FIELD)==SML_TYPE_UNSIGNED))
							{
								cJSON_AddStringToObject(ptEntry, "type", "number");

								double value = sml_value_to_double(entry->value);
								int scaler = (entry->scaler) ? *entry->scaler : 0;
								int prec = -scaler;
								if (prec < 0)
								{
									prec = 0;
								}
								value = value * pow(10, scaler);

								cJSON_AddNumberToObject(ptEntry, "value", value);
								cJSON_AddNumberToObject(ptEntry, "precision", prec);

								const char *unit = NULL;
								if( entry->unit &&  // do not crash on null (unit is optional)
								   (unit = dlms_get_unit((unsigned char) *entry->unit)) != NULL)
								{
									cJSON_AddStringToObject(ptEntry, "unit", unit);
								}
							}

							cJSON_AddItemToArray(ptDataSet, ptEntry);
						}
					}
				}

				/* Dump the data set to a JSON string. */
				pcJSON = cJSON_Print(ptDataSet);
				if( pcJSON==NULL )
				{
					fprintf(stderr, "Failed to dump the data set.\n");
				}
				else
				{
					printf("Data: ***%s***\n", pcJSON);
					free(pcJSON);
				}

				cJSON_Delete(ptDataSet);
			}
		}
	}

	// free the malloc'd memory
	sml_file_free(file);
}


int main(void)
{
	cJSON *ptDataSet;
	cJSON *ptItem;
	char *pcJSON;


	printf("Using cJSON %s\n", cJSON_Version());

	/* Open serial port. */
	int fd = serial_port_open("/dev/ttyUSB1");
	if (fd < 0) {
		// error message is printed by serial_port_open()
		exit(1);
	}

	// listen on the serial device, this call is blocking.
	sml_transport_listen(fd, &transport_receiver);
	close(fd);
}
