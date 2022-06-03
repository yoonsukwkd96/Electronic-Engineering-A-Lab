#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"
#include "FreeRTOSConfig.h"

bool xLockLane(char Lane, char Destination);
void xUnlockLane(char Lane, char Destination);

void vNorth(void *pvParameters);
void vSouth(void *pvParameters);
void vWest(void *pvParameters);
void vEast(void *pvParameters);

xQueueHandle NorthIncomingLaneCars;
xQueueHandle SouthIncomingLaneCars;
xQueueHandle WestIncomingLaneCars;
xQueueHandle EastIncomingLaneCars;

int NorthIncomingLaneCarsNumber = 100;
int SouthIncomingLaneCarsNumber = 100;
int WestIncomingLaneCarsNumber = 100;
int EastIncomingLaneCarsNumber = 100;

xSemaphoreHandle WS = 0;
xSemaphoreHandle WE = 0;
xSemaphoreHandle WN = 0;
xSemaphoreHandle WW = 0;
xSemaphoreHandle SE = 0;
xSemaphoreHandle SN = 0;
xSemaphoreHandle SW = 0;
xSemaphoreHandle SS = 0;
xSemaphoreHandle EN = 0;
xSemaphoreHandle EW = 0;
xSemaphoreHandle ES = 0;
xSemaphoreHandle EE = 0;
xSemaphoreHandle NW = 0;
xSemaphoreHandle NS = 0;
xSemaphoreHandle NE = 0;
xSemaphoreHandle NN = 0;

int main(void)
{
    printf("beginning of main()\n");

    NorthIncomingLaneCars = xQueueCreate(NorthIncomingLaneCarsNumber, sizeof(char));
    SouthIncomingLaneCars = xQueueCreate(SouthIncomingLaneCarsNumber, sizeof(char));
    WestIncomingLaneCars = xQueueCreate(WestIncomingLaneCarsNumber, sizeof(char));
    EastIncomingLaneCars = xQueueCreate(EastIncomingLaneCarsNumber, sizeof(char));
    
    char temp;
    int num;
    // order of the incoming directions in the array: W S E N
    char dest[4][3]={{'S','E','N'},{'E','N','W'},{'N','W','S'},{'W','S','E'}};

    printf("just before for loops\n");

    for(int i=0; i<NorthIncomingLaneCarsNumber; i++)
    {
        num=rand()%3;
        xQueueSend(NorthIncomingLaneCars,&dest[0][num], 100);
    }
    printf("for 1");
    for(int i=0; i<SouthIncomingLaneCarsNumber; i++)
    {
        num=rand()%3;
        xQueueSend(SouthIncomingLaneCars,&dest[1][num], 100);
    }
    printf("for 2");
    for(int i=0; i<WestIncomingLaneCarsNumber; i++)
    {
        num=rand()%3;
        xQueueSend(WestIncomingLaneCars,&dest[2][num], 100);
    }
    printf("for 3");
    for(int i=0; i<EastIncomingLaneCarsNumber; i++)
    {
        num=rand()%3;
        xQueueSend(EastIncomingLaneCars,&dest[3][num], 100);
    }

    printf("semaphores\n");

    WS = xSemaphoreCreateMutex();
    WE = xSemaphoreCreateMutex();
    WN = xSemaphoreCreateMutex();
    NS = xSemaphoreCreateMutex();
    NW = xSemaphoreCreateMutex();
    NE = xSemaphoreCreateMutex();
    EN = xSemaphoreCreateMutex();
    EW = xSemaphoreCreateMutex();
    ES = xSemaphoreCreateMutex();
    SN = xSemaphoreCreateMutex();
    SW = xSemaphoreCreateMutex();
    SE = xSemaphoreCreateMutex();

    printf("task creation\n");

    xTaskCreate(&vEast, "From E to NSW", 1024, NULL, 1, NULL);
    xTaskCreate(&vWest, "From W to NSE", 1024, NULL, 1, NULL);
    xTaskCreate(&vSouth, "From S to NEW", 1024, NULL, 1, NULL);
    xTaskCreate(&vNorth, "From N to ESW", 1024, NULL, 1, NULL);

    printf("scheduler\n");

    vTaskStartScheduler();
    
    printf("end of main()\n");

    return 0;
}

bool xLockLane(char Lane, char Destination)
{
    bool taken = false;

    if (Lane == 'W' && Destination == 'S')
    {
        if (uxSemaphoreGetCount(ES)+uxSemaphoreGetCount(NS)==2)
        {
            xSemaphoreTake(ES,100);
            xSemaphoreTake(NS,100);
            taken = true;
        }
    }
    else if (Lane=='W' && Destination=='E')
    {
        if(uxSemaphoreGetCount(SN)+
            uxSemaphoreGetCount(SW)+
            uxSemaphoreGetCount(NS)+
            uxSemaphoreGetCount(NE)==4)
        {
            xSemaphoreTake(SN,100);
            xSemaphoreTake(SW,100);
            xSemaphoreTake(NS,100);
            xSemaphoreTake(NE,100);
            taken = true;
        }
    }
    else if (Lane=='W' && Destination=='N')
    {
        if(uxSemaphoreGetCount(SN)+
            uxSemaphoreGetCount(SW)+
            uxSemaphoreGetCount(EN)+
            uxSemaphoreGetCount(EW)+
            uxSemaphoreGetCount(NS)+
            uxSemaphoreGetCount(NE)==6)
        {
            xSemaphoreTake(SN,100);
            xSemaphoreTake(SW,100);
            xSemaphoreTake(EN,100);
            xSemaphoreTake(EW,100);
            xSemaphoreTake(NS,100);
            xSemaphoreTake(NE,100);
            taken = true;
        }
    }
    else if (Lane=='S' && Destination=='E')
    {
        if(uxSemaphoreGetCount(WE)+uxSemaphoreGetCount(NE)==2)
        {
            xSemaphoreTake(WE,100);
            xSemaphoreTake(NE,100);
            taken = true;
        }
    }
    else if (Lane=='S' && Destination=='N')
    {
        if(uxSemaphoreGetCount(WE)+
            uxSemaphoreGetCount(WN)+
            uxSemaphoreGetCount(ES)+
            uxSemaphoreGetCount(EW)+
            uxSemaphoreGetCount(EN)+
            uxSemaphoreGetCount(NE)==6)
        {
            xSemaphoreTake(WE,100);
            xSemaphoreTake(WN,100);
            xSemaphoreTake(ES,100);
            xSemaphoreTake(EW,100);
            xSemaphoreTake(EN,100);
            xSemaphoreTake(NE,100);
            taken = true;
        }
    }
    else if (Lane=='S' && Destination=='W')
    {
        if(uxSemaphoreGetCount(WE)+
            uxSemaphoreGetCount(WN)+
            uxSemaphoreGetCount(ES)+
            uxSemaphoreGetCount(EW)+
            uxSemaphoreGetCount(NS)+
            uxSemaphoreGetCount(NW)==6)
        {
            xSemaphoreTake(WE,100);
            xSemaphoreTake(WN,100);
            xSemaphoreTake(ES,100);
            xSemaphoreTake(EW,100);
            xSemaphoreTake(NS,100);
            xSemaphoreTake(NW,100);
            taken = true;
        }
    }
    else if (Lane=='E' && Destination=='N')
    {
        if(uxSemaphoreGetCount(WN)+
            uxSemaphoreGetCount(SN)==2)
        {
            xSemaphoreTake(WN,100);
            xSemaphoreTake(SN,100);
            taken = true;
        }
    }
    else if (Lane=='E' && Destination=='W')
    {
        if(uxSemaphoreGetCount(WN)+
            uxSemaphoreGetCount(SN)+
            uxSemaphoreGetCount(SW)+
            uxSemaphoreGetCount(NS)+
            uxSemaphoreGetCount(NE)+
            uxSemaphoreGetCount(NW)==6)
        {
            xSemaphoreTake(WN,100);
            xSemaphoreTake(SN,100);
            xSemaphoreTake(SW,100);
            xSemaphoreTake(NS,100);
            xSemaphoreTake(NE,100);
            xSemaphoreTake(NW,100);
            taken = true;
        }
    }
    else if (Lane=='E' && Destination=='S')
    {
        if(uxSemaphoreGetCount(WS)+
            uxSemaphoreGetCount(WE)+
            uxSemaphoreGetCount(SW)+
            uxSemaphoreGetCount(SN)+
            uxSemaphoreGetCount(NS)+
            uxSemaphoreGetCount(NE)==6)
        {
            xSemaphoreTake(WS,100);
            xSemaphoreTake(WE,100);
            xSemaphoreTake(SW,100);
            xSemaphoreTake(SN,100);
            xSemaphoreTake(NS,100);
            xSemaphoreTake(NE,100);
            taken = true;
        }
    }
    else if (Lane=='N' && Destination=='W')
    {
        if(uxSemaphoreGetCount(SW)+
            uxSemaphoreGetCount(EW)==2)
        {
            xSemaphoreTake(SW,100);
            xSemaphoreTake(EW,100);
            taken = true;
        }
    }
    else if (Lane=='N' && Destination=='S')
    {
        if(uxSemaphoreGetCount(WS)+
            uxSemaphoreGetCount(WE)+
            uxSemaphoreGetCount(WN)+
            uxSemaphoreGetCount(SW)+
            uxSemaphoreGetCount(ES)+
            uxSemaphoreGetCount(EW)==6)
        {
            xSemaphoreTake(WS,100);
            xSemaphoreTake(WE,100);
            xSemaphoreTake(WN,100);
            xSemaphoreTake(SW,100);
            xSemaphoreTake(ES,100);
            xSemaphoreTake(EW,100);
            taken = true;
        }
    }
    else if (Lane=='N' && Destination=='E')
    {
        if(uxSemaphoreGetCount(WN)+
            uxSemaphoreGetCount(WE)+
            uxSemaphoreGetCount(SE)+
            uxSemaphoreGetCount(SN)+
            uxSemaphoreGetCount(ES)+
            uxSemaphoreGetCount(EW)==6)
        {
            xSemaphoreTake(WN,100);
            xSemaphoreTake(WE,100);
            xSemaphoreTake(SE,100);
            xSemaphoreTake(SN,100);
            xSemaphoreTake(ES,100);
            xSemaphoreTake(EW,100);
            taken = true;
        }
    }
    return taken;
}

void xUnlockLane(char Lane, char Destination)
{
    if(Lane=='W' && Destination=='S')
    {
        xSemaphoreGive(ES);
        xSemaphoreGive(NS);
    }
    else if(Lane=='W' && Destination=='E')
    {
        xSemaphoreGive(SN);
        xSemaphoreGive(SW);
        xSemaphoreGive(NS);
        xSemaphoreGive(NE);
    }
    else if(Lane=='W' && Destination=='N')
    {
        xSemaphoreGive(SN);
        xSemaphoreGive(SW);
        xSemaphoreGive(EN);
        xSemaphoreGive(EW);
        xSemaphoreGive(NS);
        xSemaphoreGive(NE);
    }
    else if(Lane=='S' && Destination=='E')
    {
        xSemaphoreGive(WE);
        xSemaphoreGive(NE);
    }
    else if(Lane=='S' && Destination=='N')
    {
            xSemaphoreGive(WE);
            xSemaphoreGive(WN);
            xSemaphoreGive(ES);
            xSemaphoreGive(EW);
            xSemaphoreGive(EN);
            xSemaphoreGive(NE);
    }
    else if(Lane=='S' && Destination=='W')
    {
            xSemaphoreGive(WE);
            xSemaphoreGive(WN);
            xSemaphoreGive(ES);
            xSemaphoreGive(EW);
            xSemaphoreGive(NS);
            xSemaphoreGive(NW);
    }
    else if(Lane=='E' && Destination=='N')
    {
            xSemaphoreGive(WN);
            xSemaphoreGive(SN);
    }
    else if(Lane=='E' && Destination=='W')
    {
            xSemaphoreGive(WN);
            xSemaphoreGive(SN);
            xSemaphoreGive(SW);
            xSemaphoreGive(NS);
            xSemaphoreGive(NE);
            xSemaphoreGive(NW);
    }
    else if(Lane=='E' && Destination=='S')
    {
            xSemaphoreGive(WS);
            xSemaphoreGive(WE);
            xSemaphoreGive(SW);
            xSemaphoreGive(SN);
            xSemaphoreGive(NS);
            xSemaphoreGive(NE);
    }
    else if(Lane=='N' && Destination=='W')
    {
            xSemaphoreGive(SW);
            xSemaphoreGive(EW);
    }
    else if(Lane=='N' && Destination=='S')
    {
            xSemaphoreGive(WS);
            xSemaphoreGive(WE);
            xSemaphoreGive(WN);
            xSemaphoreGive(SW);
            xSemaphoreGive(ES);
            xSemaphoreGive(EW);
    }
    else if(Lane=='N' && Destination=='E')
    {
            xSemaphoreGive(WN);
            xSemaphoreGive(WE);
            xSemaphoreGive(SE);
            xSemaphoreGive(SN);
            xSemaphoreGive(ES);
            xSemaphoreGive(EW);
    }
}

void vWest(void *pvParameters)
{
    char current_car;
    char next_car;
    bool status;

    for (int i=0; i<WestIncomingLaneCarsNumber; i++)
    {
        xQueueReceive(WestIncomingLaneCars, &current_car, 1000);
        do
        {
            status = xLockLane('W', current_car);
            if(status)
            {
                printf("Car from West is heading to %c \r}n", current_car);
        
                do
                {
                    xQueueReceive(WestIncomingLaneCars, &next_car, 1000);
                    if(current_car==next_car)
                    {
                        printf("Car from West is heading to %c \r\n", current_car);
                        i++;
                    }
                } while (current_car==next_car);
                xQueueSendToFront(WestIncomingLaneCars, &next_car, 1000);
                vTaskDelay(1000);
                xUnlockLane('W', current_car);
                vTaskDelay(10);
            }
            else{
                printf("Car from West to %c is waiting for permission!\r\n", current_car);
                vTaskDelay(1000);
            }
        } while (!status);
        
    }
}

void vSouth(void *pvParameters)
{
    char current_car;
    char next_car;
    bool status;

    for (int i=0; i<SouthIncomingLaneCarsNumber; i++)
    {
        xQueueReceive(SouthIncomingLaneCars, &current_car, 1000);
        do
        {
            status = xLockLane('S', current_car);
            if(status)
            {
                printf("Car from South is heading to %c \r}n", current_car);
        
                do
                {
                    xQueueReceive(SouthIncomingLaneCars, &next_car, 1000);
                    if(current_car==next_car)
                    {
                        printf("Car from South is heading to %c \r\n", current_car);
                        i++;
                    }
                } while (current_car==next_car);
                xQueueSendToFront(SouthIncomingLaneCars, &next_car, 1000);
                vTaskDelay(1000);
                xUnlockLane('S', current_car);
                vTaskDelay(10);
            }
            else{
                printf("Car from South to %c is waiting for permission!\r\n", current_car);
                vTaskDelay(1000);
            }
        } while (!status);
        
    }
}

void vEast(void *pvParameters)
{
    char current_car;
    char next_car;
    bool status;

    for (int i=0; i<EastIncomingLaneCarsNumber; i++)
    {
        xQueueReceive(EastIncomingLaneCars, &current_car, 1000);
        do
        {
            status = xLockLane('E', current_car);
            if(status)
            {
                printf("Car from East is heading to %c \r}n", current_car);
        
                do
                {
                    xQueueReceive(EastIncomingLaneCars, &next_car, 1000);
                    if(current_car==next_car)
                    {
                        printf("Car from East is heading to %c \r\n", current_car);
                        i++;
                    }
                } while (current_car==next_car);
                xQueueSendToFront(EastIncomingLaneCars, &next_car, 1000);
                vTaskDelay(1000);
                xUnlockLane('E', current_car);
                vTaskDelay(10);
            }
            else{
                printf("Car from East to %c is waiting for permission!\r\n", current_car);
                vTaskDelay(1000);
            }
        } while (!status);
        
    }
}

void vNorth(void *pvParameters)
{
    char current_car;
    char next_car;
    bool status;

    for (int i=0; i<EastIncomingLaneCarsNumber; i++)
    {
        xQueueReceive(EastIncomingLaneCars, &current_car, 1000);
        do
        {
            status = xLockLane('E', current_car);
            if(status)
            {
                printf("Car from East is heading to %c \r}n", current_car);
        
                do
                {
                    xQueueReceive(EastIncomingLaneCars, &next_car, 1000);
                    if(current_car==next_car)
                    {
                        printf("Car from East is heading to %c \r\n", current_car);
                        i++;
                    }
                } while (current_car==next_car);
                xQueueSendToFront(EastIncomingLaneCars, &next_car, 1000);
                vTaskDelay(1000);
                xUnlockLane('E', current_car);
                vTaskDelay(10);
            }
            else{
                printf("Car from East to %c is waiting for permission!\r\n", current_car);
                vTaskDelay(1000);
            }
        } while (!status);
        
    }
}