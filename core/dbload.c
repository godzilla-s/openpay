// 数据库加载

#include "dbload.h"

int 
db_read_service()
{
    int     ret;
    db_service_t  service;
    memset(&service, 0, sizeof(db_service_t));
    ret = db_read_service_all("1", &service);

    return ret;
}

int 
db_read_trade() 
{

}

int 
db_read_module()
{
    
}