#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winsock2.h>
#include <windows.h>
#include "cJSON.c"
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
int wtd=1,wtd2=1,wtd3=1;
char token[1000];

int Sock_Creator();
void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}
void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

void Send_Req(char reqstring[5000],char *restype,char *resinside){
    int sock;
    sock=Sock_Creator();
    char saver[5000];
    strcpy(saver,reqstring);
    send(sock,reqstring,5000,0);
    memset(reqstring,0,sizeof(reqstring));
    memset(resinside,0,sizeof(reqstring));
    memset(restype,0,sizeof(reqstring));
    recv(sock, reqstring,5000,0);
    cJSON* jsonrec = cJSON_Parse(reqstring);
    cJSON* jsontype = cJSON_GetObjectItemCaseSensitive(jsonrec,"type");
    cJSON* jsoncontent = cJSON_GetObjectItemCaseSensitive(jsonrec,"content");
    if(cJSON_IsString(jsoncontent)){
        strcpy(resinside,jsoncontent->valuestring);
        strcpy(restype,jsontype->valuestring);
    }
    else if(cJSON_IsArray(jsoncontent)){
        system("cls");
        if (*saver=='r') {
            cJSON* list1 = NULL;
            cJSON_ArrayForEach(list1, jsoncontent)
            {
                char* ressend=(char *)malloc(5000*sizeof(char));
                char* resinside2=(char *)malloc(5000*sizeof(char));
                memset(ressend,0,sizeof(ressend));
                memset(resinside2,0,sizeof(resinside2));
                cJSON* listcont = cJSON_GetObjectItemCaseSensitive(list1,"content");
                cJSON* listsender = cJSON_GetObjectItemCaseSensitive(list1,"sender");
                strcat(resinside2,listcont->valuestring);
                strcat(ressend,listsender->valuestring);
                printf("%s: %s\n",ressend,resinside2);
            }
            printf("To Continue Please Press Enter...");
            getchar();
        }
        else {
            for(int i=0;i<cJSON_GetArraySize(jsoncontent);i++){
                printf("%s ",cJSON_GetArrayItem(jsoncontent,i)->valuestring);
                if (i==cJSON_GetArraySize(jsoncontent)-1) printf(" .");
                else printf(" , ");
            }
            printf("\n");
            printf("To Continue Please Press Enter...");
            getchar();
        }
    }
}
void chat(){
    while(1){
        char reqstring[5000];
        char mess[5000];
        memset(mess,0,sizeof(mess));
        memset(reqstring,0,sizeof(reqstring));
        system("cls");
        printf("1-Send Message :\n");
        printf("2-Refresh :\n");
        printf("3-Channel Members :\n");
        printf("4-Find Members :\n");
        printf("5-Find Message :\n");
        printf("6-Leave Channel :\n");
        wtd3=0;
        int h=0;
        h=getch();
        while (h == 0xE0){
            int h2;
            h2=getch();
            if (h2==80){
                if (wtd3<6) wtd3++;
                gotoxy(0,0);
                system("cls");
                printf("1-Send Message :\n");
                printf("2-Refresh :\n");
                printf("3-Channel Members :\n");
                printf("4-Find Members :\n");
                printf("5-Find Message :\n");
                printf("6-Leave Channel :\n");
                gotoxy(20,wtd3-1);
                SetColor(12);
                printf("<-");
                SetColor(0);
            }
            if (h2==72){
                if (wtd3>1) wtd3--;
                gotoxy(0,0);
                system("cls");
                printf("1-Send Message :\n");
                printf("2-Refresh :\n");
                printf("3-Channel Members :\n");
                printf("4-Find Members :\n");
                printf("5-Find Message :\n");
                printf("6-Leave Channel :\n");
                gotoxy(20,wtd3-1);
                SetColor(12);
                printf("<-");
                SetColor(0);
            }
            h=getch();
        }
        if (h!=0xE0 && h!=13 && h!=10)
            if (h>=49 && h<=54) {
                printf("%c",h);
                wtd3=h-48;
            }
            else
                wtd3=0;
        if (wtd3==1){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            system("cls");
            printf("Enter Your Message : \n");
            for(int i=0;;i++){
                scanf("%c",&mess[i]);
                if (mess[i]=='\n') {
                    mess[i]='\0';
                    break;
                }
            }
            strcat(reqstring,"send ");
            strcat(reqstring,&mess);
            strcat(reqstring,", ");
            strcat(reqstring,&token);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
        }
        if (wtd3==2){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            system("cls");
            strcat(reqstring,"refresh ");
            strcat(reqstring,&token);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
        }
        if (wtd3==3){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            system("cls");
            strcat(reqstring,"channel members ");
            strcat(reqstring,&token);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
        }
        if (wtd3==4){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            memset(reqstring,0,sizeof(reqstring));
            system("cls");
            printf("Enter Members Name:\n");
            char a[100];
            scanf("%[^\n]s",a);
            strcat(reqstring,"find members ");
            strcat(reqstring,a);
            strcat(reqstring,", ");
            strcat(reqstring,&token);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
            system("cls");
            printf("%s\n",resinside);
            printf("To Continue Please Press Enter...");
            getchar();
            getchar();
        }
        if (wtd3==6){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            memset(reqstring,0,sizeof(reqstring));
            strcat(reqstring,"leave ");
            strcat(reqstring,&token);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
            break;
        }
    }
    return;
}

int main(){
    system("COLOR B0");
    char reqstring[5000];
    char username[MAX],password[MAX];
    printf("Welcome!\n");
    while (1){
        memset(reqstring,0,sizeof(reqstring));
        printf("1-Sign-Up :\n");
        printf("2-Sign-In :\n");
        printf("3-Exit :\n");
        wtd=0;
        int h=0;
        h=getch();
        while (h == 0xE0){
            system("cls");
            int h2;
            h2=getch();
            if (h2==80){
                if (wtd<3) wtd++;
                gotoxy(0,0);
                system("cls");
                printf("1-Sign-Up :\n");
                printf("2-Sign-In :\n");
                printf("3-Exit :\n");
                gotoxy(12,wtd-1);
                SetColor(12);
                printf("<-");
                SetColor(0);
            }
            if (h2==72){
                if (wtd>1) wtd--;
                gotoxy(0,0);
                system("cls");
                printf("1-Sign-Up :\n");
                printf("2-Sign-In :\n");
                printf("3-Exit :\n");
                gotoxy(12,wtd-1);
                SetColor(12);
                printf("<-");
                SetColor(0);
            }
            h=getch();
        }
        if (h!=0xE0 && h!=13 && h!=10)
            if (h>=49 && h<=51) {
                printf("%c",h);
                wtd=h-48;
            }
            else
                wtd=0;
        if (wtd==1){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            system("cls");
            printf("please Enter a User-name : \n");
            for(int i=0;;i++){
                scanf("%c",&username[i]);
                if (username[i]=='\n') {
                    username[i]='\0';
                    break;
                }
            }
            printf("please Enter a Password : \n");
            for(int i=0;;i++){
                scanf("%c",&password[i]);
                if (password[i]=='\n') {
                    password[i]='\0';
                    break;
                }
            }
            strcat(reqstring,"register ");
            strcat(reqstring,&username);
            strcat(reqstring,", ");
            strcat(reqstring,&password);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
            if(*restype=='S'){
                system("cls");
                printf("You Have Successfully Registered as \" %s \"\n",username);
                printf("Press Enter To Continue...");
                getchar();
            }
            else {
                system("cls");
                printf("%s\n",resinside);
                printf("Press Enter To Continue...");
                getchar();
            }
        }
        else if (wtd==2){
            char *restype=(char *)malloc(5000*sizeof(char));
            char *resinside=(char *)malloc(5000*sizeof(char));
            memset(restype,0,sizeof(restype));
            system("cls");
            printf("Please Enter Your User-name : \n");
            for(int i=0;;i++){
                scanf("%c",&username[i]);
                if (username[i]=='\n') {
                    username[i]='\0';
                    break;
                }
            }
            printf("Please Enter Your Password : \n");
            for(int i=0;;i++){
                scanf("%c",&password[i]);
                if (password[i]=='\n') {
                    password[i]='\0';
                    break;
                }
            }
            strcat(reqstring,"login ");
            strcat(reqstring,&username);
            strcat(reqstring,", ");
            strcat(reqstring,&password);
            strcat(reqstring,"\n");
            Send_Req(reqstring,restype,resinside);
                if(*restype=='A'){
                system("cls");
                printf("Logined successfuly\n");
                printf("Press Enter To Continue...");
                memset(token,0,sizeof(token));
                strcat(token,resinside);
                //printf("%s\n",token);
                getchar();
                while(1){
                    system("cls");
                    memset(reqstring,0,sizeof(reqstring));
                    printf("1-Create Channel :\n");
                    printf("2-Join a Channel :\n");
                    printf("3-Sign-out :\n");
                    wtd2=0;
                    h=0;
                    h=getch();
                    while (h == 0xE0){
                        int h2;
                        h2=getch();
                        if (h2==80){
                            if (wtd2<3) wtd2++;
                            gotoxy(0,0);
                            system("cls");
                            printf("1-Create Channel :\n");
                            printf("2-Join a Channel :\n");
                            printf("3-Sign-out :\n");
                            gotoxy(18,wtd2-1);
                            SetColor(12);
                            printf("<-");
                            SetColor(0);
                        }
                        if (h2==72){
                            if (wtd2>1) wtd2--;
                            gotoxy(0,0);
                            system("cls");
                            printf("1-Create Channel :\n");
                            printf("2-Join a Channel :\n");
                            printf("3-Sign-out :\n");
                            gotoxy(18,wtd2-1);
                            SetColor(12);
                            printf("<-");
                            SetColor(0);
                        }
                        h=getch();
                    }
                    if (h!=0xE0 && h!=13 && h!=10)
                        if (h>=49 && h<=51) {
                            printf("%c",h);
                            wtd2=h-48;
                        }
                        else
                            wtd2=0;
                    char channame[MAX];
                    memset(channame,0,sizeof(channame));
                    if (wtd2==1){
                        system("cls");
                        printf("Enter Channel's Name : \n");
                        for(int i=0;;i++){
                            scanf("%c",&channame[i]);
                            if (channame[i]=='\n') {
                                channame[i]='\0';
                                break;
                            }
                        }
                        strcat(reqstring,"create channel ");
                        strcat(reqstring,&channame);
                        strcat(reqstring,", ");
                        strcat(reqstring,&token);
                        strcat(reqstring,"\n");
                        Send_Req(reqstring,restype,resinside);
                        if(*restype=='S'){
                            system("cls");
                            printf("You Have Successfully Joined \" %s \" Chat-Room\n",channame);
                            printf("Press Enter To Continue...");
                            getchar();
                            chat();
                        }
                        else {
                            system("cls");
                            printf("%s\n",resinside);
                            printf("Press Enter To Continue...");
                            getchar();
                        }
                    }
                    if (wtd2==2){
                            system("cls");
                            printf("Enter A Channel's Name : \n");
                            for(int i=0;;i++){
                                scanf("%c",&channame[i]);
                                if (channame[i]=='\n') {
                                    channame[i]='\0';
                                    break;
                                }
                            }
                            memset(reqstring,0,sizeof(reqstring));
                            strcat(reqstring,"join channel ");
                            strcat(reqstring,&channame);
                            strcat(reqstring,", ");
                            strcat(reqstring,&token);
                            strcat(reqstring,"\n");
                            Send_Req(reqstring,restype,resinside);
                            if(*restype=='S'){
                                system("cls");
                                printf("You Have Successfully Joined \" %s \" Chat-Room\n",channame);
                                printf("Press Enter To Continue...");
                                getchar();
                                chat();
                            }
                            else {
                                system("cls");
                                printf("%s\n",resinside);
                                printf("Press Enter To Continue...");
                                getchar();
                            }
                    }
                    if (wtd2==3){
                        memset(reqstring,0,sizeof(reqstring));
                        strcat(reqstring,"logout ");
                        strcat(reqstring,&token);
                        strcat(reqstring,"\n");
                        Send_Req(reqstring,restype,resinside);
                        break;
                    }
                }
            }
            else {
                system("cls");
                printf("%s\n",resinside);
                printf("Press Enter To Continue...");
                getchar();
            }
        }
        else if (wtd==3)break;
        system("cls");
    }
    system("cls");
    printf("!Bye-Bye!\n\n");
}
int Sock_Creator(){

    int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;

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
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else{
		printf("Successfully connected to the server..\n");
		return client_socket;
	}
	return 0;

}
