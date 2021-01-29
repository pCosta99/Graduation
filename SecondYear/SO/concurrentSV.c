#include "utilities.h"

long long code = 0;
long long next = 0;
/* Guarda a última posição em que o agregador foi chamado */
long long lastRan = 0;
pid_t pidCliente;
int input, nextClient, flag = 0;
char serverFifo[128];
char serverClients[10][256];
int clientPid[10];
CircularArray *circ;

void closeServer(void){
	freeCircularArray(circ);

	close(input);
	unlink(serverFifo);
}

void alrm_handler(){
	kill(pidCliente, SIGUSR1);
	ualarm(1, 0);
}

void ctrl_c_handler(){
	closeServer();
}

void sigusr1_handler(){
	flag = 1;
}

int controlAgregation(void){
	struct stat stv;
    int fdv = open(VENDAS, O_RDWR);
    int sizeV = 0;

    if(fdv < 0){
        write(2, "Could not open vendas.\n", 24);
        return -1;
    }

    fstat(fdv, &stv);
    sizeV = stv.st_size;
    if(!sizeV){
    	/* Initialize the value (as sizeof(lastRan)) in stocks */
		long long sizeOfLastRan = sizeof(lastRan);
		write(fdv, &sizeOfLastRan, sizeof(sizeOfLastRan));
		lastRan = sizeOfLastRan;
    } else {
    	read(fdv, &lastRan, sizeof(lastRan));
    }

    return 0;
}

int initServer(void){
	int fdel;
	sprintf(serverFifo, "pip%d", getpid());

	getGlobals(&code, &next);
	controlAgregation();
	signal(SIGUSR1, sigusr1_handler);
	signal(SIGALRM, alrm_handler);
	circ = initCA(CACHE);
	nextClient = 0;

	if((fdel = open(ERRORS, O_WRONLY)) == -1){
		write(2, "Rip error logging.\n", 19);
		return -2;
	} else {
		dup2(fdel, 2);
		close(fdel);
	}

	system("rm pip*");
	mkfifo(serverFifo, 0777);

	if ((input = open(serverFifo, O_RDONLY))  == -1) {
		write(2, "Rip pipe.\n", 10);
		return -1;
	}

	return 0;
}

int calculateNumberOfForks(long long sizeToAgregate){
	return ((sizeToAgregate / 12) < 12 ? (int) sizeToAgregate / 12 : 12);
}

void calculatePartion(long long sizeToAgregate, int numberOfForks, int *partition){
	int numberOfSales = sizeToAgregate/sizeof(struct venda);
	int i, j = 0;
	for(i = 0; i < numberOfSales; i++){
		if(j == numberOfForks) j = 0;
		partition[j]++;
		j++;
	}
}

void multipleAgregation(char *path){
	struct stat stv;
	int fdv = open(VENDAS, O_RDWR);
	int status, numberOfForks, fdOut;
	long long sizeToAgregate;
	char finalPath[128], fifoPath[128];

	/* Create path */
    sprintf(finalPath, "Sales/%s", timeToString());

	/* Go to the point where the last agregation happened */
	lseek(fdv, lastRan, SEEK_SET);

	/* Calculate size to agregate and distribute it through the buffer. Set lastRan to EOF */
	fstat(fdv, &stv);
    sizeToAgregate = stv.st_size - lastRan;
    lastRan = stv.st_size;
    numberOfForks = calculateNumberOfForks(sizeToAgregate);

    /* Create buffer's to read the sales and an array of pids to store the forks */
    struct venda** buffer = (struct venda**) malloc((sizeof(struct venda*) * numberOfForks));
	int allPids[numberOfForks];

   	/* If there's nothing to agregate quit */
   	if((fdOut = open(path, O_WRONLY)) == -1){
		write(2, "couldn't open client fifo\n", 29);
		return;
	} else if(sizeToAgregate == 0){
   		write(fdOut, "No sales to agregate since last time.\n", 38);
   		return;
   	}

    /* Perform some arithmetic to divide the charge into the numberOfForks */
    int partition[numberOfForks];
    memset(partition, 0, numberOfForks * sizeof(int));
    calculatePartion(sizeToAgregate, numberOfForks, partition);
    for(int i = 0; i < numberOfForks; i++){
    	int j = 0;
    	buffer[i] = (struct venda*) malloc(partition[i] * sizeof(struct venda));
    	while(j < partition[i]){
    		read(fdv, &buffer[i][j++], sizeof(struct venda));
		}
    }

    /* Launch several forks which will run each part of the buffer separately */
    for(int i = 0; i < numberOfForks; i++){
    	if(!(allPids[i] = fork())){
    		int j = 0, fifo;
    		/* Create a fifo to receive the agregator output */
    		memset(fifoPath, '\0', 128);
    		sprintf(fifoPath, "/tmp/holder%d", getpid());
    		mkfifo(fifoPath, 0777);
    		/* Open fifo and redirect stdout to it */
    		if((fifo = open(fifoPath, O_WRONLY | O_APPEND)) == -1){
    			write(2, "couldn't create holder fifo\n", 29);
    		}
    		dup2(fifo, 1);
    		close(fifo);
    		int fd = mypopen("./ag", "w");
    		/* Pass information to agregator */
    		while(j < partition[i]){
    			write(fd, &buffer[i][j++], sizeof(struct venda));
			}
    		close(fd);
			wait(&status);
    		_exit(1);
    	}
    }

    /* Minor sleep to give time for forks to run */
    sleep(2);

    /* Read all fork results */
    int j = 0, sizeMax = 2048;
    struct venda* readTo = malloc(sizeof(struct venda) * sizeMax);
    for(int i = 0; i < numberOfForks; i++){
    	int readFrom;
    	memset(fifoPath, '\0', 128);
    	sprintf(fifoPath, "/tmp/holder%d", allPids[i]);
    	if((readFrom = open(fifoPath, O_RDONLY)) == -1){
    		write(2, "couldn't open holder fifo\n", 29);
    	} else {
    		while(read(readFrom, &readTo[j], sizeof(struct venda))){
	    		if(j == sizeMax){
	    			sizeMax *= 2;
	    			readTo = (struct venda*) realloc(readTo, sizeMax);
	    		}
	    		j++;
    		}
    	}
    	close(readFrom);
    }

    /* Catch all child processes */
    while (wait(&status) != -1) ;

    /* Open an anonymous pipe to read the sub-agregated sales to the final instance of the agregator */
    int writeTo = open(finalPath, O_CREAT | O_WRONLY | O_APPEND);
    dup2(writeTo, 1);
    close(writeTo);
    int fd = mypopen("./ag", "w");
    for(int i = 0; i < j; i++){
    	write(fd, &readTo[i], sizeof(struct venda));
    }

    /* Remove fifo's */
    system("rm -f /tmp/holder*");

    /* Inform client and update lastRan on file */
    lseek(fdv, 0, SEEK_SET);
	write(fdv, &lastRan, sizeof(lastRan));
	write(fdOut, "Agregation sucessful.\n", 22);

    /* Free stuff */
    free(readTo);
    for(int i = 0; i < numberOfForks; i++) free(buffer[i]);
    free(buffer);

    /* Close descriptors */
    close(writeTo);
    close(fd);
}

int process(int artCode, int quantity, char *path, pid_t clientPid){
	struct artigo art;
	struct venda v;
	char out[256];
	int fds = open(STOCKS, O_RDWR);
	int fdv = open(VENDAS, O_WRONLY | O_APPEND);
	int flag = 0, fdOut;
	long long inStock;
	float price;

	/* Open output descriptor */
	if((fdOut = open(path, O_WRONLY)) == -1){
		write(2, "couldn't open client fifo\n", 26);
		return -2;
	}

	/* Read the amount in stock */
	lseek(fds, artCode * sizeof(long long), SEEK_SET);
	read(fds, &inStock, sizeof(long long));

	/* Calculate new stock amount and write it on the file */
	if(inStock + quantity >= 0) {
		inStock += quantity;
		lseek(fds, 0, SEEK_SET);
		lseek(fds, artCode * sizeof(long long), SEEK_SET);
		write(fds, &inStock, sizeof(inStock));

		/* Check if it was a sale */
		if(quantity < 0){
			/* Create a sale and write it to file */
			v.code = artCode;
			v.quantity = quantity * (-1);
			if((price = findCA(circ, artCode)) == -1) {
				art = readArtigo(artCode);
				insertCA(circ, artCode, art.price);
				v.total = v.quantity * art.price;
			}
			else v.total = v.quantity * price;
			write(fdv, &v, sizeof(struct venda));
		}
	} else {
		snprintf(out, 256, "Not enough amount in stock.\n");
		flag = -1;
	}

	/* Prepare message to be displayed */
	if (!flag) snprintf(out, 256, "Stock: %lld\n", inStock);
	write(fdOut, out, strlen(out)+1);

	/* Close files */
	close(fds);
	close(fdv);
	close(fdOut);

	return flag;
}

void checkStock(int artCode, char *path, pid_t clientPid){
	char out[256];
	struct artigo art = readArtigo(artCode);
	int fds = open(STOCKS, O_RDONLY), fdOut;
	long long inStock;
	float endPrice;

	/* Open output descriptor */
	if((fdOut = open(path, O_WRONLY)) == -1){
		write(2, "couldn't open client fifo\n", 26);
		return;
	}

	/* Read the amount in stock */
	lseek(fds, artCode * sizeof(long long), SEEK_SET);
	read(fds, &inStock, sizeof(long long));

	/* Prepare message to be displayed and insert the product */
	if((endPrice = findCA(circ, artCode)) == -1) {
		art = readArtigo(artCode); 
		insertCA(circ, artCode, art.price); 
		endPrice = art.price;
	}
	snprintf(out, 256, "Stock: %lld\nPrice: %.2f\n", inStock, endPrice);
	write(fdOut, out, strlen(out)+1);

	/* Close files */
	close(fds);
	close(fdOut);
}

int parser(char* buffer, char *path, pid_t clientPid){
	int size;
	char** command = words(buffer, &size);

	if (!strcmp(command[0], "shutdown")) 
		return 1;

	if(!strcmp(buffer, "reload") || command[0][0] == 'r') {
		getGlobals(&code, &next);
		write(1, "Values reloaded.\n", 17);
		return 2;
	}

	if(!strcmp(command[0], "p")){
		circ = initCA(CACHE);
	} else if (size == 1){
		getGlobals(&code, &next);
		long long artCode = atoll(command[0]);
		if(artCode < code) checkStock(artCode, path, clientPid);
		else {
			int fdOut;
			if((fdOut = open(path, O_RDWR)) == -1){
				write(2, "couldn't open client fifo\n", 26);
				return -1;
			}
			write(fdOut, "Invalid code!\n", 14);
			close(fdOut);
		}
	} else if (size == 2){
		getGlobals(&code, &next);
		long long artCode = atoll(command[0]);
		long long quantity = atoll(command[1]);
		if(artCode < code) process(artCode, quantity, path, clientPid);
		else {
			int fdOut;
			if((fdOut = open(path, O_RDWR)) == -1){
				write(2, "couldn't open client fifo\n", 26);
				return -1;
			}
			write(fdOut, "Invalid code!\n", 14);
			close(fdOut);
		}
	}

	for(int i = 0; command[i]; i++) free(command[i]);
	free(command);

	return 0;
}

void closeClientReader(int clientDescriptor){
	close(clientDescriptor);
}

int initClientReader(int *clientDescriptor, char *path, pid_t *clientPid){
	pid_t forkPid = getpid();
	if(((*clientDescriptor) = open(path, O_RDWR)) == -1){
		write(2, "couldn't open client fifo\n", 26);
		return -1;
	}
	(*clientPid) = getPidFromPath(path);
	write(*clientDescriptor, &forkPid, sizeof(forkPid));
	signal(SIGALRM, alrm_handler);
	return 0;
}

int firstPass(char *buffer){
	int size;
	char** command = words(buffer, &size);

	if(!strcmp(command[0], "init")){
		if (!(clientPid[nextClient++] = fork())){
			int bufferSize = 1024, offset = 0, clientDescriptor;
			char *buffer = malloc(sizeof(char) * bufferSize);
			char *path = malloc(sizeof(char) * 128);
			strcpy(path, command[1]);

			if(initClientReader(&clientDescriptor, path, &pidCliente)){
				free(buffer);
				free(path);
				_exit(0);
			}

			memset(buffer, '\0', bufferSize);

			while(1){
				while(!flag);
				flag = 0;
				int n = readln(clientDescriptor, buffer + offset, bufferSize - offset);
				offset += n;
				if(n > 0){
					if ((offset && buffer[offset-1] == '\n') || buffer[n-1] == '\n'){
						buffer[offset-1] = '\0';
						if (buffer[0] == '\0' || parser(buffer, path, pidCliente)) break;
						else kill(pidCliente, SIGUSR1);
						offset = 0;
						memset(buffer, '\0', bufferSize);
					} else {
						buffer = realloc(buffer, bufferSize * 2);
						bufferSize *= 2;
					}
				}
			}

			free(buffer);
			free(path);
			closeClientReader(clientDescriptor);
			_exit(0);
		}
	} else if(!strcmp(command[0], "agregate")){
		multipleAgregation(command[1]);
	} 

	for(int i = 0; command[i]; i++) free(command[i]);
	free(command);

	return 0;
}

int main(void){
	int bufferSize = 1024, offset = 0;
	char *buffer = malloc(sizeof(char) * bufferSize);

	initServer();

	while(1) {
		int n = readln(input, buffer + offset, bufferSize - offset);
		offset += n;
		if(n > 0) {
			if ((offset && buffer[offset-1] == '\n') || buffer[n-1] == '\n') {
				buffer[offset-1] = '\0';
				if (buffer[0] == '\0' || firstPass(buffer)) break;
				offset = 0;
				memset(buffer, '\0', bufferSize);
			} else {
				buffer = realloc(buffer, bufferSize * 2);
	            bufferSize *= 2;
			}
		} 
	}

	free(buffer);
	closeServer();

	return 0;
}