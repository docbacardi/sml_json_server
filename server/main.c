#include <stdlib.h>
#include <stdio.h>

#include <cjson/cJSON.h>

int main(void)
{
    cJSON *ptDataSet;
    cJSON *ptItem;
    char *pcJSON;


    printf("Using cJSON %s\n", cJSON_Version());

    /* Create a new JSON object. */
    ptDataSet = cJSON_CreateObject();
    if( ptDataSet==NULL )
    {
        fprintf(stderr, "Failed to create a JSON object.\n");
    }
    else
    {
        /* Add all data... */
        ptItem = cJSON_AddStringToObject(ptDataSet, "1-1", "0123456789ABCDEF");
        if( ptItem!=NULL )
        {
            ptItem = cJSON_AddNumberToObject(ptDataSet, "1-2", 1234);
            if( ptItem!=NULL )
            {
                ptItem = cJSON_AddBoolToObject(ptDataSet, "1-3", 0);
                if( ptItem!=NULL )
                {
                    ptItem = cJSON_AddBoolToObject(ptDataSet, "1-4", 1);
                    if( ptItem!=NULL )
                    {
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
                    }
                }
            }
        }

        cJSON_Delete(ptDataSet);
    }
}
