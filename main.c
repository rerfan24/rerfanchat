#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dir.h>
#include <process.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include "cJSON.c"
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
struct users{
    char name[1000];
    char tok[33];
    char channel[100];
};
int i=0,w=0;
char reqstring[5000];
char* respond;
char pathg[1000];
char previous_chancontent[10000];
int previous_arraaynum=0;
struct users members[1000];
int Sock_Creator(int server_socket);
void token_creator(char* a);
void registering();
void loginning();
void logouting();
void createchanneling();
void joinchanneling();
void leaving();
void messagesending();
void channelmembering();
void refreshing();
void findmembering();
void Rec_Req(int server_socket){
    int sock;
    sock=Sock_Creator(server_socket);
    recv(sock,reqstring,5000,0);
    printf("%s\n",reqstring);
    if(strncmp(reqstring,"register ",9)==0)
        registering();
    if(strncmp(reqstring,"login ",6)==0)
        loginning();
    if(strncmp(reqstring,"logout ",7)==0)
        logouting();
    if(strncmp(reqstring,"create channel ",15)==0)
        createchanneling();
    if(strncmp(reqstring,"join channel ",13)==0)
        joinchanneling();
    if(strncmp(reqstring,"leave ",6)==0)
        leaving();
    if(strncmp(reqstring,"send ",5)==0)
        messagesending();
    if(strncmp(reqstring,"channel members ",16)==0)
        channelmembering();
    if (strncmp(reqstring,"refresh ",8)==0)
        refreshing();
    if (strncmp(reqstring,"find members ",13)==0)
        findmembering();
    if(sock == -1)
        return;
    printf("%s\n",respond);
    send(sock,respond,strlen(respond),0);
    closesocket(sock);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");
    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        return -1;
    }
    else
        printf("Socket successfully bound..\n");
    // Now server is ready to listen and verify
    memset(previous_chancontent,0,10000);
    while(1){
        memset(reqstring,0,sizeof(reqstring));
        Rec_Req(server_socket);
    }
    return 0;
}

int Sock_Creator(int server_socket){
    struct sockaddr_in client;
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        return -1;
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    int len = sizeof(client);
    int client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        return -1;
    }
    else{
        printf("Server acccepted the client..\n");
        // Function for chatting between client and server
        return client_socket;
    }
}

void token_creator(char* a){
    srand(time(NULL));
    strcpy(members[i].name,a);
    for (int j=0;j<32;j++){
        members[i].tok[j]=65+(rand()%26);
    }
    i++;
}

void registering(){
    char a[1000],b[1000];
    sscanf(reqstring,"%*s%*c%s%s",a,b);
    a[strlen(a)-1]=0;
    printf("%s : %s\n",a,b);
    char* dirname = "Resources";
    mkdir(dirname);
    mkdir("Resources/Users");
    mkdir("Resources/Channels");
    char path[1000];
    strcpy(path,"Resources/Users/");
    strcat(path,a);
    strcat(path,".json");
    FILE *cfPtr;
    if ((cfPtr = fopen(path, "r"))==NULL){
        if ((cfPtr = fopen(path, "w"))==NULL){
            puts("File could not be opened");
        }
        else {
            cJSON* client = cJSON_CreateObject();
            cJSON_AddStringToObject(client,"username",a);
            cJSON_AddStringToObject(client,"password",b);
            char* jsonclient = cJSON_PrintUnformatted(client);
            fprintf(cfPtr,"%s",jsonclient);
        }
        cJSON* success=cJSON_CreateObject();
        cJSON_AddStringToObject(success,"type","Successful");
        cJSON_AddStringToObject(success,"content","");
        respond = cJSON_PrintUnformatted(success);
        fclose(cfPtr);
    }
    else {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error,"type","Error");
        cJSON_AddStringToObject(error,"content","This User-Name Has Been Taken");
        respond = cJSON_PrintUnformatted(error);
    }
}

void loginning(){
    char a[1000],b[1000];
    sscanf(reqstring,"%*s%*c%s%s",a,b);
    a[strlen(a)-1]=0;
    printf("%s : %s\n",a,b);
    char path[1000];
    strcpy(path,"Resources/Users/");
    strcat(path,a);
    strcat(path,".json");
    FILE *cfPtr;
    if ((cfPtr = fopen(path, "r"))==NULL){
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error,"type","Error");
        cJSON_AddStringToObject(error,"content","There Is No User With Such Name!");
        respond = cJSON_PrintUnformatted(error);
        fclose(cfPtr);
    }
    else {
        fclose(cfPtr);
        for (int j=0;j<i;j++){
            if (strcmp(a,members[j].name)==0){
                cJSON* error=cJSON_CreateObject();
                cJSON_AddStringToObject(error,"type","Error");
                cJSON_AddStringToObject(error,"content","This User-Name Is Already Online");
                respond = cJSON_PrintUnformatted(error);
                return;
            }
        }
        FILE* cfPtr2;
        char loginpasscheck[1000];
        cfPtr2=fopen(path, "r");
        fscanf(cfPtr2,"%s",loginpasscheck);
        fclose(cfPtr2);
        cJSON* loginpasscheckj=cJSON_Parse(loginpasscheck);
        printf("%s\n",loginpasscheck);
        cJSON* jsonpass = cJSON_GetObjectItemCaseSensitive(loginpasscheckj,"password");
        memset(loginpasscheck,0,sizeof(loginpasscheck));
        strcpy(loginpasscheck,jsonpass->valuestring);
        if (strcmp(loginpasscheck,b)==0){
            token_creator(a);
            cJSON* success=cJSON_CreateObject();
            cJSON_AddStringToObject(success,"type","AuthToken");
            cJSON_AddStringToObject(success,"content",members[i-1].tok);
            respond = cJSON_PrintUnformatted(success);
        }
        else {
            cJSON* error=cJSON_CreateObject();
            cJSON_AddStringToObject(error,"type","Error");
            cJSON_AddStringToObject(error,"content","Password Is Wrong!");
            respond = cJSON_PrintUnformatted(error);
        }
    }
}

void logouting(){
    char a[1000];
    sscanf(reqstring,"%*s%*c%s",a);
    for (int j=0;j<i;j++){
            if (strcmp(a,members[j].tok)==0){
                cJSON* success=cJSON_CreateObject();
                cJSON_AddStringToObject(success,"type","Successful");
                cJSON_AddStringToObject(success,"content","");
                respond = cJSON_PrintUnformatted(success);
                memset(members[j].name,0,sizeof(members[j].name));
                memset(members[j].tok,0,sizeof(members[j].tok));
                return;
            }
    }
}

void createchanneling(){
    char nametok[1000];
    sscanf(reqstring,"%*s%*s%*c%[^\n]s",nametok);
    int len=strlen(nametok);
    char toktok[33];
    for(int j=0;j<32;j++)
        toktok[j]=nametok[len+j-32];
    toktok[32]='\0';
    for (int j=len;j>=len-34;j--)
        nametok[j]='\0';
    char name[100];
    int w=-1;
    for (int j=0;j<=i;j++)
        if (strcmp(members[j].tok,toktok)==0){
            strcpy(name,members[j].name);
            w=j;
        }
    memset(pathg,0,sizeof(pathg));
    strcpy(pathg,"Resources/Channels/");
    strcat(pathg,nametok);
    strcat(pathg,".json");
    printf("%s\n",pathg);
    FILE *cfPtr;
    if (w==(-1)) {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error,"type","Error");
        cJSON_AddStringToObject(error,"content","Token Not Found!");
        respond = cJSON_PrintUnformatted(error);
    }
    else{
        if ((cfPtr = fopen(pathg, "r"))!=NULL){
            cJSON* error=cJSON_CreateObject();
            cJSON_AddStringToObject(error,"type","Error");
            cJSON_AddStringToObject(error,"content","There Is Another Channel With The Same Name!");
            respond = cJSON_PrintUnformatted(error);
        }
        else {
            fclose(cfPtr);
            cfPtr=fopen(pathg,"w");
            cJSON* whole = cJSON_CreateObject();
            cJSON* mess = cJSON_CreateArray();
            cJSON* m;
            cJSON_AddItemToArray(mess , m = cJSON_CreateObject());
            strcat(name," Created -");
            strcat(name,nametok);
            strcat(name,"-");
            strcpy(members[w].channel,nametok);
            cJSON_AddStringToObject(m, "sender" , "Server");
            cJSON_AddStringToObject(m, "content" , name);
            cJSON_AddItemToObject(whole,"messages",mess);
            char* jsonchannel = cJSON_PrintUnformatted(whole);
            printf("%s\n",jsonchannel);
            fprintf(cfPtr,"%s%c",jsonchannel,'\n');
            cJSON* success=cJSON_CreateObject();
            cJSON_AddStringToObject(success,"type","Successful");
            cJSON_AddStringToObject(success,"content","");
            respond = cJSON_PrintUnformatted(success);
            w++;
            fclose(cfPtr);
        }
    }
}

void joinchanneling(){
    char nametok[1000];
    sscanf(reqstring,"%*s%*s%*c%[^\n]s",nametok);
    int len=strlen(nametok);
    char toktok[33];
    for(int j=0;j<32;j++)
        toktok[j]=nametok[len+j-32];
    toktok[32]='\0';
    for (int j=len;j>=len-34;j--)
        nametok[j]='\0';
    char name[100];
    int w=-1;
    for (int j=0;j<=i;j++)
        if (strcmp(members[j].tok,toktok)==0){
            strcpy(name,members[j].name);
            w=j;
        }
    memset(pathg,0,sizeof(pathg));
    strcpy(pathg,"Resources/Channels/");
    strcat(pathg,nametok);
    strcat(pathg,".json");
    printf("%s\n",pathg);
    FILE *cfPtr;
    if (w==(-1)) {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error,"type","Error");
        cJSON_AddStringToObject(error,"content","Token Not Found!");
        respond = cJSON_PrintUnformatted(error);
    }
    else{
        if ((cfPtr = fopen(pathg, "r"))==NULL){
            cJSON* error=cJSON_CreateObject();
            cJSON_AddStringToObject(error,"type","Error");
            cJSON_AddStringToObject(error,"content","There Is No Channel With Such Name!");
            respond = cJSON_PrintUnformatted(error);
        }
        else {
            char chancontent[10000];
            fscanf(cfPtr,"%[^\n]s",chancontent);
            printf("%s\n",chancontent);
            fclose(cfPtr);
            cfPtr=fopen(pathg,"w");
            cJSON* jsoncont = cJSON_Parse(chancontent);
            cJSON* jsonmessage = cJSON_GetObjectItemCaseSensitive(jsoncont,"messages");
            cJSON* m;
            cJSON_AddItemToArray(jsonmessage , m = cJSON_CreateObject());
            strcat(name," Joined -");
            strcat(name,nametok);
            strcat(name,"-");
            strcpy(members[w].channel,nametok);
            cJSON_AddStringToObject(m, "sender" , "Server");
            cJSON_AddStringToObject(m, "content" , name);
            cJSON* whole= cJSON_CreateObject();;
            cJSON_AddItemToObject(whole,"messages",jsonmessage);
            char* jsonchannel = cJSON_PrintUnformatted(whole);
            printf("%s\n",jsonchannel);
            fprintf(cfPtr,"%s",jsonchannel);
            cJSON* success=cJSON_CreateObject();
            cJSON_AddStringToObject(success,"type","Successful");
            cJSON_AddStringToObject(success,"content","");
            respond = cJSON_PrintUnformatted(success);
            w++;
            fclose(cfPtr);
        }
    }
}

void leaving(){
    char toktok[33];
    sscanf(reqstring,"%*s%s",toktok);
    int j;
    for (j=0;j<i;j++){
        if (strcmp(toktok,members[j].tok)==0){
            break;
        }
    }
    FILE* cfPtr;
    cfPtr = fopen(pathg, "r");
    char chancontent[10000];
    fscanf(cfPtr,"%[^\n]s",chancontent);
    printf("%s\n",chancontent);
    fclose(cfPtr);
    cfPtr=fopen(pathg,"w");
    cJSON* jsoncont = cJSON_Parse(chancontent);
    cJSON* jsonmessage = cJSON_GetObjectItemCaseSensitive(jsoncont,"messages");
    cJSON* m;
    cJSON_AddItemToArray(jsonmessage , m = cJSON_CreateObject());
    char name[1000];
    strcpy(name,members[j].name);
    strcat(name," Left -");
    strcat(name,members[j].channel);
    strcat(name,"-");
    cJSON_AddStringToObject(m, "sender" , "Server");
    cJSON_AddStringToObject(m, "content" , name);
    cJSON* whole= cJSON_CreateObject();;
    cJSON_AddItemToObject(whole,"messages",jsonmessage);
    char* jsonchannel = cJSON_PrintUnformatted(whole);
    printf("%s\n",jsonchannel);
    fprintf(cfPtr,"%s",jsonchannel);
    cJSON* success=cJSON_CreateObject();
    cJSON_AddStringToObject(success,"type","Successful");
    cJSON_AddStringToObject(success,"content","");
    respond = cJSON_PrintUnformatted(success);
    memset(members[j].channel,0,sizeof(members[j].channel));
    previous_arraaynum=0;
    fclose(cfPtr);
}

void messagesending(){
    char mess[1000];
    sscanf(reqstring,"%*s%*c%[^\n]s",mess);
    int len=strlen(mess);
    char toktok[33];
    for(int j=0;j<32;j++)
        toktok[j]=mess[len+j-32];
    toktok[32]='\0';
    for (int j=len;j>=len-34;j--)
        mess[j]='\0';
    char name[100];
    int w=-1;
    for (int j=0;j<=i;j++)
        if (strcmp(members[j].tok,toktok)==0){
            strcpy(name,members[j].name);
            w=j;
        }
    FILE *cfPtr;
    if (w==(-1)) {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error,"type","Error");
        cJSON_AddStringToObject(error,"content","Token Not Found!");
        respond = cJSON_PrintUnformatted(error);
    }
    else{
        cfPtr = fopen(pathg, "r");
        char chancontent[10000];
        fscanf(cfPtr,"%[^\n]s",chancontent);
        printf("%s\n",chancontent);
        fclose(cfPtr);
        cfPtr=fopen(pathg,"w");
        cJSON* jsoncont = cJSON_Parse(chancontent);
        cJSON* jsonmessage = cJSON_GetObjectItemCaseSensitive(jsoncont,"messages");
        cJSON* m;
        cJSON_AddItemToArray(jsonmessage , m = cJSON_CreateObject());
        cJSON_AddStringToObject(m, "sender" , name);
        cJSON_AddStringToObject(m, "content" , mess);
        cJSON* whole= cJSON_CreateObject();;
        cJSON_AddItemToObject(whole,"messages",jsonmessage);
        char* jsonchannel = cJSON_PrintUnformatted(whole);
        printf("%s\n",jsonchannel);
        fprintf(cfPtr,"%s",jsonchannel);
        cJSON* success=cJSON_CreateObject();
        cJSON_AddStringToObject(success,"type","Successful");
        cJSON_AddStringToObject(success,"content","");
        respond = cJSON_PrintUnformatted(success);
        fclose(cfPtr);
    }
}

void channelmembering(){
    char toktok[33];
    sscanf(reqstring,"%*s%*s%s",toktok);
    char name[100];
    int w=-1;
    for (int j=0;j<=i;j++)
        if (strcmp(members[j].tok,toktok)==0){
            strcpy(name,members[j].name);
            w=j;
        }
    if (w==(-1)) {
        cJSON* error=cJSON_CreateObject();
        cJSON_AddStringToObject(error,"type","Error");
        cJSON_AddStringToObject(error,"content","Token Not Found, Make Sure The Requested Member Is In The Channel!");
        respond = cJSON_PrintUnformatted(error);
    }
    else{
        cJSON* whole = cJSON_CreateObject();
        cJSON* memlist = cJSON_CreateArray();
        for (int j=0;j<=i;j++){
            if(strcmp(members[w].channel,members[j].channel)==0){
                cJSON_AddItemToArray(memlist,cJSON_CreateString(members[j].name));
            }
        }
        cJSON_AddStringToObject(whole,"type","List");
        cJSON_AddItemToObject(whole,"content",memlist);
        respond = cJSON_PrintUnformatted(whole);
    }
}

void refreshing(){
    FILE* cfPtr;
    char chancontent[10000];
    memset(chancontent,0,sizeof(chancontent));
    cfPtr = fopen(pathg,"r");
    fscanf(cfPtr,"%[^\n]s",chancontent);
    fclose(cfPtr);
    cJSON* jsonchancontent=cJSON_CreateArray();
    cJSON* saver=cJSON_Parse(chancontent);
    cJSON* mess=cJSON_GetObjectItemCaseSensitive(saver,"messages");
    for (int j=previous_arraaynum;j<cJSON_GetArraySize(mess);j++){
        if (j==previous_arraaynum)
            cJSON_AddItemToArray(jsonchancontent,cJSON_GetArrayItem(mess,j));
    }
    printf("ok1\n");
    previous_arraaynum=cJSON_GetArraySize(mess);
    cJSON* whole=cJSON_CreateObject();
    cJSON_AddStringToObject(whole,"type","List");
    cJSON_AddItemToObject(whole,"content",jsonchancontent);
    respond = cJSON_PrintUnformatted(whole);
}

void findmembering(){
    char toktok[33];
    char name[100];
    name[0]='f';
    char use[100];
    sscanf(reqstring,"%*s%*s%s%s",use,toktok);
    use[strlen(use)-1]=0;
    cJSON* whole = cJSON_CreateObject();
    char path[1000];
    strcat(path,pathg);
    int j;
    for (j=strlen(pathg);path[j]!='/';j--);
    for (int t=0;path[t+j]!='\0';t++){
        path[t]=path[t+j+1];
    }
    path[strlen(path)-1]=0;
    path[strlen(path)-1]=0;
    path[strlen(path)-1]=0;
    path[strlen(path)-1]=0;
    path[strlen(path)-1]=0;
    printf("--%s--\n",path);
    for (int t=0;t<=i;t++){
        if (strcmp(path,members[t].channel)==0){
            if (strcmp(use,members[t].name)==0){
                strcpy(name,"True");
            }
        }
    }
    if (name[0]=='f'){
        strcpy(name,"False");
    }
    cJSON_AddStringToObject(whole,"type","Successful");
    cJSON_AddStringToObject(whole,"content",name);
    respond = cJSON_PrintUnformatted(whole);
}

