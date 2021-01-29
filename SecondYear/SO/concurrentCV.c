#include "utilities.h"

int fifo, myfifo, flag = 0;
pid_t pid, serverPid;
char keepPath[128];
char serverPath[128];

void closeCommunication(char* path){
	unlink(path);
	close(fifo);
}

void ctrl_c_handler(){
	closeCommunication(keepPath);
	close(myfifo);
	exit(0);
}

void ctrl_alarm_handler(){
	closeCommunication(keepPath);
	close(myfifo);
	exit(0);
}

void sigusr1_handler(){
	flag = 1;
}

char* initCommunication(void){
	int fdel;
	/* Prepare signals */
	signal(SIGALRM, ctrl_alarm_handler);
	signal(SIGINT, ctrl_c_handler);
	signal(SIGUSR1, sigusr1_handler);

	getServerPath(serverPath, serverPid);

	char* path = malloc(sizeof(char) * 64);
	memset(path, '\0', 64);
	pid = getpid();
	snprintf(path, 64, "/tmp/fifo%ld", (long) pid);
	if((fdel = open(ERRORS, O_WRONLY)) == -1){
		write(2, "Rip error logging.\n", 19);
		return NULL;
	} else {
		dup2(fdel, 2);
		close(fdel);
	}
	if (mkfifo(path, 0777) == -1){
		write(2, "creating client fifo\n", 21);
		return NULL;
	}
	alarm(3);
	if((fifo = open(serverPath, O_WRONLY)) == -1){
		alarm(0);
		write(2, "server fifo\n", 13);
		return NULL;
	} else {
		alarm(0);
		char out[256];
		sprintf(out, "init %s\n", path);
		write(fifo, out, strlen(out));
	}

	if((myfifo = open(path, O_RDWR)) == -1){
		write(2, "opening client fifo\n", 20);
		return NULL;
	} else {	
		printf("Taking a nap.\n");
		usleep(10);
		read(myfifo, &serverPid, sizeof(serverPid));
		printf("Read %d\n", serverPid);
	}

	strcpy(keepPath, path);

	return path;
}

int safeConversion(const char *buff, int *num, float *f, int flag) {
  char *end;
  char buffer[1024];
 
  errno = 0;
 
  const long sl = strtol(buff, &end, 10);
 
  if (end == buff) {
    snprintf(buffer, 1024, "%s: not a decimal number\n", buff);
    write(2, buffer, strlen(buffer));
    memset(buffer, '\0', 1024);
  } else if ('\0' != *end) {
    snprintf(buffer, 1024, "%s: extra characters at end of input: %s\n", buff, end);
    write(2, buffer, strlen(buffer));
    memset(buffer, '\0', 1024);
  } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
    snprintf(buffer, 1024, "%s out of range of type long\n", buff);
    write(2, buffer, strlen(buffer));
    memset(buffer, '\0', 1024);
  } else if (sl > INT_MAX) {
    snprintf(buffer, 1024, "%ld greater than INT_MAX\n", sl);
    write(2, buffer, strlen(buffer));
    memset(buffer, '\0', 1024);
  } else if (sl < INT_MIN) {
     snprintf(buffer, 1024, "%ld less than INT_MIN\n", sl);
     write(2, buffer, strlen(buffer));
     memset(buffer, '\0', 1024);
  } else {
    if (flag) (*num) = (int)sl;
    else {
  		const double sd = strtod(buff, &end);
    	(*f) = (float)sd;
    }
    return 0;
  }
  return 1;
}

int parser(char* buffer, char* path){
	if (!strcmp(buffer, "quit") || !strcmp(buffer, "q")) 
		return 1;

	if (!strcmp(buffer, "shutdown")){
		write(fifo, "shutdown\n", 9);
		return 2;
	}

	int size;
	char** command = words(buffer, &size);

	if(!strcmp(buffer, "reload") || command[0][0] == 'r') {
		write(fifo, "reload\n", 7);
	} else if (size == 1){
		int artCode;
		if(safeConversion(command[0], &artCode, NULL, 1)) {
			write(2, "Bad input!\n", 11);
		} else {
			char out[256];
			memset(out, '\0', 256);
			snprintf(out, 256, "%s\n", command[0]);
			write(myfifo, out, strlen(out)+1);
			memset(out, '\0', 256);
			kill(serverPid, SIGUSR1);
			pause();
			read(myfifo, out, 256);
			write(1, out, 256);
		}
	} else if (size == 2) {
		int artCode;
		float price;
		if(safeConversion(command[0], &artCode, NULL, 1) || safeConversion(command[1], NULL, &price, 0)) {
			write(2, "Bad input!\n", 11);
		} else {
			char out[256];
			memset(out, '\0', 256);
			snprintf(out, 256, "%s %s\n", command[0], command[1]);
			write(myfifo, out, strlen(out)+1);
			memset(out, '\0', 256);
			kill(serverPid, SIGUSR1);
			pause();
			read(myfifo, out, 256);
			write(1, out, 256);
		}
	}

	for(int i = 0; command[i]; i++) free(command[i]);
	free(command);

	return 0;
}

int main(void){
	int n, bufferSize = 1024, offset = 0;
	char *path, *buffer = malloc(sizeof(char) * bufferSize);

	if((path = initCommunication()) == NULL) return -1;

	/* Read cicle */
	while((n = readln(0, buffer + offset, bufferSize - offset)) > 0){
		offset += n;
		if ((offset && buffer[offset-1] == '\n') || buffer[n-1] == '\n') {
			buffer[offset-1] = '\0';
			if (buffer[0] == '\0' || parser(buffer, path)) break;
			offset = 0;
			memset(buffer, '\0', bufferSize);
		} else {
			buffer = realloc(buffer, bufferSize * 2);
            bufferSize *= 2;
		}
	}

	closeCommunication(path);

	free(path);
	free(buffer);

	return 0;
}