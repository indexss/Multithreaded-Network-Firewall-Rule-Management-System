#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct URL
{
    int isIPRange;
    int isPortRange;
    int a1, b1, c1, d1, port1, a2, b2, c2, d2, port2;
};

typedef struct URL URL;

int split(char *str, char *delim, char *result[], int maxTokens)
{
    int tokenCount = 0;
    char *token = strtok(str, delim);

    while (token != NULL && tokenCount < maxTokens)
    {
        result[tokenCount] = token;
        token = strtok(NULL, delim);
        tokenCount++;
    }

    return tokenCount;
}

// 1: url1 < url2 ; 0: url1 > url2 ; 3: url1 == url2
int smallerThan(URL url1, URL url2)
{
    if (url1.a1 < url2.a1)
    {
        return 1;
    }
    else if (url1.a1 > url2.a1)
    {
        return 0;
    }
    else if (url1.a1 == url2.a1)
    {
        if (url1.b1 < url2.b1)
        {
            return 1;
        }
        else if (url1.b1 > url2.b1)
        {
            return 0;
        }
        else if (url1.b1 == url2.b1)
        {
            if (url1.c1 < url2.c1)
            {
                return 1;
            }
            else if (url1.c1 > url2.c1)
            {
                return 0;
            }
            else if (url1.c1 == url2.c1)
            {
                if (url1.d1 > url2.d1)
                {
                    return 1;
                }
                else if (url1.d1 > url2.d1)
                {
                    return 0;
                }
                else if (url1.d1 == url2.d1)
                {
                    if (url1.port1 < url2.port1)
                    {
                        return 1;
                    }
                    else if (url1.port1 > url2.port1)
                    {
                        return 0;
                    }
                    else if (url1.port1 == url2.port1)
                    {
                        return 3;
                    }
                }
            }
        }
    }
    return 3;
}

int main(int argc, char **argv)
{
    // read the filename
    char *fileName = *(argv + 1);
    URL urls[100];

    // open and read the file
    FILE *fp = fopen(fileName, "r");
    // FILE *fp = fopen("ban.txt", "r");
    if (fp == NULL)
    {
        printf("wrong file name.\n");
        return -1;
    }

    char *line = NULL;
    char *ip = NULL;
    size_t len = 0;
    // ssize_t read;
    int count = 0;

    // read file and store data into structure
    while ((getline(&ip, &len, fp)) != -1)
    {
        int dotCount = 0;
        int dashCount = 0;
        for (int i = 0; i < strlen(ip); i++)
        {
            if (ip[i] == '.')
            {
                dotCount++;
            }
            else if (ip[i] == '-')
            {
                dashCount++;
            }
        }
        if (dotCount == 3 && dashCount == 0)
        {
            // printf("3 0\n");
            urls[count].isIPRange = 0;
            urls[count].isPortRange = 0;
            char delim[] = ".- ";
            int maxTokens = 10;
            char *result[maxTokens];
            split(ip, delim, result, maxTokens);
            urls[count].a1 = atoi(result[0]);
            urls[count].b1 = atoi(result[1]);
            urls[count].c1 = atoi(result[2]);
            urls[count].d1 = atoi(result[3]);
            urls[count].port1 = atoi(result[4]);
            if ((urls[count].a1 < 0 || urls[count].a1 > 255) || (urls[count].b1 < 0 || urls[count].b1 > 255) || (urls[count].c1 < 0 || urls[count].c1 > 255) || (urls[count].d1 < 0 || urls[count].d1 > 255) || (urls[count].port1 < 0 || urls[count].port1 > 65535))
            {
                printf("Ill formed rule: %d.%d.%d.%d %d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1, urls[count].port1);
                continue;
            }
            // printf("%d %d %d %d %d\n", urls[count].a1,urls[count].b1,urls[count].c1,urls[count].d1,urls[count].port1);
            count++;
            continue;
        }
        else if (dotCount == 6 && dashCount == 1)
        {
            // printf("6 1\n");
            urls[count].isIPRange = 1;
            urls[count].isPortRange = 0;
            char delim[] = ".- ";
            int maxTokens = 10;
            char *result[maxTokens];
            split(ip, delim, result, maxTokens);
            urls[count].a1 = atoi(result[0]);
            urls[count].b1 = atoi(result[1]);
            urls[count].c1 = atoi(result[2]);
            urls[count].d1 = atoi(result[3]);
            urls[count].a2 = atoi(result[4]);
            urls[count].b2 = atoi(result[5]);
            urls[count].c2 = atoi(result[6]);
            urls[count].d2 = atoi(result[7]);
            urls[count].port1 = atoi(result[8]);
            if ((urls[count].a1 < 0 || urls[count].a1 > 255) || (urls[count].b1 < 0 || urls[count].b1 > 255) || (urls[count].c1 < 0 || urls[count].c1 > 255) || (urls[count].d1 < 0 || urls[count].d1 > 255) || (urls[count].port1 < 0 || urls[count].port1 > 65535) || (urls[count].a2 < 0 || urls[count].a2 > 255) || (urls[count].b2 < 0 || urls[count].b2 > 255) || (urls[count].c2 < 0 || urls[count].c2 > 255) || (urls[count].d2 < 0 || urls[count].d2 > 255))
            {
                printf("Ill formed rule: %d.%d.%d.%d-%d.%d.%d.%d %d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1, urls[count].a2, urls[count].b2, urls[count].c2, urls[count].d2, urls[count].port1);
                continue;
            }
            count++;
            continue;
        }
        else if (dotCount == 3 && dashCount == 1)
        {
            // printf("3 1\n");
            urls[count].isIPRange = 0;
            urls[count].isPortRange = 1;
            char delim[] = ".- ";
            int maxTokens = 10;
            char *result[maxTokens];
            split(ip, delim, result, maxTokens);
            urls[count].a1 = atoi(result[0]);
            urls[count].b1 = atoi(result[1]);
            urls[count].c1 = atoi(result[2]);
            urls[count].d1 = atoi(result[3]);
            urls[count].port1 = atoi(result[4]);
            urls[count].port2 = atoi(result[5]);
            if ((urls[count].a1 < 0 || urls[count].a1 > 255) || (urls[count].b1 < 0 || urls[count].b1 > 255) || (urls[count].c1 < 0 || urls[count].c1 > 255) || (urls[count].d1 < 0 || urls[count].d1 > 255) || (urls[count].port1 < 0 || urls[count].port1 > 65535) || (urls[count].port2 < 0 || urls[count].port2 > 65535))
            {
                printf("Ill formed rule: %d.%d.%d.%d %d-%d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1, urls[count].port1, urls[count].port2);
                continue;
            }
            count++;
            continue;
        }
        else if (dotCount == 6 && dashCount == 2)
        {
            // printf("6 2\n");
            urls[count].isIPRange = 1;
            urls[count].isPortRange = 1;
            char delim[] = ".- ";
            int maxTokens = 10;
            char *result[maxTokens];
            split(ip, delim, result, maxTokens);
            urls[count].a1 = atoi(result[0]);
            urls[count].b1 = atoi(result[1]);
            urls[count].c1 = atoi(result[2]);
            urls[count].d1 = atoi(result[3]);
            urls[count].a2 = atoi(result[4]);
            urls[count].b2 = atoi(result[5]);
            urls[count].c2 = atoi(result[6]);
            urls[count].d2 = atoi(result[7]);
            urls[count].port1 = atoi(result[8]);
            urls[count].port2 = atoi(result[9]);
            if ((urls[count].a1 < 0 || urls[count].a1 > 255) || (urls[count].b1 < 0 || urls[count].b1 > 255) || (urls[count].c1 < 0 || urls[count].c1 > 255) || (urls[count].d1 < 0 || urls[count].d1 > 255) || (urls[count].port1 < 0 || urls[count].port1 > 65535) || (urls[count].a2 < 0 || urls[count].a2 > 255) || (urls[count].b2 < 0 || urls[count].b2 > 255) || (urls[count].c2 < 0 || urls[count].c2 > 255) || (urls[count].d2 < 0 || urls[count].d2 > 255) || (urls[count].port2 < 0 || urls[count].port2 > 65535))
            {
                printf("Ill formed rule: %d.%d.%d.%d-%d.%d.%d.%d %d-%d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1, urls[count].a2, urls[count].b2, urls[count].c2, urls[count].d2, urls[count].port1, urls[count].port2);
                continue;
            }
            count++;
            continue;
        }
        else
        {
            printf("Ill formed rule: %s", ip);
            continue;
        }
    }

    // close the file
    fclose(fp);
    if (line)
    {
        free(line);
    }

    // bubble sort
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (smallerThan(urls[j], urls[j + 1]) != 0)
            {
                URL temp;
                temp = urls[j];
                urls[j] = urls[j + 1];
                urls[j + 1] = temp;
            }
        }
    }

    // output
    for (int i = 0; i < count; i++)
    {
        if (urls[i].isIPRange == 0 && urls[i].isPortRange == 0)
        {
            printf("Rule: %d.%d.%d.%d %d\n", urls[i].a1, urls[i].b1, urls[i].c1, urls[i].d1, urls[i].port1);
        }
        else if (urls[i].isIPRange == 1 && urls[i].isPortRange == 0)
        {
            printf("Rule: %d.%d.%d.%d-%d.%d.%d.%d %d\n", urls[i].a1, urls[i].b1, urls[i].c1, urls[i].d1, urls[i].a2, urls[i].b2, urls[i].c2, urls[i].d2, urls[i].port1);
        }
        else if (urls[i].isIPRange == 0 && urls[i].isPortRange == 1)
        {
            printf("Rule: %d.%d.%d.%d %d-%d\n", urls[i].a1, urls[i].b1, urls[i].c1, urls[i].d1, urls[i].port1, urls[i].port2);
        }
        else if (urls[i].isIPRange == 1 && urls[i].isPortRange == 1)
        {
            printf("Rule: %d.%d.%d.%d-%d.%d.%d.%d %d-%d\n", urls[i].a1, urls[i].b1, urls[i].c1, urls[i].d1, urls[i].a2, urls[i].b2, urls[i].c2, urls[i].d2, urls[i].port1, urls[i].port2);
        }
    }

    return 0;
}