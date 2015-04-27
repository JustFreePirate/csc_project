#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

const int COUNT_SERVO = 1;
const int ALL_RIGHT = 0;
const int EXIT_CODE = 1;
const int WRONG_COMMAND = 2;

int open_port(void)
{
    int fd; /* file descriptor for the port */
    
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY );
    if (fd == -1)
    {
        /*
        * could not open the port.
        */
        
        perror("open_port: unable to open /dev/ttyUSB0");
    }
    else {
        fcntl(fd, F_SETFL, 0);
        struct termios options; /*структура для установки порта*/
        tcgetattr(fd, &options); /*читает пораметры порта*/
        
        cfsetispeed(&options, B9600); /*установка скорости порта*/
        cfsetospeed(&options, B9600); /*установка скорости порта*/
        
        options.c_cflag &= ~PARENB; /*выкл проверка четности*/
        options.c_cflag &= ~CSTOPB; /*выкл 2-х стобит, вкл 1 стопбит*/
        options.c_cflag &= ~CSIZE; /*выкл битовой маски*/
        options.c_cflag |= CS8; /*вкл 8бит*/
        options.c_cc[VMIN] = 1;
        options.c_cc[VTIME] = 0;
        tcsetattr(fd, TCSANOW, &options); /*сохронения параметров порта*/
    }
    return (fd);
}

int checkString(const char * s) {
    string str(s);
    if (str == "exit")
        return EXIT_CODE;
    else {
        long num, deg;
        char *pos;
        num = strtol(s, &pos, 10);
        deg = strtol(pos, &pos, 10);
        if (num >= 1 && num <= COUNT_SERVO && deg >= 0 && deg <= 180 && strlen(pos) == 0)
            return ALL_RIGHT;
        else
            return WRONG_COMMAND;
    }
    return 0;
}

char s[255];



int main(int argc, char **argv) {
    if (argc > 2) {
        cout << "Usage: [ name_of_file ]" << endl;
    }
    if (argc == 2) {
        if (!freopen(argv[1], "r", stdin)) {
            cout << "Cannot read this script file" << endl;
            return 1;
        }
    }
    
    int fd = open_port();
    int code,i;
    string str;
    while (1) {
        cout << ">> ";
        i = 0;
        
        s[i] = getchar();
        while (s[i] != '\n' && s[i] != EOF)
            s[++i] = getchar();
        if (s[i] == EOF) {
            cout << "good bye!" << endl;
            return 0;
        }
            
       // while ((s[i++] = getchar()) != '\n');
        s[i] = '\0';
        code = checkString(s);
        switch (code) {
            case ALL_RIGHT:
                //cout << "OK" << endl; 
                cout << s << endl;
                write(fd, s , i);
                sleep(2);
                break;
            
            case WRONG_COMMAND:
                cout << "Wrong command" << endl;
                break;
                
            case EXIT_CODE: 
                cout << "Good bye!" << endl;
                return 0;
        }
        
        /*
        while (read(fd,&c,1) && c!='\n')
            cout << c;
        cout << endl;
        */
    }
}
