//Include libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"
#include "FreeRTOSConfig.h"

//acquire all locks based on source and destination
bool xTakeLock(char Lane, char Destination);
//Give/Release all locks
void xGiveLock(char Lane, char Destination);

//4 processes, each for a Lane
void vAtoBDF(void *pvParameters);
void vCtoDFH(void *pvParameters);
void vEtoFHB(void *pvParameters);
void vGtoHBD(void *pvParameters);

//4 queues, 1 each for each lane
xQueueHandle VehiclesInLaneA = 0;
xQueueHandle VehiclesInLaneC = 0;
xQueueHandle VehiclesInLaneE = 0;
xQueueHandle VehiclesInLaneG = 0;

//Number of initial vehicles in each lane
int NumberOfVehiclesInA = 100;
int NumberOfVehiclesInC = 100;
int NumberOfVehiclesInE = 100;
int NumberOfVehiclesInG = 100;

//Mutex locks declared for each source/destination
xSemaphoreHandle AB = 0;
xSemaphoreHandle AD = 0;
xSemaphoreHandle AF = 0;
xSemaphoreHandle CD = 0;
xSemaphoreHandle CF = 0;
xSemaphoreHandle CH = 0;
xSemaphoreHandle EF = 0;
xSemaphoreHandle EH = 0;
xSemaphoreHandle EB = 0;
xSemaphoreHandle GH = 0;
xSemaphoreHandle GB = 0;
xSemaphoreHandle GD = 0;

int main(void)
{
    //initialize queues with the max size
    VehiclesInLaneA = xQueueCreate(NumberOfVehiclesInA,sizeof(char));
    VehiclesInLaneC = xQueueCreate(NumberOfVehiclesInC,sizeof(char));
    VehiclesInLaneE = xQueueCreate(NumberOfVehiclesInE,sizeof(char));
    VehiclesInLaneG = xQueueCreate(NumberOfVehiclesInG,sizeof(char));

    char temp;
    int num;
    //possible destinations of cars in A/C/E/G
    char paths[4][3]={{'B','D','F'},{'D','F','H'},{'F','H','B'},{'H','B','D'}};

    //Add radom destination of cars in each lane
    for(int i=0; i<NumberOfVehiclesInA; i++)
    {
        num=rand()%3;
        xQueueSend(VehiclesInLaneA,&paths[0][num],100);
    }
    for(int i=0; i<NumberOfVehiclesInC; i++)
    {
        num=rand()%3;
        xQueueSend(VehiclesInLaneC,&paths[1][num],100);
    }
    for(int i=0; i<NumberOfVehiclesInE; i++)
    {
        num=rand()%3;
        xQueueSend(VehiclesInLaneE,&paths[2][num],100);
    }
    for(int i=0;i<NumberOfVehiclesInG;i++)
    {
        num=rand()%3;
        xQueueSend(VehiclesInLaneG,&paths[3][num],100);
    }

    //initialize mutex locks
    AB = xSemaphoreCreateMutex();
    AD = xSemaphoreCreateMutex();
    AF = xSemaphoreCreateMutex();
    CD = xSemaphoreCreateMutex();
    CF = xSemaphoreCreateMutex();
    CH = xSemaphoreCreateMutex();
    EF = xSemaphoreCreateMutex();
    EH = xSemaphoreCreateMutex();
    EB = xSemaphoreCreateMutex();
    GH = xSemaphoreCreateMutex();
    GB = xSemaphoreCreateMutex();
    GD = xSemaphoreCreateMutex();

    //initialize 4 tasks, each process handles one lane
    xTaskCreate(&vAtoBDF, "A to BDF", 1024, NULL, 1, NULL);
    xTaskCreate(&vCtoDFH, "C to DFH", 1024, NULL, 1, NULL);
    xTaskCreate(&vEtoFHB, "E to FHB", 1024, NULL, 1, NULL);
    xTaskCreate(&vGtoHBD, "G to HBD", 1024, NULL, 1, NULL);

    //start all 4 tasks and start scheduling them
    vTaskStartScheduler();

    return 0;
}

bool xTakeLock(char Lane, char Destination)
{
    bool taken = false;
    //Check if all required mutex are free
    if(Lane=='A' && Destination=='B')
    {
        if(uxSemaphoreGetCount(EB)+uxSemaphoreGetCount(GB)==2)
        {
            //if yes then Take these mutex
            xSemaphoreTake(EB,100);
            xSemaphoreTake(GB,100);
            taken = true;
        }
    }
    else if(Lane=='A' && Destination=='D')
    {
        if(uxSemaphoreGetCount(CF)+
            uxSemaphoreGetCount(CH)+
            uxSemaphoreGetCount(GB)+
            uxSemaphoreGetCount(GD)==4)
        {
            xSemaphoreTake(CF,100);
            xSemaphoreTake(CH,100);
            xSemaphoreTake(GB,100);
            xSemaphoreTake(GD,100);
            taken = true;
        }
    }
    else if(Lane=='A' && Destination=='F')
    {
        if(uxSemaphoreGetCount(CF)+
            uxSemaphoreGetCount(CH)+
            uxSemaphoreGetCount(EF)+
            uxSemaphoreGetCount(EH)+
            uxSemaphoreGetCount(GB)+
            uxSemaphoreGetCount(GD)==6)
        {
            xSemaphoreTake(CF,100);
            xSemaphoreTake(CH,100);
            xSemaphoreTake(EF,100);
            xSemaphoreTake(EH,100);
            xSemaphoreTake(GB,100);
            xSemaphoreTake(GD,100);
            taken = true;
        }
    }
    else if(Lane=='C' && Destination=='D')
    {
        if(uxSemaphoreGetCount(AD)+uxSemaphoreGetCount(GD)==2)
        {
            xSemaphoreTake(AD,100);
            xSemaphoreTake(GD,100);
            taken = true;
        }
    }
    else if(Lane=='C' && Destination=='F')
    {
        if(uxSemaphoreGetCount(AD)+
            uxSemaphoreGetCount(AF)+
            uxSemaphoreGetCount(EB)+
            uxSemaphoreGetCount(EH)+
            uxSemaphoreGetCount(EF)+
            uxSemaphoreGetCount(GD)==6)
        {
            xSemaphoreTake(AD,100);
            xSemaphoreTake(AF,100);
            xSemaphoreTake(EB,100);
            xSemaphoreTake(EH,100);
            xSemaphoreTake(EF,100);
            xSemaphoreTake(GD,100);
            taken = true;
        }
    }
    else if(Lane=='C' && Destination=='H')
    {
        if(uxSemaphoreGetCount(AD)+
            uxSemaphoreGetCount(AF)+
            uxSemaphoreGetCount(EB)+
            uxSemaphoreGetCount(EH)+
            uxSemaphoreGetCount(GB)+
            uxSemaphoreGetCount(GH)==6)
        {
            xSemaphoreTake(AD,100);
            xSemaphoreTake(AF,100);
            xSemaphoreTake(EB,100);
            xSemaphoreTake(EH,100);
            xSemaphoreTake(GB,100);
            xSemaphoreTake(GH,100);
            taken = true;
        }
    }
    else if(Lane=='E' && Destination=='F')
    {
        if(uxSemaphoreGetCount(AF)+
            uxSemaphoreGetCount(CF)==2)
        {
            xSemaphoreTake(AF,100);
            xSemaphoreTake(CF,100);
            taken = true;
        }
    }
    else if(Lane=='E' && Destination=='H')
    {
        if(uxSemaphoreGetCount(AF)+
            uxSemaphoreGetCount(CF)+
            uxSemaphoreGetCount(CH)+
            uxSemaphoreGetCount(GB)+
            uxSemaphoreGetCount(GD)+
            uxSemaphoreGetCount(GH)==6)
        {
            xSemaphoreTake(AF,100);
            xSemaphoreTake(CF,100);
            xSemaphoreTake(CH,100);
            xSemaphoreTake(GB,100);
            xSemaphoreTake(GD,100);
            xSemaphoreTake(GH,100);
            taken = true;
        }
    }
    else if(Lane=='E' && Destination=='B')
    {
        if(uxSemaphoreGetCount(AB)+
            uxSemaphoreGetCount(AD)+
            uxSemaphoreGetCount(CH)+
            uxSemaphoreGetCount(CF)+
            uxSemaphoreGetCount(GB)+
            uxSemaphoreGetCount(GD)==6)
        {
            xSemaphoreTake(AB,100);
            xSemaphoreTake(AD,100);
            xSemaphoreTake(CH,100);
            xSemaphoreTake(CF,100);
            xSemaphoreTake(GB,100);
            xSemaphoreTake(GD,100);
            taken = true;
        }
    }
    else if(Lane=='G' && Destination=='H')
    {
        if(uxSemaphoreGetCount(CH)+
            uxSemaphoreGetCount(EH)==2)
        {
            xSemaphoreTake(CH,100);
            xSemaphoreTake(EH,100);
            taken = true;
        }
    }
    else if(Lane=='G' && Destination=='B')
    {
        if(uxSemaphoreGetCount(AB)+
            uxSemaphoreGetCount(AD)+
            uxSemaphoreGetCount(AF)+
            uxSemaphoreGetCount(CH)+
            uxSemaphoreGetCount(EB)+
            uxSemaphoreGetCount(EH)==6)
        {
            xSemaphoreTake(AB,100);
            xSemaphoreTake(AD,100);
            xSemaphoreTake(AF,100);
            xSemaphoreTake(CH,100);
            xSemaphoreTake(EB,100);
            xSemaphoreTake(EH,100);
            taken = true;
        }
    }
    else if(Lane=='G' && Destination=='D')
    {
        if(uxSemaphoreGetCount(AF)+
            uxSemaphoreGetCount(AD)+
            uxSemaphoreGetCount(CD)+
            uxSemaphoreGetCount(CF)+
            uxSemaphoreGetCount(EB)+
            uxSemaphoreGetCount(EH)==6)
        {
            xSemaphoreTake(AF,100);
            xSemaphoreTake(AD,100);
            xSemaphoreTake(CD,100);
            xSemaphoreTake(CF,100);
            xSemaphoreTake(EB,100);
            xSemaphoreTake(EH,100);
            taken = true;
        }
    }
    return taken;
}

void xGiveLock(char Lane, char Destination)
{
    //Release all locks/mutex once car has reached destination/crosse junction
    if(Lane=='A' && Destination=='B')
    {
        xSemaphoreGive(EB);
        xSemaphoreGive(GB);
    }
    else if(Lane=='A' && Destination=='D')
    {
        xSemaphoreGive(CF);
        xSemaphoreGive(CH);
        xSemaphoreGive(GB);
        xSemaphoreGive(GD);
    }
    else if(Lane=='A' && Destination=='F')
    {
        xSemaphoreGive(CF);
        xSemaphoreGive(CH);
        xSemaphoreGive(EF);
        xSemaphoreGive(EH);
        xSemaphoreGive(GB);
        xSemaphoreGive(GD);
    }
    else if(Lane=='C' && Destination=='D')
    {
        xSemaphoreGive(AD);
        xSemaphoreGive(GD);
    }
    else if(Lane=='C' && Destination=='F')
    {
            xSemaphoreGive(AD);
            xSemaphoreGive(AF);
            xSemaphoreGive(EB);
            xSemaphoreGive(EH);
            xSemaphoreGive(EF);
            xSemaphoreGive(GD);
    }
    else if(Lane=='C' && Destination=='H')
    {
            xSemaphoreGive(AD);
            xSemaphoreGive(AF);
            xSemaphoreGive(EB);
            xSemaphoreGive(EH);
            xSemaphoreGive(GB);
            xSemaphoreGive(GH);
    }
    else if(Lane=='E' && Destination=='F')
    {
            xSemaphoreGive(AF);
            xSemaphoreGive(CF);
    }
    else if(Lane=='E' && Destination=='H')
    {
            xSemaphoreGive(AF);
            xSemaphoreGive(CF);
            xSemaphoreGive(CH);
            xSemaphoreGive(GB);
            xSemaphoreGive(GD);
            xSemaphoreGive(GH);
    }
    else if(Lane=='E' && Destination=='B')
    {
            xSemaphoreGive(AB);
            xSemaphoreGive(AD);
            xSemaphoreGive(CH);
            xSemaphoreGive(CF);
            xSemaphoreGive(GB);
            xSemaphoreGive(GD);
    }
    else if(Lane=='G' && Destination=='H')
    {
            xSemaphoreGive(CH);
            xSemaphoreGive(EH);
    }
    else if(Lane=='G' && Destination=='B')
    {
            xSemaphoreGive(AB);
            xSemaphoreGive(AD);
            xSemaphoreGive(AF);
            xSemaphoreGive(CH);
            xSemaphoreGive(EB);
            xSemaphoreGive(EH);
    }
    else if(Lane=='G' && Destination=='D')
    {
            xSemaphoreGive(AF);
            xSemaphoreGive(AD);
            xSemaphoreGive(CD);
            xSemaphoreGive(CF);
            xSemaphoreGive(EB);
            xSemaphoreGive(EH);
    }
}

void vAtoBDF(void *pvParameters)
{
    char path;//holds the destination of each vehicle
    char nextVeh;//holds the destination of the vehicle behind current vehicle
    bool status;//holds true if all locks acquired successfully
    for (int i=0; i<NumberOfVehiclesInA; i++)
    {
        xQueueReceive(VehiclesInLaneA,&path,1000);//get destination of next car from queue of Lane A
        do
        {
            status=xTakeLock('A',path);//check and acquire mutex if all free. Returns false if one or more mutex are acquired by any other process
            if(status)//if mutex taken successfully
            {
                printf("Vehicle moving from A to %c \r\n",path);
                do{
                    //check if next vehicle has same destination
                    xQueueReceive(VehiclesInLaneA,&nextVeh,1000);
                    if(path==nextVeh)
                    {
                        //if yes then let is pass als without making it wait
                        printf("Vehicle moving from A to %c \r\n",path);
                        i++;
                    }
                }while(path==nextVeh);
                //if destination of next vehicle if different then add it back to queue to make it wait for its turn
                xQueueSendToFront(VehiclesInLaneA,&nextVeh,1000);
                //lets say each car takes 1 second to reach dest
                vTaskDelay(1000);
                //once car has reached dest, release all the locks
                xGiveLock('A',path);
                //add a bit more delay, so that other processes can take charge
                vTaskDelay(10);
            }else{
                //if lock wasn't acquired successfully then wait for car's turn.
                printf("A to %c Waiting for turn\r\n",path);
                vTaskDelay(1000);
            }
        }while(!status);
    }
}

void vCtoDFH(void *pvParameters)
{
    char path;
    char nextVeh;
    bool status;
    for (int i=0; i<NumberOfVehiclesInC; i++)
    {
        xQueueReceive(VehiclesInLaneC,&path,1000);
        do
        {
            status=xTakeLock('C',path);
            if(status)
            {
                printf("Vehicle moving from C to %c \r\n",path);
                do{
                    xQueueReceive(VehiclesInLaneC,&nextVeh,1000);
                    if(path==nextVeh)
                    {
                        printf("Vehicle moving from C to %c \r\n",path);
                        i++;
                    }
                }while(path==nextVeh);
                xQueueSendToFront(VehiclesInLaneC,&nextVeh,1000);
                vTaskDelay(1000);
                xGiveLock('C',path);
                vTaskDelay(10);
            }else{
                printf("C to %c Waiting for turn\r\n",path);
                vTaskDelay(1000);
            }
        }while(!status);
    }
}

void vEtoFHB(void *pvParameters)
{
    char path;
    char nextVeh;
    bool status;
    for (int i=0; i<NumberOfVehiclesInE; i++)
    {
        xQueueReceive(VehiclesInLaneE,&path,1000);
        do
        {
            status=xTakeLock('E',path);
            if(status)
            {
                printf("Vehicle moving from E to %c \r\n",path);
                do{
                    xQueueReceive(VehiclesInLaneE,&nextVeh,1000);
                    if(path==nextVeh)
                    {
                        printf("Vehicle moving from E to %c \r\n",path);
                        i++;
                    }
                }while(path==nextVeh);
                xQueueSendToFront(VehiclesInLaneE,&nextVeh,1000);
                vTaskDelay(1000);
                xGiveLock('E',path);
                vTaskDelay(10);
            }else{
                printf("E to %c Waiting for turn\r\n",path);
                vTaskDelay(1000);
            }
        }while(!status);
    }
}

void vGtoHBD(void *pvParameters)
{
    char path;
    char nextVeh;
    bool status;
    for (int i=0; i<NumberOfVehiclesInG; i++)
    {
        xQueueReceive(VehiclesInLaneG,&path,1000);
        do
        {
            status=xTakeLock('G',path);
            if(status)
            {
                printf("Vehicle moving from G to %c \r\n",path);
                do{
                    xQueueReceive(VehiclesInLaneG,&nextVeh,1000);
                    if(path==nextVeh)
                    {
                        printf("Vehicle moving from G to %c \r\n",path);
                        i++;
                    }
                }while(path==nextVeh);
                xQueueSendToFront(VehiclesInLaneG,&nextVeh,1000);
                vTaskDelay(1000);
                xGiveLock('G',path);
                vTaskDelay(10);
            }else{
                printf("G to %c Waiting for turn\r\n",path);
                vTaskDelay(1000);
            }
        }while(!status);
    }
}