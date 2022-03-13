
#include"storage.h"
int compare(const void *pa, const void *pb)
{
    cl_opencl_object *pa_ = (cl_opencl_object*)pa;
    cl_opencl_object *pb_ = (cl_opencl_object*)pb;
    
    char str_ptr_1[64];
    char str_ptr_2[64];
    sprintf(str_ptr_1,"%p",pa_->object_local);
    sprintf(str_ptr_2,"%p",pb_->object_local);
    
    return strcmp(str_ptr_1,str_ptr_2);
   
   /* if (pa_->object_local < pb_->object_local)
        return -1;
    
    
    if (pa_->object_local > pb_->object_local)
        return 1;
    
    
    if (pa_->object_local == pb_->object_local)
        //return strcmp(pa_->daemon->ip, pb_->daemon->ip);
        return 0;*/
}

void * register_object(void *object_remote, void *object_local, remote_daemon * daemon) 
{
 
    /*
     * Primeiro passo é construir  o objecto que vai guardar a informação na arvore, guardando os dados dos apontadores
     * e o daemon em qual estes apontadores são válidos, no que toca, ao daemon, importa deixar registado que é um apontador  para a estrutura onde estão armazenados os daemons, que é construida no ccl_connect_daemons.
     */
    
    cl_opencl_object *object = malloc(sizeof(cl_opencl_object));
    object->object_local = object_local;
    object->object_remote = object_remote;
    object->ptr_local=NULL;
    object->ptr_remote=NULL;
    object->daemon = daemon;
    
    void *ptr = NULL;
    pthread_mutex_lock(&mutex_tree_objects);
        ptr = tsearch((void*) object, &tree_objects, compare);
    pthread_mutex_unlock(&mutex_tree_objects);
    return ptr;
}

void * lookup_object(void *object_local) 
{
    cl_opencl_object obj ;
    obj.object_local = object_local;
    void * ptr = NULL;
    ptr = tfind((void*) &obj, &tree_objects, compare);
    return ptr;
}

void * delete_object(void *object_local) 
{
     
    pthread_mutex_lock(&mutex_tree_objects);
    void * pt= tdelete((void*) object_local, &tree_objects, compare);
   
    pthread_mutex_unlock(&mutex_tree_objects);
}
