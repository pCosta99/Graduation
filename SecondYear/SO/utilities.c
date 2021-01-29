#include "utilities.h"

char** words(const char* line, int *size){
    char* input = strdup(line);
    size_t word_count = 1;
    while (*line) {
        if (*line == ' ') {
            while (*line && *line == ' ') {
                line++;
            }
            word_count++;
        } else {
            line++;
        }
    }
    char** words = malloc((word_count + 1) * sizeof(char*));
    size_t word = 0;
    for(char* token = strtok(input, " "); token; token = strtok(NULL, " ")) {
        words[word++] = strdup(token);
    }
    words[word] = NULL;
    (*size) = word;
    free(input);
    return words;
}

int mypopen(char* command, char* type) {
    int pfd[2];
    int size;
    char** argv = words(command, &size);

    if(pipe(pfd) == -1){
        perror("pipe");
        return -1;
    }

    if(!fork()){
        if(type[0] == 'r') dup2(pfd[1], 1);
        else if(type[0] == 'w') dup2(pfd[0], 0);
        else _exit(1);

        close(pfd[0]);
        close(pfd[1]);
        execvp(argv[0], argv);
        _exit(1);
    }

    if(type[0] == 'r'){
        close(pfd[1]);
        return pfd[0];
    }
    else if (type[0] == 'w'){
        close(pfd[0]);
        return pfd[1];
    }

    return -1;
}

/* bytes == -1 => Reads until newline */
/* buf == NULL => Doesn't read, only counts how many bytes until a newline */
ssize_t readln(int fildes, void *buf, size_t bytes) {
    ssize_t size = 0;
    char ch;
    char * buff = (char*) buf;

    while((size < bytes || bytes == -1) && read(fildes, &ch, 1) == 1) {
        if (ch == '\0')
            return size;

        if (buff != NULL) {
            buff[size++] = ch;
        } else size++;

        if (ch == '\n')
            return size;
    }

    return size;
}

CircularArray* initCA(int size){
    CircularArray *c = malloc(sizeof(struct circularArray));
    c->size = size;
    c->currentPos = 0;
    c->entries = malloc(sizeof(struct entry) * c->size);
    for(int i = 0; i < c->size; i++) {
        c->entries[i].flag = 0;
    }
    return c;
}

/* Regular insertion */
void insertCA(CircularArray *c, int artCode, float price){
    if(c->currentPos == c->size) c->currentPos = 0;
    c->entries[c->currentPos].artCode = artCode;
    c->entries[c->currentPos].price = price;
    c->entries[c->currentPos++].flag = 1;
}

/* Agregator insertion */
void insertCAintoAG(CircularArray *c, int code, float totalSold, int artQuantity){
    int i = 0;
    for(i = 0; i < c->size && c->entries[i].flag; i++){
        if(c->entries[i].artCode == code){
            c->entries[i].total += totalSold;
            c->entries[i].quantity += artQuantity;
            break;
        }
    }
    if(i == c->size){
        c->entries = realloc(c->entries, c->size * 2);
        c->size *= 2;
        c->entries[i].artCode = code;
        c->entries[i].total += totalSold;
        c->entries[i].quantity += artQuantity;
        c->entries[i].flag = 1;
    } else if(!c->entries[i].flag){
        c->entries[i].artCode = code;
        c->entries[i].total += totalSold;
        c->entries[i].quantity += artQuantity;
        c->entries[i].flag = 1;
    }
}

float findCA(CircularArray *c, int artCode){
    float ret = -1;

    for(int i = 0; ret == -1 && i < c->size && c->entries[i].flag; i++){
        if(c->entries[i].artCode == artCode)
            ret = c->entries[i].price;
    }

    return ret;
}

void freeCircularArray(CircularArray *c){
    free(c->entries);
    free(c);
}

/* Control options:
0 -> Binary mode
1 -> Readable mode */
void flushCircularArray(CircularArray *c, int control){
    char out[256];
    for(int i = 0; i < c->size && c->entries[i].flag; i++){
        if(control){
            sprintf(out, "Code: %d\nQuantity: %d\nTotal: %f\n", c->entries[i].artCode, c->entries[i].quantity, c->entries[i].total);
            write(1, out, strlen(out));
            memset(out, '\0', 256);
        } else {
            struct venda v;
            v.code = c->entries[i].artCode;
            v.quantity = c->entries[i].quantity;
            v.total = c->entries[i].total;
            write(1, &v, sizeof(struct venda));
        }
    }
}

char* timeToString(void){
    char *out = malloc(sizeof(char) * 256);
    char day[7] = "DSTQQSS";
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(out, "%d-%d-%d%c%d:%d:%d", tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,day[tm->tm_wday],tm->tm_hour,tm->tm_min,tm->tm_sec);
    return out;
}

int getGlobals(long long *code, long long *next){
    struct stat sta, std;
    int fda = open(ARTIGOS, O_RDONLY);
    int fds = open(STRINGS, O_RDWR);
    long long sizeA = 0, sizeS = 0;
    long long effectiveSize = 0;

    if(fda < 0){
        write(2, "Could not open artigos.\n", 24);
        return -1;
    }
    if(fds < 0){
        write(2, "Could not open strings.\n", 24);
        return -2;
    }

    /* Check if effective size of strings is already in place */
    if(read(fds, &effectiveSize, sizeof(effectiveSize)) <= 0){
        write(fds, &effectiveSize, sizeof(effectiveSize));
    }

    /* Get code and next */
    fstat(fda, &sta);
    sizeA = sta.st_size;
    (*code) = sizeA / (sizeof(long long) + sizeof(float));
    fstat(fds, &std);
    sizeS = std.st_size;
    (*next) = sizeS;

    /* Close file */
    close(fda);
    close(fds);

    return 0;
}

void readVenda(struct venda v){
    char out[256];
    memset(out, '\0', 256);

    sprintf(out, "Código: %d\nQuantidade: %d\nTotal: %.2f\n", v.code, v.quantity, v.total);
    write(1, out, 256);
}

struct artigo readArtigo(long long artCode){
    int lastBit;
    int fda = open(ARTIGOS, O_RDONLY);
    int fds = open(STRINGS, O_RDONLY);
    long long artPos;
    struct artigo a;

    lseek(fda, artCode * (sizeof(long long) + sizeof(float)), SEEK_SET);
    read(fda, &artPos, sizeof(long long));
    read(fda, &a.price, sizeof(float));
    lseek(fds, artPos, SEEK_SET);
    lastBit = readln(fds, &a.name, -1);
    a.name[lastBit-1] = '\0';

    a.code = artCode;

    close(fda);
    close(fds);

    return a;
}

void display(struct artigo art){
    char* nameOut = malloc(sizeof(char) * NAME_SIZE);
    char priceOut[12];
    char codeOut[12];

    strcpy(nameOut, art.name);
    sprintf(priceOut, "%.2f", art.price);
    sprintf(codeOut, "%lld", art.code);

    write(1, "Nome: ", 6);
    write(1, nameOut, strlen(nameOut));
    PUT_LINE;
    write(1, "Preço: ", 8);
    write(1, priceOut, strlen(priceOut));
    PUT_LINE;
    write(1, "Código: ", 9);
    write(1, codeOut, strlen(codeOut));
    PUT_LINE;
}

pid_t getPidFromPath(char* path){
    char pid[10];
    int i, ret;

    /* Find the first digit */
    for(i = 0; path[i] <= 47 || path[i] >= 58; i++) 
        ;

    /* Copy all digits into string pid */
    for(int j = 0; path[i]; j++)
        pid[j] = path[i++];

    /* Convert to pid_t */
    ret = atoi(pid);

    return (pid_t) ret;
}

void getServerPath(char* serverPath, pid_t serverPid){
    char buffer[1024];
    int n;
    int ls = mypopen("ls", "r");
    int temp = open("tempFile", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    int saveStdOut = dup(1);
    dup2(temp, 1);
    close(temp);
    int grep = mypopen("grep pip", "w");

    /* Read result from ls */
    while((n = read(ls, buffer, 1024)) > 0){
        write(grep, buffer, n);
    }

    /* Close descriptors */
    close(ls);
    close(grep);

    /* Wait for grep */
    usleep(50000);

    /* Read name from file */
    temp = open("tempFile", O_RDONLY);
    n = read(temp, serverPath, 128);
    serverPath[n-1] = '\0';

    /* Save the server pid */
    if(serverPath[0]) serverPid = getPidFromPath(serverPath);

    /* Remove the redirection */
    dup2(saveStdOut, 1);
    close(saveStdOut);

    /* Close temp and unlink it */
    close(temp);
    unlink("tempFile");
}