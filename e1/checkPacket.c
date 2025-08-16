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

void isBanned(char *sip, char **sport, URL urls[], int count)
{
    // printf("%s\n", sip);
    int a, b, c, d;
    int flag = 1;
    int port = atoi(*sport);
    int pt1 = 0;
    int pt2 = 0;
    char aa[4];
    char bb[4];
    char cc[4];
    char dd[4];

    while (sip[pt2] != '.')
    {
        pt2++;
    }
    strncpy(aa, sip, (pt2 - pt1));
    int cha1 = pt2 - pt1 + 1;
    pt1 = pt2;
    pt1++;
    pt2++;

    while (sip[pt2] != '.')
    {
        pt2++;
    }
    strncpy(bb, sip + cha1, (pt2 - pt1));
    int cha2 = pt2 - pt1 + 1;
    pt1 = pt2;
    pt1++;
    pt2++;

    while (sip[pt2] != '.')
    {
        pt2++;
    }
    strncpy(cc, sip + cha2 + cha1, (pt2 - pt1));
    int cha3 = pt2 - pt1 + 1;
    pt1 = pt2;
    pt1++;
    pt2++;

    while (sip[pt2] != '\0')
    {
        pt2++;
    }
    strncpy(dd, sip + cha3 + cha2 + cha1, (pt2 - pt1));

    a = atoi(aa);
    b = atoi(bb);
    c = atoi(cc);
    d = atoi(dd);
    // char delim[] = ".- ";
    // int maxTokens = 10;
    // char *result[maxTokens];
    // split(*sip, delim, result, maxTokens);
    // a = atoi(result[0]);
    // b = atoi(result[1]);
    // c = atoi(result[2]);
    // d = atoi(result[3]);
    // a = 92;
    // b = 18;
    // c = 0;
    // d = 14;
    // port = 203;
    // printf("%d %d %d %d %d",a,b,c,d,port);
    for (int i = 0; i < count; i++)
    {
        if (urls[i].isIPRange == 0 && urls[i].isPortRange == 0)
        {
            if ((urls[i].a1 == a) && (urls[i].b1 == b) && (urls[i].c1 == c) && (urls[i].d1 == d) && (urls[i].port1 == port))
            {
                flag = 0;
                break;
            }
            else
            {
                flag = 1;
            }
        }
        else if (urls[i].isIPRange == 1 && urls[i].isPortRange == 0)
        {
            long long ipboard1 = ((long long)(urls[i].a1) * 1000000000) + (urls[i].b1 * 1000000) + (urls[i].c1 * 1000) + (urls[i].d1);
            long long ipboard2 = ((long long)(urls[i].a2) * 1000000000) + (urls[i].b2 * 1000000) + (urls[i].c2 * 1000) + (urls[i].d2);
            long long currentIp = (long long)(a) * 1000000000 + b * 1000000 + c * 1000 + d;
            if (currentIp >= ipboard1 && currentIp <= ipboard2)
            {
                if (port == urls[i].port1)
                {
                    flag = 0;
                    break;
                }
            }
            else
            {
                flag = 1;
            }
            continue;
        }
        else if (urls[i].isIPRange == 0 && urls[i].isPortRange == 1)
        {
            if ((urls[i].a1 == a) && (urls[i].b1 == b) && (urls[i].c1 == c) && (urls[i].d1 == d))
            {
                if (port >= urls[i].port1 && port <= urls[i].port2)
                {
                    flag = 0;
                    break;
                }
            }
            else
            {
                flag = 1;
            }
            continue;
        }
        else if (urls[i].isIPRange == 1 && urls[i].isPortRange == 1)
        {
            long long ipboard1 = ((long long)(urls[i].a1) * 1000000000) + (urls[i].b1 * 1000000) + (urls[i].c1 * 1000) + (urls[i].d1);
            long long ipboard2 = ((long long)(urls[i].a2) * 1000000000) + (urls[i].b2 * 1000000) + (urls[i].c2 * 1000) + (urls[i].d2);
            long long currentIp = (long long)(a) * 1000000000 + b * 1000000 + c * 1000 + d;
            if (currentIp >= ipboard1 && currentIp <= ipboard2)
            {
                if (port >= urls[i].port1 && port <= urls[i].port2)
                {
                    flag = 0;
                    break;
                }
            }
            else
            {
                flag = 1;
            }
        }
    }
    if (flag == 0)
    {
        printf("Packet from %d.%d.%d.%d and port %d rejected\n", a, b, c, d, port);
    }
    else
    {
        printf("Packet from %d.%d.%d.%d and port %d accepted\n", a, b, c, d, port);
    }
}

int main(int argc, char **argv)
{
    // read the filename
    char *fileName = *(argv + 1);
    char *ipAddress = *(argv + 2);
    char *port = *(argv + 3);
    // printf("%s\n%s\n", ipAddress, port);

    URL urls[9999];

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
                // printf("Ill formed rule:%d.%d.%d.%d %d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1,urls[count].port1);
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
                // printf("Ill formed rule:%d.%d.%d.%d-%d.%d.%d.%d %d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1,urls[count].a2, urls[count].b2, urls[count].c2, urls[count].d2,urls[count].port1);
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
                // printf("Ill formed rule:%d.%d.%d.%d %d-%d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1,urls[count].port1, urls[count].port2);
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
                // printf("Ill formed rule:%d.%d.%d.%d-%d.%d.%d.%d %d-%d\n", urls[count].a1, urls[count].b1, urls[count].c1, urls[count].d1,urls[count].a2, urls[count].b2, urls[count].c2, urls[count].d2,urls[count].port1,urls[count].port2);
                continue;
            }
            count++;
            continue;
        }
        else
        {
            // printf("Ill formed rule:%s", ip);
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
            if (smallerThan(urls[j], urls[j + 1]) == 0)
            {
                URL temp;
                temp = urls[j];
                urls[j] = urls[j + 1];
                urls[j + 1] = temp;
            }
        }
    }

    isBanned(ipAddress, &port, urls, count);

    return 0;
}
