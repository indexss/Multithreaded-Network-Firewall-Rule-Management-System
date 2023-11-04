/* A threaded server in the internet domain using TCP
   The port number is passed as an argument */
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERLENGTH 256

#define THREAD_IN_USE 0
#define THREAD_FINISHED 1
#define THREAD_AVAILABLE 2
#define THREADS_ALLOCATED 10

struct Node *head = NULL;
pthread_mutex_t nodeMutex = PTHREAD_MUTEX_INITIALIZER;

char **splitString(char *input, char delimiter, int *count) {
    char *inputCopy = strdup(input);

    char **result = NULL;
    *count = 0;

    char *token = strtok(inputCopy, &delimiter);

    while (token != NULL) {
        result = (char **)realloc(result, (*count + 1) * sizeof(char *));
        result[*count] = strdup(token);
        (*count)++;

        token = strtok(NULL, &delimiter);
    }

    free(inputCopy);

    return result;
}

struct StringNode {
    char *data;
    struct StringNode *next;
};


struct Node {
    int ip1[4];
    int ip2[4];
    int port1;
    int port2;
    struct StringNode *string_list; 
    struct Node *next;
};

struct StringNode *createStringNode(char *data) {
    struct StringNode *newNode =
        (struct StringNode *)malloc(sizeof(struct StringNode));
    if (newNode == NULL) {
        printf("malloc fail!\n");
        exit(1);
    }
    newNode->data = (char *)malloc(strlen(data) + 1);
    if (newNode->data == NULL) {
        printf("malloc fail!\n");
        exit(1);
    }
    strcpy(newNode->data, data);
    newNode->next = NULL;
    return newNode;
}


struct Node *createNode(int ip1[4], int ip2[4], int port1, int port2,
                        char *data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("malloc fail!\n");
        exit(1);
    }

    memcpy(newNode->ip1, ip1, sizeof(int) * 4);
    memcpy(newNode->ip2, ip2, sizeof(int) * 4);
    newNode->port1 = port1;
    newNode->port2 = port2;
    newNode->string_list = createStringNode(data);
    newNode->next = NULL;

    return newNode;
}


void insertStringNode(struct StringNode **head, char *data) {
    struct StringNode *newNode = createStringNode(data);

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct StringNode *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


void insertNode(struct Node **head, int ip1[4], int ip2[4], int port1,
                int port2, char *data) {
    struct Node *newNode = createNode(ip1, ip2, port1, port2, data);

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


void printStringList(struct StringNode *head) {
    struct StringNode *current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}


void printList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("IP1: %d.%d.%d.%d\n", current->ip1[0], current->ip1[1],
               current->ip1[2], current->ip1[3]);
        printf("IP2: %d.%d.%d.%d\n", current->ip2[0], current->ip2[1],
               current->ip2[2], current->ip2[3]);
        printf("Port1: %d\n", current->port1);
        printf("Port2: %d\n", current->port2);
        printf("String Data:\n");
        printStringList(current->string_list);
        printf("\n");
        current = current->next;
    }
}

// void showquery(struct StringNode* head, char response[]){
//     struct StringNode* current1 = head;
//     char* tmp1 = NULL;
//     while(current1!=NULL){
//         if(strcmp(current1->data, "") == 0 && current1 -> next == NULL){
//             break;
//         }
//         if(strcmp(current1->data, "") == 0 && current1 -> next != NULL){
//             current1 = current1 -> next;
//         }
//         sprintf(tmp1, "Query: %s\n", current1 ->data);
//         strcat(response, tmp1);
//         current1 = current1 -> next;
//     }
// }

// void showinfo(struct Node* head, char response[]) {
//     struct Node* current = head;
//     while (current != NULL) {
//         // printf("Rule: ");
//         strcpy(response, "Rule: ");
//         char* tmp = NULL;
//         if(current -> ip2[0] == -1 && current -> port2 == -1){
//             sprintf(tmp,"%d.%d.%d.%d %d\n", current ->ip1[0],current
//             ->ip1[1],current ->ip1[2],current ->ip1[3],current ->port1);
//             strcat(response,tmp);
//         } else if(current -> ip2[0] == -1 && current -> port2 != -1){
//             sprintf(tmp,"%d.%d.%d.%d %d-%d\n", current ->ip1[0],current
//             ->ip1[1],current ->ip1[2],current ->ip1[3],current ->port1,
//             current->port2); strcat(response,tmp);
//         } else if(current -> ip2[0] != -1 && current -> port2 == -1){
//             sprintf(tmp,"%d.%d.%d.%d-%d.%d.%d.%d %d\n", current
//             ->ip1[0],current ->ip1[1],current ->ip1[2],current
//             ->ip1[3],current ->ip2[0],current ->ip2[1],current
//             ->ip2[2],current ->ip2[3],current ->port1); strcat(response,tmp);
//         } else if(current -> ip2[0] != -1 && current -> port2 != -1){
//             sprintf(tmp,"%d.%d.%d.%d-%d.%d.%d.%d %d-%d\n", current
//             ->ip1[0],current ->ip1[1],current ->ip1[2],current
//             ->ip1[3],current ->ip2[0],current ->ip2[1],current
//             ->ip2[2],current ->ip2[3],current ->port1, current->port2);
//             strcat(response,tmp);
//         }
//         showquery(current->string_list, response);
//         current = current->next;
//     }
// }


void freeStringList(struct StringNode *head) {
    struct StringNode *current = head;
    while (current != NULL) {
        struct StringNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}


void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *next = current->next;
        freeStringList(current->string_list);
        free(current);
        current = next;
    }
}

int deleteNode(struct Node **head, int ip1[4], int ip2[4], int port1,
               int port2) {
    // int flag = -1;
    struct Node *current = *head;
    struct Node *previous = NULL;

    while (current != NULL) {
        if (memcmp(current->ip1, ip1, sizeof(int) * 4) == 0 &&
            memcmp(current->ip2, ip2, sizeof(int) * 4) == 0 &&
            current->port1 == port1 && current->port2 == port2) {
            // flag = 1;
            // Node matches the criteria, remove it from the list
            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            freeStringList(current->string_list);
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return -1;
}

int countCharInString(const char *str, char targetChar) {
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == targetChar) {
            count++;
        }
    }
    return count;
}

int isValid(char *string) {
    int ip1[4] = {-1, -1, -1, -1};
    int ip2[4] = {-1, -1, -1, -1};
    int port1 = -1;
    int port2 = -1;
    int flag = -1;
    // int a1,b1,c1,d1,a2,b2,c2,d2,p1,p2;
    // a1=-1;b1=-1;c1=-1;d1=-1;a2=-1;b2=-1;c2=-1;d2=-1;p1=-1;p2=-1;
    // printf("----%d-----%d-----\n",countCharInString(string, '.'),
    // countCharInString(string, '-') ); printf("--------------------------\n");
    if (countCharInString(string, '.') == 3 &&
        countCharInString(string, '-') == 0) {
        sscanf(string, "%d.%d.%d.%d %d", &ip1[0], &ip1[1], &ip1[2], &ip1[3],
               &port1);
        flag = 1;
        for (int i = 0; i < 4; i++) {
            if (ip1[i] < 0 || ip1[i] > 255) {
                flag = -1;
            }
        }
        if (port1 < 0 || port1 > 65535) {
            flag = -1;
        }
    } else if (countCharInString(string, '.') == 3 &&
               countCharInString(string, '-') == 1) {
        sscanf(string, "%d.%d.%d.%d %d-%d", &ip1[0], &ip1[1], &ip1[2], &ip1[3],
               &port1, &port2);
        flag = 2;
        for (int i = 0; i < 4; i++) {
            if (port1 >= port2) {
                flag = -1;
            }
            if (ip1[i] < 0 || ip1[i] > 255) {
                flag = -1;
            }
        }
        if (port1 < 0 || port1 > 65535) {
            flag = -1;
        }
        if (port2 < 0 || port2 > 65535) {
            flag = -1;
        }
    } else if (countCharInString(string, '.') == 6 &&
               countCharInString(string, '-') == 1) {
        sscanf(string, "%d.%d.%d.%d-%d.%d.%d.%d %d", &ip1[0], &ip1[1], &ip1[2],
               &ip1[3], &ip2[0], &ip2[1], &ip2[2], &ip2[3], &port1);
        flag = 3;
        for (int i = 0; i < 4; i++) {
            long long i1a, i1b, i1c, i1d, i2a, i2b, i2c, i2d, suml, sumr;
            i1a = (long long)ip1[0];
            i1b = (long long)ip1[1];
            i1c = (long long)ip1[2];
            i1d = (long long)ip1[3];

            i2a = (long long)ip2[0];
            i2b = (long long)ip2[1];
            i2c = (long long)ip2[2];
            i2d = (long long)ip2[3];

            suml = i1a * 1000000000 + i1b * 1000000 + i1c * 1000 + i1d;
            sumr = i2a * 1000000000 + i2b * 1000000 + i2c * 1000 + i2d;
            if (suml >= sumr) {
                flag = -1;
            }
            if (ip1[i] < 0 || ip1[i] > 255) {
                flag = -1;
            }
            if (ip2[i] < 0 || ip2[i] > 255) {
                flag = -1;
            }
        }
        if (port1 < 0 || port1 > 65535) {
            flag = -1;
        }
    } else if (countCharInString(string, '.') == 6 &&
               countCharInString(string, '-') == 2) {
        sscanf(string, "%d.%d.%d.%d-%d.%d.%d.%d %d-%d", &ip1[0], &ip1[1],
               &ip1[2], &ip1[3], &ip2[0], &ip2[1], &ip2[2], &ip2[3], &port1,
               &port2);
        flag = 4;
        long long i1a, i1b, i1c, i1d, i2a, i2b, i2c, i2d;
        i1a = (long long)ip1[0];
        i1b = (long long)ip1[1];
        i1c = (long long)ip1[2];
        i1d = (long long)ip1[3];

        i2a = (long long)ip2[0];
        i2b = (long long)ip2[1];
        i2c = (long long)ip2[2];
        i2d = (long long)ip2[3];

        long long suml = i1a * 1000000000 + i1b * 1000000 + i1c * 1000 + i1d;
        long long sumr = i2a * 1000000000 + i2b * 1000000 + i2c * 1000 + i2d;
        if (suml >= sumr || port1 >= port2) {
            flag = -1;
        }
        for (int i = 0; i < 4; i++) {
            if (ip1[i] < 0 || ip1[i] > 255) {
                flag = -1;
            }
            if (ip2[i] < 0 || ip2[i] > 255) {
                flag = -1;
            }
        }
        if (port1 < 0 || port1 > 65535) {
            flag = -1;
        }
        if (port2 < 0 || port2 > 65535) {
            flag = -1;
        }
    } else {
        flag = -1;
    }
    // for(int i = 0; i < 4; i++){
    //     printf("%d\n", ip1[i]);
    // }
    // for(int i = 0; i < 4; i++){
    //     printf("%d\n", ip2[i]);
    // }
    // printf("%d\n",port1);
    // printf("%d\n", port2);

    return flag;
}

/* displays error messages from system calls */
void error(char *msg) {
    perror(msg);
    exit(1);
};

struct threadArgs_t {
    int newsockfd;
    int threadIndex;
};

int isExecuted = 0;
int returnValue = 0; /* not used; need something to keep compiler happy */
pthread_mutex_t mut =
    PTHREAD_MUTEX_INITIALIZER; /* the lock used for processing */

/* this is only necessary for proper termination of threads - you should not
 * need to access this part in your code */
struct threadInfo_t {
    pthread_t pthreadInfo;
    pthread_attr_t attributes;
    int status;
};
struct threadInfo_t *serverThreads = NULL;
int noOfThreads = 0;
pthread_rwlock_t threadLock = PTHREAD_RWLOCK_INITIALIZER;
pthread_cond_t threadCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t threadEndLock = PTHREAD_MUTEX_INITIALIZER;
int traverseQuery(struct Node *head, char *query);
//----------------------------------------------------------------------------------------------------------------------
/* For each connection, this function is called in a separate thread. You need
 * to modify this function. */
void *processRequest(void *args) {
    char response[9999];
    struct threadArgs_t *threadArgs; 
    char buffer[BUFFERLENGTH];       
    int n;
    int tmp; 
    threadArgs = (struct threadArgs_t *)args;
    bzero(buffer, BUFFERLENGTH); 
    n = read(threadArgs->newsockfd, buffer,
             BUFFERLENGTH - 1);
    if (n < 0)
        error("ERROR reading from socket");


    //   printf("Here is the message: %s\n", buffer);
    int count = 0;
    char delimiter = '_';
    char **command = splitString(buffer, delimiter, &count);
    // printf("-------------------------------------------------\n");
    // for (int i = 0; i < count; i++)
    // {
    //     printf("Part %d: %s\n", i + 1, command[i]);
    // }
    // printf("%s--==--=-=-=-=-=-=-=-=-=-==--=-=-=-=-=", command[0]);
    if (strcmp(command[0], "A") == 0) {
        int type = isValid(command[1]);
        // printf("%d----====\n", type);
        if (type == 1) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            char *data = "";
            sscanf(command[1], "%d.%d.%d.%d %d", &ip1[0], &ip1[1], &ip1[2],
                   &ip1[3], &port1);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            insertNode(&head, ip1, ip2, port1, port2, data);
            pthread_mutex_unlock(&nodeMutex);
            // response = "Rule added\n";
            strcpy(response, "Rule added\n");
        } else if (type == 2) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            char *data = "";
            sscanf(command[1], "%d.%d.%d.%d %d-%d", &ip1[0], &ip1[1], &ip1[2],
                   &ip1[3], &port1, &port2);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            insertNode(&head, ip1, ip2, port1, port2, data);
            pthread_mutex_unlock(&nodeMutex);
            strcpy(response, "Rule added\n");
        } else if (type == 3) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            char *data = "";
            // printf("%s\n", command[1]);
            sscanf(command[1], "%d.%d.%d.%d-%d.%d.%d.%d %d", &ip1[0], &ip1[1],
                   &ip1[2], &ip1[3], &ip2[0], &ip2[1], &ip2[2], &ip2[3],
                   &port1);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            insertNode(&head, ip1, ip2, port1, port2, data);
            pthread_mutex_unlock(&nodeMutex);
            strcpy(response, "Rule added\n");
        } else if (type == 4) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            char *data = "";
            sscanf(command[1], "%d.%d.%d.%d-%d.%d.%d.%d %d-%d", &ip1[0],
                   &ip1[1], &ip1[2], &ip1[3], &ip2[0], &ip2[1], &ip2[2],
                   &ip2[3], &port1, &port2);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            insertNode(&head, ip1, ip2, port1, port2, data);
            pthread_mutex_unlock(&nodeMutex);
            strcpy(response, "Rule added\n");
        } else if (type == -1) {
            strcpy(response, "Invalid rule\n");
        }
        // printf("--------------------\n");
        // printList(head);
        // printf("--------------------\n");
    } else if (strcmp(command[0], "C") == 0) {
        char *query = command[1];
        int type = isValid(query);
        if (type == 1) {
            int res = traverseQuery(head, query);
            if (res == 1) {
                // response = "Connection accepted\n";
                strcpy(response, "Connection accepted\n");
            } else if (res == -1) {
                // response = "Connection rejected\n";
                strcpy(response, "Connection rejected\n");
            }
        } else {
            // response = "Illegal IP address or port specified";
            strcpy(response, "Illegal IP address or port specified\n");
        }
    } else if (strcmp(command[0], "D") == 0) {
        int type = isValid(command[1]);
        // printf("%d----====\n", type);
        if (type == 1) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            sscanf(command[1], "%d.%d.%d.%d %d", &ip1[0], &ip1[1], &ip1[2],
                   &ip1[3], &port1);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            int res = deleteNode(&head, ip1, ip2, port1, port2);
            if (res == 1) {
                // response = "Rule deleted\n";
                strcpy(response, "Rule deleted\n");
            } else {
                // response = "Rule invalid\n";
                strcpy(response, "Rule invalid\n");
            }
            pthread_mutex_unlock(&nodeMutex);
        } else if (type == 2) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            sscanf(command[1], "%d.%d.%d.%d %d-%d", &ip1[0], &ip1[1], &ip1[2],
                   &ip1[3], &port1, &port2);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            int res = deleteNode(&head, ip1, ip2, port1, port2);
            if (res == 1) {
                // response = "Rule deleted\n";
                strcpy(response, "Rule deleted\n");
            } else {
                // response = "Rule invalid\n";
                strcpy(response, "Rule invalid\n");
            }
            pthread_mutex_unlock(&nodeMutex);
        } else if (type == 3) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            // printf("%s\n", command[1]);
            sscanf(command[1], "%d.%d.%d.%d-%d.%d.%d.%d %d", &ip1[0], &ip1[1],
                   &ip1[2], &ip1[3], &ip2[0], &ip2[1], &ip2[2], &ip2[3],
                   &port1);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            int res = deleteNode(&head, ip1, ip2, port1, port2);
            if (res == 1) {
                strcpy(response, "Rule deleted\n");
            } else {
                strcpy(response, "Rule invalid\n");
            }
            pthread_mutex_unlock(&nodeMutex);
        } else if (type == 4) {
            int ip1[4] = {-1, -1, -1, -1};
            int ip2[4] = {-1, -1, -1, -1};
            int port1 = -1;
            int port2 = -1;
            sscanf(command[1], "%d.%d.%d.%d-%d.%d.%d.%d %d-%d", &ip1[0],
                   &ip1[1], &ip1[2], &ip1[3], &ip2[0], &ip2[1], &ip2[2],
                   &ip2[3], &port1, &port2);
            // printf("%d.%d.%d.%d %d", ip1[0], ip1[1], ip1[2],ip1[3], port1);
            pthread_mutex_lock(&nodeMutex);
            int res = deleteNode(&head, ip1, ip2, port1, port2);
            if (res == 1) {
                strcpy(response, "Rule deleted\n");
            } else {
                strcpy(response, "Rule invalid\n");
            }
            pthread_mutex_unlock(&nodeMutex);
        } else if (type == -1) {
            // response = "Rule not found\n";
            strcpy(response, "Rule not found\n");
        }
    } else if (strcmp(command[0], "L") == 0) {
        pthread_mutex_lock(&nodeMutex);
        // showinfo(head, response);
        struct Node *current = head;
        strcpy(response, "");
        while (current != NULL) {
            // printf("Rule: ");
            strcat(response, "Rule: ");
            char tmp[99];
            if (current->ip2[0] == -1 && current->port2 == -1) {
                sprintf(tmp, "%d.%d.%d.%d %d\n", current->ip1[0],
                        current->ip1[1], current->ip1[2], current->ip1[3],
                        current->port1);
                strcat(response, tmp);
            } else if (current->ip2[0] == -1 && current->port2 != -1) {
                sprintf(tmp, "%d.%d.%d.%d %d-%d\n", current->ip1[0],
                        current->ip1[1], current->ip1[2], current->ip1[3],
                        current->port1, current->port2);
                strcat(response, tmp);
            } else if (current->ip2[0] != -1 && current->port2 == -1) {
                sprintf(tmp, "%d.%d.%d.%d-%d.%d.%d.%d %d\n", current->ip1[0],
                        current->ip1[1], current->ip1[2], current->ip1[3],
                        current->ip2[0], current->ip2[1], current->ip2[2],
                        current->ip2[3], current->port1);
                strcat(response, tmp);
            } else if (current->ip2[0] != -1 && current->port2 != -1) {
                sprintf(tmp, "%d.%d.%d.%d-%d.%d.%d.%d %d-%d\n", current->ip1[0],
                        current->ip1[1], current->ip1[2], current->ip1[3],
                        current->ip2[0], current->ip2[1], current->ip2[2],
                        current->ip2[3], current->port1, current->port2);
                strcat(response, tmp);
            }
            // showquery(current->string_list, response);
            struct StringNode *current1 = current->string_list;
            char tmp1[99];
            while (current1 != NULL) {
                if (strcmp(current1->data, "") == 0 && current1->next == NULL) {
                    break;
                }
                if (strcmp(current1->data, "") == 0 && current1->next != NULL) {
                    current1 = current1->next;
                }
                sprintf(tmp1, "Query: %s\n", current1->data);
                strcat(response, tmp1);
                current1 = current1->next;
            }
            current = current->next;
        }
        pthread_mutex_unlock(&nodeMutex);
    } else {
        // printf("error type! \n");
        strcpy(response, "Illegal request\n");
    }
    // 记得释放command内存！
    for (int i = 0; i < count; i++) {
        free(command[i]);
    }
    free(command);

    // printList(head);

    pthread_mutex_lock(&mut); /* lock exclusive access to variable isExecuted */
    tmp = isExecuted;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // printf("Waiting for confirmation: Please input an integer\n");
    // scanf("%d", &n); /* not to be done in real programs: don't go to sleep
    // while holding a lock! Done here to demonstrate the mutual exclusion
    // problem. */ printf("Read value %d\n", n);

    isExecuted = tmp + 1;
    pthread_mutex_unlock(&mut); /* release the lock */
    // int x = 100;
    // n = sprintf(buffer, "I got you message, the  value of isExecuted is
    // %d\n", x);
    strcpy(buffer, response);
    /* send the reply back */
    n = write(threadArgs->newsockfd, buffer, BUFFERLENGTH);
    if (n < 0)
        error("ERROR writing to socket");

    /* these two lines are required for proper thread termination */
    serverThreads[threadArgs->threadIndex].status = THREAD_FINISHED;
    pthread_cond_signal(&threadCond);

    close(threadArgs->newsockfd); /* important to avoid memory leak */
    free(threadArgs);
    pthread_exit(&returnValue);
}
//----------------------------------------------------------------------------------------------------------------------
/* finds unused thread info slot; allocates more slots if necessary
   only called by main thread */
int findThreadIndex() {
    int i, tmp;

    for (i = 0; i < noOfThreads; i++) {
        if (serverThreads[i].status == THREAD_AVAILABLE) {
            serverThreads[i].status = THREAD_IN_USE;
            return i;
        }
    }

    /* no available thread found; need to allocate more threads */
    pthread_rwlock_wrlock(&threadLock);
    serverThreads = realloc(serverThreads, ((noOfThreads + THREADS_ALLOCATED) *
                                            sizeof(struct threadInfo_t)));
    noOfThreads = noOfThreads + THREADS_ALLOCATED;
    pthread_rwlock_unlock(&threadLock);
    if (serverThreads == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    /* initialise thread status */
    for (tmp = i + 1; tmp < noOfThreads; tmp++) {
        serverThreads[tmp].status = THREAD_AVAILABLE;
    }
    serverThreads[i].status = THREAD_IN_USE;
    return i;
}

/* waits for threads to finish and releases resources used by the thread
 * management functions. You don't need to modify this function */
void *waitForThreads(void *args) {
    int i, res;
    while (1) {
        pthread_mutex_lock(&threadEndLock);
        pthread_cond_wait(&threadCond, &threadEndLock);
        pthread_mutex_unlock(&threadEndLock);

        pthread_rwlock_rdlock(&threadLock);
        for (i = 0; i < noOfThreads; i++) {
            if (serverThreads[i].status == THREAD_FINISHED) {
                res = pthread_join(serverThreads[i].pthreadInfo, NULL);
                if (res != 0) {
                    fprintf(stderr, "thread joining failed, exiting\n");
                    exit(1);
                }
                serverThreads[i].status = THREAD_AVAILABLE;
            }
        }
        pthread_rwlock_unlock(&threadLock);
    }
}

int checkStringIn(struct StringNode *head, char *ipip) {
    struct StringNode *current = head;
    while (current != NULL) {
        if (strcmp(current->data, ipip) == 0) {
            return 1; // 在
        }
        current = current->next;
    }
    return 0; // 不在
}

int traverseQuery(struct Node *head, char *query) {
    int flag = -1;
    struct Node *current = head;
    long long querya, queryb, queryc, queryd, queryport, querysum, suml, sumr,
        ip1a, ip1b, ip1c, ip1d, port1, ip2a, ip2b, ip2c, ip2d, port2;
    if (isValid(query) == 1) {
        sscanf(query, "%lld.%lld.%lld.%lld %lld", &querya, &queryb, &queryc,
               &queryd, &queryport);
        querysum =
            querya * 1000000000 + queryb * 1000000 + queryc * 1000 + queryd;
    } else if (isValid(query) == -1) {
        printf("query packet wrong.\n");
    }
    while (current != NULL) {
        if (current->ip2[0] == -1 && current->port2 == -1) {
            ip1a = (long long)current->ip1[0];
            ip1b = (long long)current->ip1[1];
            ip1c = (long long)current->ip1[2];
            ip1d = (long long)current->ip1[3];
            port1 = (long long)current->port1;
            suml = ip1a * 1000000000 + ip1b * 1000000 + ip1c * 1000 + ip1d;
            if (querysum == suml && queryport == port1) {
                flag = 1;
                if (checkStringIn(current->string_list, query) == 0) {
                    insertStringNode(&(current->string_list), query);
                }
            }
        } else if (current->ip2[0] == -1 && current->port2 != -1) {
            ip1a = (long long)current->ip1[0];
            ip1b = (long long)current->ip1[1];
            ip1c = (long long)current->ip1[2];
            ip1d = (long long)current->ip1[3];
            port1 = (long long)current->port1;
            port2 = (long long)current->port2;
            suml = ip1a * 1000000000 + ip1b * 1000000 + ip1c * 1000 + ip1d;
            if (querysum == suml && queryport >= port1 && queryport <= port2) {
                flag = 1;
                if (checkStringIn(current->string_list, query) == 0) {
                    insertStringNode(&(current->string_list), query);
                }
            }
        } else if (current->ip2[0] != -1 && current->port2 == -1) {
            ip1a = (long long)current->ip1[0];
            ip1b = (long long)current->ip1[1];
            ip1c = (long long)current->ip1[2];
            ip1d = (long long)current->ip1[3];
            ip2a = (long long)current->ip2[0];
            ip2b = (long long)current->ip2[1];
            ip2c = (long long)current->ip2[2];
            ip2d = (long long)current->ip2[3];
            port1 = (long long)current->port1;
            suml = ip1a * 1000000000 + ip1b * 1000000 + ip1c * 1000 + ip1d;
            sumr = ip2a * 1000000000 + ip2b * 1000000 + ip2c * 1000 + ip2d;
            if (querysum >= suml && querysum <= sumr && queryport == port1) {
                flag = 1;
                if (checkStringIn(current->string_list, query) == 0) {
                    insertStringNode(&(current->string_list), query);
                }
            }
        } else if (current->ip2[0] != -1 && current->port2 != -1) {
            ip1a = (long long)current->ip1[0];
            ip1b = (long long)current->ip1[1];
            ip1c = (long long)current->ip1[2];
            ip1d = (long long)current->ip1[3];
            ip2a = (long long)current->ip2[0];
            ip2b = (long long)current->ip2[1];
            ip2c = (long long)current->ip2[2];
            ip2d = (long long)current->ip2[3];
            port1 = (long long)current->port1;
            port2 = (long long)current->port2;
            suml = ip1a * 1000000000 + ip1b * 1000000 + ip1c * 1000 + ip1d;
            sumr = ip2a * 1000000000 + ip2b * 1000000 + ip2c * 1000 + ip2d;
            if (querysum >= suml && querysum <= sumr && queryport >= port1 &&
                queryport <= port2) {
                flag = 1;
                if (checkStringIn(current->string_list, query) == 0) {
                    insertStringNode(&(current->string_list), query);
                }
            }
        }
        current = current->next;
    }
    return flag;
}

int main(int argc, char *argv[]) {

    socklen_t clilen;
    int sockfd, portno;
    struct sockaddr_in6 serv_addr, cli_addr;
    int result;
    pthread_t waitInfo;
    pthread_attr_t waitAttributes;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    /* create socket */
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(portno);

    /* bind it */
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    /* ready to accept connections */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* create separate thread for waiting  for other threads to finish */
    if (pthread_attr_init(&waitAttributes)) {
        fprintf(stderr, "Creating initial thread attributes failed!\n");
        exit(1);
    }

    result = pthread_create(&waitInfo, &waitAttributes, waitForThreads, NULL);
    if (result != 0) {
        fprintf(stderr, "Initial Thread creation failed!\n");
        exit(1);
    }

    /* now wait in an endless loop for connections and process them */
    while (1) {

        struct threadArgs_t
            *threadArgs; /* must be allocated on the heap to prevent variable
                            going out of scope */
        int threadIndex;

        threadArgs = malloc(sizeof(struct threadArgs_t));
        if (!threadArgs) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }

        /* waiting for connections */
        threadArgs->newsockfd =
            accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (threadArgs->newsockfd < 0)
            error("ERROR on accept");

        /* create thread for processing of connection */
        threadIndex = findThreadIndex();
        threadArgs->threadIndex = threadIndex;
        if (pthread_attr_init(&(serverThreads[threadIndex].attributes))) {
            fprintf(stderr, "Creating thread attributes failed!\n");
            exit(1);
        }

        result = pthread_create(&(serverThreads[threadIndex].pthreadInfo),
                                &(serverThreads[threadIndex].attributes),
                                processRequest, (void *)threadArgs);
        if (result != 0) {
            fprintf(stderr, "Thread creation failed!\n");
            exit(1);
        }
    }
    freeList(head);
}
