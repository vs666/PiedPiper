#include "bwt.h"

#define BUFFER_SIZE 100000

struct pair
{
    char first;
    int second;
};

struct bwt
{
    /* data */
    int key;
};

void merge(struct pair arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    struct pair a[n1], b[n2];
    for (i = 0; i < n1; i++)
    {
        a[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        b[j] = arr[m + 1 + j];
    }
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (a[i].first <= b[j].first ||
            (a[i].first == b[j].first &&
             a[i].second < b[j].second))
        {
            arr[k] = a[i];
            i++;
        }
        else
        {
            arr[k] = b[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = a[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = b[j];
        j++;
        k++;
    }
}
void mergesort(struct pair arr[], int l, int r)
{
    int m;
    if (l < r)
    {
        m = (l + r) / 2;
        mergesort(arr, l, m);
        mergesort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void count_sort(int n, int *p, int *c)
{
    int cnt[BUFFER_SIZE] = {0}, pair[BUFFER_SIZE] = {0};
    for (int i = 0; i < n; i++)
    {
        cnt[c[i]]++;
    }
    pair[0] = 0;
    for (int i = 1; i < n; i++)
    {
        pair[i] = pair[i - 1] + cnt[i - 1];
    }
    int p_new[BUFFER_SIZE];
    for (int i = 0; i < n; i++)
    {
        int x = p[i];
        int itr = c[x];
        p_new[pair[itr]] = x;
        pair[itr]++;
    }
    for (int i = 0; i < n; i++)
    {
        p[i] = p_new[i];
    }
}

int encoder(int n, char *s)
{
    int c[BUFFER_SIZE], c_new[BUFFER_SIZE], p[BUFFER_SIZE];
    struct pair a[BUFFER_SIZE];
    for (int i = 0; i < n; i++)
    {
        a[i].first = s[i];
        a[i].second = i;
    }
    mergesort(a, 0, n - 1);
    for (int i = 0; i < n; i++)
    {
        p[i] = a[i].second;
    }
    c[p[0]] = 0;
    for (int i = 1; i < n; i++)
    {
        if (a[i].first == a[i - 1].first)
        {
            c[p[i]] = c[p[i - 1]];
        }
        else
        {
            c[p[i]] = c[p[i - 1]] + 1;
        }
    }
    int k = 0;
    while ((1 << k) < n)
    {
        for (int i = 0; i < n; i++)
        {
            p[i] = (p[i] - (1 << k) + n) % n;
        }
        count_sort(n, p, c);
        c_new[p[0]] = 0;
        for (int i = 1; i < n; i++)
        {
            struct pair prev = {c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]};
            struct pair now = {c[p[i]], c[(p[i] + (1 << k)) % n]};
            if (now.first == prev.first && now.second == prev.second)
            {
                c_new[p[i]] = c_new[p[i - 1]];
            }
            else
            {
                c_new[p[i]] = c_new[p[i - 1]] + 1;
            }
        }
        for (int i = 0; i < n; i++)
        {
            c[i] = c_new[i];
        }
        k++;
    }
    char s_new[BUFFER_SIZE];
    for (int i = 0; i < n; i++)
    {
        s_new[i] = s[(p[i] + n - 1) % n];
    }
    for (int i = 0; i < n; i++)
    {
        s[i] = s_new[i];
    }
    return c[0];
}

void decoder(int key, int n, char *s)
{
    int start = key;
    struct pair a[BUFFER_SIZE];
    for (int i = 0; i < n; i++)
    {
        a[i].first = s[i];
        a[i].second = i;
    }
    mergesort(a, 0, n - 1);
    char s_new[BUFFER_SIZE];
    for (int i = 0; i < n; i++)
    {
        s_new[i] = a[start].first;
        start = a[start].second;
    }
    for (int i = 0; i < n; i++)
    {
        s[i] = s_new[i];
    }
    s = s_new;
}

int main(int argc, char *argv[])
{
    struct stat stats;

    if (argc != 2)
    {
        char error[] = "Usage: ./encrypt <filename>\n";
        write(2, error, strlen(error));
        return 1;
    }
    if (stat(argv[1], &stats) == 0 && S_ISDIR(stats.st_mode))
    {
        char error[] = "ERROR: please provide file\n";
        write(2, error, strlen(error));
        return 1;
    }
    char s[1005], e[1005] = "_encrypted";
    long long i, j, n = strlen(argv[1]);
    for (i = n - 1; i >= 0; i--)
    {
        if (argv[1][i] == '.')
        {
            break;
        }
    }
    int en = 0;
    struct bwt key;
    if (i >= strlen(e) && strncmp(&argv[1][i - strlen(e)], e, strlen(e)) == 0)
    {
        en = 1;
        strcpy(s, argv[1]);
        s[i - strlen(e) + 1] = 'd';
        s[i - strlen(e) + 2] = 'e';
    }
    else
    {
        strncpy(s, argv[1], i);
        strcpy(&e[strlen(e)], &argv[1][i]);
        strcpy(&s[i], e);
    }
    FILE *fd_in = fopen(argv[1], "r");
    if (fd_in == NULL)
    {
        perror(argv[1]);
        return 1;
    }
    FILE *fd_out = fopen(s, "w");
    if (fd_out == NULL)
    {
        fclose(fd_in);
        perror(s);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    long long SIZE = lseek(fd, 0, SEEK_END);
    close(fd);
    char bytes[100000];
    long long num = SIZE / (BUFFER_SIZE + en * sizeof(key));
    fseek(fd_in, 0, SEEK_SET);
    for (i = 0; i < num; i++)
    {
        if (en)
        {
            fread(&key, sizeof(key), 1, fd_in);
        }
        fread(bytes, BUFFER_SIZE, 1, fd_in);
        if (!en)
        {
            key.key = encoder(BUFFER_SIZE, bytes);
            fwrite(&key, sizeof(key), 1, fd_out);
        }
        else
        {
            decoder(key.key, BUFFER_SIZE, bytes);
        }
        fwrite(bytes, BUFFER_SIZE, 1, fd_out);
    }
    if (SIZE % (BUFFER_SIZE + en * sizeof(key)) > 0)
    {
        num = SIZE % (BUFFER_SIZE + en * sizeof(key));
        if (en)
        {
            fread(&key, sizeof(key), 1, fd_in);
        }
        fread(bytes, num - en * sizeof(key), 1, fd_in);
        if (!en)
        {
            key.key = encoder(num, bytes);
            fwrite(&key, sizeof(key), 1, fd_out);
        }
        else
        {
            decoder(key.key, num - sizeof(key), bytes);
        }
        fwrite(bytes, num - en * sizeof(key), 1, fd_out);
    }
    if (fclose(fd_in) < 0)
    {
        perror(argv[1]);
        return 1;
    }
    if (fclose(fd_out) < 0)
    {
        perror(s);
        return 1;
    }
    return 0;
}
