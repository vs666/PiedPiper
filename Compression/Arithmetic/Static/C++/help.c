int help()
{
    FILE *fp = fopen("README.md","r");
    if (fp!=NULL)
    {
        char line[1024];
        while (fgets(line,sizeof(line),fp)!=NULL)
        {
            printf("%s",line);
        }
        fclose(fp);
    }
    else
    {
        perror("");
        return 1;
    }
}
