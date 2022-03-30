#include "h264.h"

int main() {
    PRTPDATA_RETURN * rprtpData = NULL;
    char recvbuf[MAXDATASIZE];
    int socket1;
    struct sockaddr_in client;
    struct timeval tv;
    socklen_t len_client = sizeof(client);
    socklen_t receive_bytes = 0;
    socket1 = socket(AF_INET, SOCK_DGRAM, 0);
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(DEST_IP);
    client.sin_port = htons(DEST_PORT);
    unsigned int delay,timestamp4;
    if (bind(socket1, (struct sockaddr*)&client, sizeof(client)) == -1)
    {
        printf("Bind to local machine error.\n");

        return getchar();
    }
    else
    {
        printf("Bind to local machine.\n");
    }
    while ((receive_bytes = recvfrom(socket1, recvbuf, MAXDATASIZE, 0, (struct sockaddr *)&client, &len_client)) > 0)
    {
        gettimeofday(&tv,NULL);
        rprtpData = (PRTPDATA_RETURN*)&recvbuf[12];
        timestamp4 = tv.tv_sec*1000000 + tv.tv_usec - Timestamp_parameter;
        printf("包序号：%u\n",rprtpData->send_Index);
//        printf("时间戳1：%u\n",rprtpData->send_Time);
//        printf("时间戳2：%u\n",rprtpData->receive_Time);
//        printf("时间戳3：%u\n",rprtpData->receive_resend_Time);
//        printf("时间戳4：%u\n",timestamp4);
        delay = rprtpData->receive_Time - rprtpData->send_Time + timestamp4 - rprtpData->receive_resend_Time;
        printf("本次测得SNTP时延(ms)：%f\n",(double)delay/2000);
    }
}
