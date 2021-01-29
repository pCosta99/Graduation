#include "utilities.h"

long long code = 0;
long long next = 0;
int fifo;
pid_t pid, serverPid;
char serverPath[128];

void ctrl_alarm_handler(){
	exit(0);
}

char* initCommunication(void){
	int fdel;
	getServerPath(serverPath, serverPid);
	char* path = malloc(sizeof(char) * 64);
	signal(SIGALRM, ctrl_alarm_handler);
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
		write(2, "ma fifo\n", 8);
		return NULL;
	}
	alarm(3);
	if((fifo = open(serverPath, O_WRONLY)) == -1){
		write(2, "server fifo\n", 13);
		return NULL;
	}
	alarm(0);
	return path;
}

void closeCommunication(char* path){
	char command[256];
	
	snprintf(command, 256, "rm -f %s", path);

	system(command);
	close(fifo);
}

int calculate_occupation(void){
	struct stat sts;
	int fds = open(STRINGS, O_RDONLY);
	long long effectiveSize;
	long long totalSize;

	/* Read effective size from strings */
	read(fds, &effectiveSize, sizeof(effectiveSize));

	/* Obtain total file size */ 
	fstat(fds, &sts);
    totalSize = sts.st_size - sizeof(effectiveSize);

    /* Close descriptor */
    close(fds);

    /* Quit if any of the above values is 0 */
    if(totalSize == 0 || effectiveSize == 0) return 0;
    
    /* Check if 20% is garbage and return that boolean */
    return (((totalSize - effectiveSize) >= (0.2 * totalSize)) ? 1 : 0);
}

void compact(void){
	int fda = open(ARTIGOS, O_RDWR);
	int fds = open(STRINGS, O_RDWR);
	long long validPos, effectiveSize, newPos = sizeof(long long);

	/* Read effective size and initialize a buffer with that amount of space */
	read(fds, &effectiveSize, sizeof(effectiveSize));
	char *buffer = malloc(sizeof(char) * effectiveSize);
	memset(buffer, '\0', effectiveSize);

	while(read(fda, &validPos, sizeof(validPos)) > 0){
		/* Write new reference to artigos */
		lseek(fda, (-1) * sizeof(validPos), SEEK_CUR);
		write(fda, &newPos, sizeof(newPos));
		lseek(fda, sizeof(float), SEEK_CUR);
		/* Read name from strings and charge it to buffer */
		lseek(fds, validPos, SEEK_SET);
		/* Implementar name de maneira a que não rebente se o tamanho lido for superior a 1024 */
		char *name = malloc(sizeof(char) * 1024);
		memset(name, '\0', 1024);
		newPos += readln(fds, name, -1);
		strcat(buffer, name);
	}

	/* Reopen strings and unload buffer into it */
	close(fds);
	fds = open(STRINGS, O_WRONLY | O_TRUNC);
	write(fds, &effectiveSize, sizeof(effectiveSize));
	write(fds, buffer, effectiveSize);

	/* Set next as newPos */
	next = newPos;

	/* Close descriptors */
	close(fda);
	close(fds);
}

void insert(char* name, char* priceS){
	char out[256];
	float price = atof(priceS);
	int fda = open(ARTIGOS, O_WRONLY | O_APPEND);
	int fds = open(STRINGS, O_RDWR);
	int fdStocks = open(STOCKS, O_WRONLY | O_APPEND);
	long long bytesWritten = 0, effectiveSize = 0;

	/* Compact if needed */
	if(calculate_occupation()) compact();

	/* Output code */
	snprintf(out, 256, "Código -> %lld\n", code);
	write(1, out, strlen(out));

	/* Write reference and price to artigos */
	write(fda, &next, sizeof(next));
	write(fda, &price, sizeof(price));

	/* Write name to strings */
	strncat(name, "\n", 1);
	lseek(fds, 0, SEEK_END);
	bytesWritten = write(fds, name, strlen(name));
	next += bytesWritten;
	code++;

	/* Increment real number of effective bytes in the file */
	lseek(fds, 0, SEEK_SET);
	read(fds, &effectiveSize, sizeof(effectiveSize));
	lseek(fds, 0, SEEK_SET);
	effectiveSize += bytesWritten;
	write(fds, &effectiveSize, sizeof(effectiveSize));
	
	/* Initialize the value (as zero) in stocks */
	long long zero = 0;
	write(fdStocks, &zero, sizeof(zero));

	/* Close files */
	close(fda);
	close(fds);
	close(fdStocks);
}

void changeName(long long artCode, char* newName){
	int fda = open(ARTIGOS, O_RDWR);
	int fds = open(STRINGS, O_RDWR);
	long long effectiveSize = 0, bytesWritten = 0, artPos;
	int sizeOfOld;

	/* Read effective size and subtract the unused string */
	lseek(fda, artCode * (sizeof(long long) + sizeof(float)), SEEK_SET);
    read(fda, &artPos, sizeof(long long));
	read(fds, &effectiveSize, sizeof(effectiveSize));
    lseek(fds, artPos, SEEK_SET);
    sizeOfOld = readln(fds, NULL, -1);
    effectiveSize -= sizeOfOld;

	/* Write the new reference to artigos */
	lseek(fda, artCode * (sizeof(long long) + sizeof(float)), SEEK_SET);
	write(fda, &next, sizeof(next));

	/* Write name to strings */
	strncat(newName, "\n", 1);
	lseek(fds, 0, SEEK_END);
	bytesWritten = write(fds, newName, strlen(newName));
	next += bytesWritten;

	/* Update effectiveSize and write it back to strings */
	effectiveSize += bytesWritten;
	lseek(fds, 0, SEEK_SET);
	write(fds, &effectiveSize, sizeof(effectiveSize));

	/* Compact if needed */
	if(calculate_occupation()) compact();

	/* Close files */
	close(fda);
	close(fds);
}

void changePrice(long long artCode, float newPrice){
	char out[256];
	int fda = open(ARTIGOS, O_WRONLY);
	int fifo;

	/* Write the new reference to artigos */
	lseek(fda, (artCode * (sizeof(long long) + sizeof(float))) + sizeof(long long), SEEK_SET);
	write(fda, &newPrice, sizeof(float));
	/* Signal price change to sale server */
	if ((fifo = open(serverPath, O_WRONLY)) > 0) {
		snprintf(out, 128, "p %lld %f\n", artCode, newPrice);
		write(fifo, out, strlen(out));
    }

	/* Close files */
	close(fda);
	close(fifo);
}

int parser(char* buffer){
	int size;
	char** command = words(buffer, &size);

	if (!strcmp(buffer, "quit") || !strcmp(buffer, "q")) 
		return 1;


	if (!strcmp(buffer, "a")){
		char out[256], *path;
		int communicationFifo;
		path = malloc(sizeof(char) * 256);
		if((path = initCommunication()) == NULL) return -1;
		memset(out, '\0', 256);
		snprintf(out, 256, "agregate %s\n", path);
		write(fifo, out, strlen(out)+1);
		/* Testar valor mais apropriado */
		usleep(1);
		memset(out, '\0', 256);
		if((communicationFifo = open(path, O_RDONLY)) == -1){
				write(2, "reading from client fifo\n", 25);
			} else {
				readln(communicationFifo, out, 256);
				if (out[0] != 'I') write(1, out, 256);
				close(communicationFifo);
		}
		closeCommunication(path);
		free(path);
	} else if(!strcmp(command[0],"i")) {
		insert(command[1], command[2]);
	} else if(!strcmp(command[0],"r")) {
		long long artCode = atoll(command[1]);
		if(artCode < code) {
			struct artigo a = readArtigo(artCode);
			display(a);
		}
		else write(1, "That code doesn't exists.\n", 26);
	} else if(!strcmp(command[0],"n")) {
		long long artCode = atoll(command[1]);
		if(artCode < code) changeName(artCode, command[2]);
		else write(1, "That code doesn't exists.\n", 26);
	} else if(!strcmp(command[0],"p")) {
		long long artCode = atoll(command[1]);
		float newPrice = atof(command[2]);
		if(artCode < code) changePrice(artCode, newPrice);
		else write(1, "That code doesn't exists.\n", 26);
	} else {
		write(1, "Wrong format.\n", 14);
	}

	for(int i = 0; command[i]; i++) free(command[i]);
	free(command);

	return 0;
}

int main(void){
	int bufferSize = 1024, offset = 0;
	char *buffer = malloc(sizeof(char) * bufferSize);
	int n;

	/* Read global values */
	getGlobals(&code, &next);

	/* Read cicle */
	while((n = readln(0, buffer + offset, bufferSize - offset)) > 0){
		offset += n;
		if ((offset && buffer[offset-1] == '\n') || buffer[n-1] == '\n') {
			buffer[offset-1] = '\0';
			if (buffer[0] == '\0' || parser(buffer)) break;
			offset = 0;
			memset(buffer, '\0', 1024);
		} else {
			buffer = realloc(buffer, bufferSize * 2);
            bufferSize *= 2;
		}
	}

	free(buffer);

	return 0;
}