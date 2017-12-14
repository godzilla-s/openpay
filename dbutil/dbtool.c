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
    "_order_by_desc_",
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

typedef struct vector_s
{
    int  cap;
    char **arr;
} vector_t;

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
    char        fname[256];
    int         tag;
    vector_t    *args;
} function_t;

typedef struct table_s 
{
    char        tblName[51];
    vector_t    *fields;
    vector_t    *function;
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
    destroy();
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

vector_t *vector_new()
{
    vector_t *vec = (vector_t *)mem_alloc(sizeof(vector_t));
    vec->cap = 0;
    vec->arr = (char **)mem_alloc(16 * sizeof(char *));
    return vec;
}

int vector_add(vector_t *vec, char *data)
{
    if(vec->cap >= 16)
    {
        // re_alloc
    }
    int index = vec->cap;
    vec->arr[index] = data;
    vec->cap++;
    return 0;
}

char *vector_get(vector_t *vec, int index)
{
    if (index >= vec->arr)
        return NULL;
    return vec->arr[index];
}

int vector_size(vector_t *vec)
{
    if(vec == NULL)
        return 0;
    return vec->cap;
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
    if(offset >= strlen(fileBuf))
        return NULL;
    memset(line, 0, 256);
    for(i=offset, j=0; fileBuf[i] != '\n';)
    {
        if(fileBuf[i] == '\0')
            break;
        line[j++] = fileBuf[i++];
    }
    offset = i+1;
    return line;
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
    int k = 0;
    int numFld = vector_size(table->fields);
    field_t *field;
    // printf("numfld: %d\n", numFld);
    for(k=0; k<numFld; k++)
    {  
        field = (field_t *)vector_get(table->fields, k);
        if(strcmp(field->name, name) == 0)
        {
            return field;
        }
    }
    return NULL;
}


vector_t* splitWord(char *buf, char *tag)
{
    vector_t *vec = vector_new();
    char *p = strtok(buf, tag);
    while(p)
    {
        //vec->arr[i++] = p;
        vector_add(vec, p);
        p = strtok(NULL, tag);
    }
    return vec;
}

table_t *parseTableStruct() 
{
    char    key[31] = {'\0'};
    char    fName[32];
    char    fType[10];
    int     len = 0;
    table_t  *table;
    field_t  *field;

    table = (table_t *)mem_alloc(sizeof(table_t));
    char  *p = ltrim(getLine());
    sscanf(p, "%[^ ] %s", key, table->tblName);
    if (!define_key_check(key))
    {
        fprintf(stderr, "undefine table key:%s\n", key);
        exit_it(1);
    }

    printf("parse begin\n");
    table->fields = vector_new();
    while((p = ltrim(getLine())) != NULL)
    {
        if(strcmp(p, "end") == 0)
            break;
        //printf("table: %s\n", p);
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
        vector_add(table->fields, (char *)field);
    }

    //printf("field number: %d\n", table->numfld);
    //printf("tableName: %s\n", table->tblName);

    //printmem(memory, 1024);
    char *ptrim = NULL;
    vector_t *vec;
    function_t *func;
    table->function = vector_new();
    while(1)
    {
        p = getLine();
        if(p == NULL)
        {
            break;
        }
        //printf("func: %s\n", p);
        ptrim = ltrim(p);
        if(ptrim == NULL)
        {
            continue;
        }

        //printf("func trim: %s\n", ptrim);
        if(strcmp(ptrim, "end") == 0)
        {
            break;
        }
 
        vec = splitWord(ptrim, " ");
        //printf("vec size: %d\n", vec->cap);
        if(strcmp(vec->arr[0], "end") == 0)
            break;
        if (!define_key_check(vec->arr[0]))
        {
            fprintf(stderr, "invalid function: %s\n", vec->arr[0]);
            exit_it(1);
        }
        
        function_t *func;
        args_t  *args;
        int v = 0;
        if(strcmp(vec->arr[0], "va_func") == 0)
        {
            func = (function_t *)mem_alloc(sizeof(function_t));
            func->args = vector_new();
            func->tag = 0;
            strcpy(func->fname, vec->arr[1]); 
            //printf("func name:%s, %d\n", func->fname, vec->cap);
            for(v=2; v<vec->cap; v++)
            {
                args = (args_t *)mem_alloc(sizeof(args_t));
                if(strcmp(vec->arr[v], "end") == 0)
                    break;
                if(sql_key_check(vec->arr[v]))
                {
                    strcpy(args->symbol, vec->arr[v]);
                    v++;
                }
                
                if(strcmp(vec->arr[v], "_as_") == 0)
                {
                    memset(func->fname, 0, sizeof(func->fname));
                    strcpy(func->fname, vec->arr[++v]);
                    func->tag = 1;
                    continue;
                }
                
                args->fields = getFieldByName(table, vec->arr[v]);
                if(args->fields == NULL)
                {
                    printf("invalid field:%s\n", vec->arr[v]);
                    exit_it(1);
                }  
                //printf("=== %s\n", args->fields->name);
                vector_add(func->args, (char *)args);
            }
            vector_add(table->function, (char *)func);
        }
    }

    return table;
}

char *cvtSql(vector_t *fields, char *tblName, function_t *func)
{
    char  placeHolder[128] = {'\0'};
    char fieldStr[1024] = {'\0'};
    field_t  *field;
    int k;
    char *sql = mem_alloc(512);
    for(k=0; k<vector_size(fields)-1; k++)
    {
        field = (field_t *)vector_get(fields, k);
        strcat(fieldStr, field->name);
        strcat(fieldStr, ", ");
        strcat(placeHolder, "?,");
    }
    field = (field_t *)vector_get(fields, k);
    strcat(fieldStr, field->name);
    strcat(placeHolder, "?");

    if(strcmp(func->fname, "add") == 0)
    {
        sprintf(sql, "INSERT INTO %s(%s) VALUES(%s)", tblName, fieldStr, placeHolder);
    }
    if(strcmp(func->fname, "read_by") == 0)
    {
        sprintf(sql, "SELECT %s FROM %s", fieldStr, tblName);
    }
    return sql;
}

char *toFuncName(function_t *func, char *tblName)
{
    args_t *vargs;
    int     k;
    char  *funcName = mem_alloc(256);
    char  argstr[256] = {'\0'};
    strcat(funcName, "int ");
    //printf("func tag: %d\n", func->tag);
    if(func->tag == 1)
    {
        strcat(funcName, func->fname);
        return NULL;
    }

    strcat(funcName, func->fname);
    for(k=0; k<vector_size(func->args); k++)
    {
        vargs = (args_t *)vector_get(func->args, k);
        if(strlen(vargs->symbol)>0)
        {
            strcat(funcName, vargs->symbol);
            strcat(funcName, vargs->fields->name);
        }
        else
        {
            strcat(funcName, "_");
            strcat(funcName, vargs->fields->name);
        }

        if(strcmp(vargs->fields->type, "int") == 0)
            strcat(argstr, "int ");
        if(strcmp(vargs->fields->type, "str") == 0)
            strcat(argstr, "char *");
        if(strcmp(vargs->fields->type, "double") == 0)
            strcat(argstr, "double ");
        strcat(argstr, vargs->fields->name);
        strcat(argstr, ", ");
    }

    strcat(funcName, "(");
    strcat(funcName, argstr);
    strcat(funcName, tblName);
    strcat(funcName, " *_o_data)");
    //printf("func name: %s\n", funcName);

    return funcName;
}

char *funcArgs(function_t *func)
{
    int k;
    args_t *arg;
    for(k = 0; k<vector_size(func->args); k++)
    {
        arg = (args_t *)vector_get(func->args, k);

    }
}

int fmtSelect(char *funcName, function_t *func, table_t *table) 
{
    char *sql = cvtSql(table->fields, table->tblName, func);
    fprintf(stderr, "%s\n", funcName);
    fprintf(stderr, "{\n");
    fprintf(stderr, "\tint ret;\n");
    fprintf(stderr, "\t%s _raw;\n", table->tblName);
    fprintf(stderr, "\tmemset(&_raw, '0', sizeof(_raw));\n");
    fprintf(stderr, "\tret = %s(\"%s\")\n", "db_va_read_one", sql);
    fprintf(stderr, "\tif (ret)\n\t{\n\t\treturn ret;\n\t}\n");
    fprintf(stderr, "\treturn 0;\n}\n\n");
    return 0;
}

int fmtInsert(char *funcName, function_t *func, table_t *table)
{
    char *sql = cvtSql(table->fields, table->tblName, func);
    fprintf(stderr, "%s\n", funcName);
    fprintf(stderr, "{\n");
    fprintf(stderr, "\tint ret;\n");
    fprintf(stderr, "\tret = %s_%s(\"%s\");\n", table->tblName, funcName, sql);
    fprintf(stderr, "\tif (ret)\n\t{\n\t\treturn ret;\n\n}");
    fprintf(stderr, "\treturn 0;\n}\n\n");
}

int main(int args, char *argv[])
{
    init();

    readFile("temp.act");
    // printf("filebuf: %s\n", fileBuf);
    table_t *table = parseTableStruct();

    int numFunc = vector_size(table->function);
    char *fname = NULL;
    function_t *func = NULL;
    for(j=0; j<numFunc; j++)
    {
        func = (function_t *)vector_get(table->function, j);
        fname = toFuncName(func, table->tblName);
        if(fname == NULL)
        {
            continue;
        }
       
        if(strcmp(func->fname, "add") == 0)
            fmtInsert(fname, func, table);
        if(strcmp(func->fname, "read_by") == 0)
            fmtSelect(fname, func, table);
    }
    destroy();
    return 0;
}
// dbtool -f db_server.act -o
