// dbtool 自动生成数据库操作的工具
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

char defineKey[][20] = {
    "va_table",
    "va_func",
    "va_index",
    "va_uk"
};

char funcKey[][20] = {
    "read_by",
    "update_by",
    "add",
    "delete_by",
    "open_select_by",
    "fetch_select",
    "close_select"
};

char sqlKey[][20] = {
    "_order_by_",
    "_order_by_desc",
    "_gt_",
    "_lt_",
    "_ge_",
    "_le_",
    "_limit_"
};

char  fldType[][10] = {
    "int",
    "str",
    "double",
    "date"
};

// 
char *fileBuf = NULL; 
char line[256] = {'\0'};
int  offset = 0;

typedef struct fields_s
{
    char    name[32];
    char    type[10];
    int     length;
} field_t;

typedef struct args_s
{
    field_t *fields;
    char    symbol[20];
} args_t;

typedef struct function_s
{
    char    fname[256];
    int     numArgs;
    args_t  *args;
} function_t;

typedef struct table_s 
{
    char        tblName[51];
    int         numfld;
    field_t     *fields;
    int         numfunc;
    function_t  *functions;
} table_t;

int i, j;
char   *memory = NULL;
int     pointer = 0;
#define ALLOC_DEFAULT  1024 * 1024
#define SIZE_INT   sizeof(int)

void init()
{
    memory = malloc(ALLOC_DEFAULT);
    memset(memory, 0, ALLOC_DEFAULT);
    pointer = 0;
}

void destroy()
{
    if(memory == NULL)
        return;
    free(memory);
    printf("rest of memory: %ld\n", ALLOC_DEFAULT - pointer);
}

char *mem_alloc(int size)
{
    if(pointer + SIZE_INT + size > ALLOC_DEFAULT)
        return NULL;
    char *ptr = memory + pointer;
    memcpy(ptr, &size, sizeof(int));
    pointer += size + SIZE_INT;
    return ptr + SIZE_INT;
}

char *mem_next(char *ptr)
{
    return NULL;
}

void exit_it(int no)
{
    if(fileBuf)
        free(fileBuf);
    exit(no);
}

// left trim 
char *ltrim(char *buf)
{
    if(buf == NULL)
        return NULL;
    for( ;*buf != '\0'; *buf++)
    {
        if(*buf == 0x20 || *buf == '\t')
        {   
            continue;
        }
        return buf;
    }
    return NULL;
}

// right trim
char *rtrim(char *buf, int len)
{
    return buf;
}

//
#define dk_count (sizeof(defineKey)/20)
int define_key_check(char *key)
{
    for(i=0; i<dk_count; i++)
    {
        if(strcmp(defineKey[i],key) == 0)
            return 1;
    }
    return 0;
}

//
#define fk_count sizeof(funcKey)/20
int func_key_check(char *key)
{
    for(i=0; i<fk_count; i++)
    {
        if(strcmp(funcKey[i], key) == 0)
            return 1;
    }
    return 0;
}

//
#define sk_count sizeof(sqlKey)/20
int sql_key_check(char *key)
{
    for(i=0; i<sk_count; i++)
    {
        if(strcmp(sqlKey[i], key) == 0)
            return 1;
    }
    return 0;
}

// 
#define ft_count sizeof(fldType)/20
int field_type_check(char *type)
{
    for(i=0; i<ft_count; i++)
    {
        if(strcmp(fldType[i], type) == 0)
            return 1;
    }
    return 0;
}

// read file
int readFile(char *f)
{
    FILE *fp = fopen(f, "r");
    if(!fp)
    {
        fprintf(stderr, "fail to open:%s\n", strerror(errno));
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fileBuf = mem_alloc(size+1);
    if (!fileBuf)
        exit(1);
    
    fseek(fp, 0, SEEK_SET);

    fread(fileBuf, 1, size, fp);

    fclose(0);
    return 0;
}

// get line
char *getLine()
{
    memset(line, 0, 256);
    for(i=offset, j=0; fileBuf[i] != '\n';)
    {
        line[j++] = fileBuf[i++];
    }
    offset = i+1;
    return ltrim(line);
}

void printmem(char *ptr, int size)
{
    int x;
    for(x=0; x<size; x++)
    {
        printf("[%c]", ptr[x]);
    }
    printf("\n");
}

field_t *getFieldByName(table_t *table, char *name)
{
    field_t *field = NULL;
    char *ptr = (char *)table->fields;
    for(i=0; i<table->numfld; i++)
    {
       
        field = (field_t *)(ptr + i * (sizeof(field_t) + SIZE_INT));
        //printf("get:%d, %d, %d\n", ptr, i * (sizeof(field_t) + SIZE_INT), field);
        //printf("cmp:%s\n", field->name);
        if(strcmp(field->name, name) == 0)
        {
            printf("compare ok:%s,%s\n", field->name, name);
            return field;
        }
        
    }
    return NULL;
}

char word[100] = {'\0'};
char *nextWord(char *buf)
{
    memset(word, 0, sizeof(word));
    char *p = buf;
    for(i=0; *buf != '\0'; i++)
    {
        ;
    }
}

int parseTableStruct() 
{
    char    key[31] = {'\0'};
    char    fName[32];
    char    fType[10];
    int     len = 0;
    table_t  table;
    field_t  *field;

    char  *p = ltrim(getLine());
    memset(&table, 0, sizeof(table));
    sscanf(p, "%[^ ] %s", key, table.tblName);
    if (!define_key_check(key))
    {
        fprintf(stderr, "undefine table key:%s\n", key);
        exit_it(1);
    }

    while((p = ltrim(getLine())) != NULL)
    {
        if(strcmp(p, "end") == 0)
            break;

        memset(fName, 0, sizeof(fName));
        memset(fType, 0, sizeof(fType));
        len = 0;
        sscanf(p, "%[^ ] %[^ ] %d", fName, fType, &len);
        if(!field_type_check(fType))
        {
            fprintf(stderr, "undefined field <%s> type:%s\n", fName, fType);
            exit_it(1);
        }
        //printf("%s, %s, %d\n", fName, fType, len);
        field = (field_t *)mem_alloc(sizeof(field_t));
        //printf("# %d\n", field);
        strcpy(field->name, fName);
        strcpy(field->type, fType);
        field->length = len;
        if(table.numfld == 0)
            table.fields = field;
        table.numfld++;
    }

    printf("field number: %d\n", table.numfld);
    printf("tableName: %s\n", table.tblName);

    //printmem(memory, 1024);

    printf("sizeof field: %ld\n", sizeof(field_t));

    //printmem();
    field_t *f = getFieldByName(&table, "sex");
    if(f == NULL)
    {
        printf("fail to get field\n");
        return -1;
    }

    printf("field: %s, %s, %d\n", f->name, f->type, f->length);
    return 0;
}

int cvtSql()
{
    return 0;
}

int main(int args, char *argv[])
{
    init();
    /*
    char a[] = "    hello world";
    printf("ltrim:%s\n", ltrim(a));

    char b[10] = {'\0'}, c[20] = {'\0'};
    int  d = 0;

    char e[] = "     id      int";
    char *f = ltrim(e);
    sscanf(f, "%[^ ] %s %d", b, c, &d);
    printf("%s, %s, %d\n", b, c, d);

    printf("size: %d\n", sizeof(defineKey));
    */
    readFile("test.act");
    // printf("filebuf: %s\n", fileBuf);
    /*
    printf("%s\n", getLine());
    printf("%s\n", getLine());
    printf("%s\n", getLine());
    */

    parseTableStruct();

    destroy();
    return 0;
}
// dbtool -f db_server.act -o
