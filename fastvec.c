/*
 *
 *		fastvec.c
 *
 *  Created by Andrey Derevyagin and Boris Trofimoff on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */
 
#include <string.h>
#include <stdlib.h>

typedef struct
{
  size_t count;
  size_t compacity;
  size_t compacity_diff;
  size_t rec_size;
} list_impl;

/*int max_value(int i,int j)
{
  return i>j?i:j;
}
*/

int vec_init(void** ptr, size_t rec_size,size_t start_count,size_t compacity_diff)
{
  *ptr = malloc( sizeof(list_impl) + (start_count + compacity_diff) * rec_size);
  if(*ptr==NULL)
    return -1;
  ((list_impl*)*ptr)->count = start_count;
  ((list_impl*)*ptr)->compacity = start_count + compacity_diff;
  ((list_impl*)*ptr)->compacity_diff = compacity_diff;
  ((list_impl*)*ptr)->rec_size = rec_size;
  *ptr =((list_impl*)*ptr) + 1;
  return 0;
}

void vec_remove (void* this,size_t num)
{
  list_impl* temp = (list_impl*)this - 1;
  if(num>=0 && num<temp->count)
  {    
    for(size_t i  = num;i<temp->count-1;i++) 
      memcpy( (char*)this + i * temp->rec_size, (char*)this + (i+1) * temp->rec_size  ,temp->rec_size);
    temp->count--;
  }
}

/*
void vec_del (void** this,size_t num)
{
  //list_impl* temp = (list_impl*)*this - 1;
  // todo
  
}
*/

int vec_add (void** this,void* item)
{
  list_impl* temp = (list_impl*)*this - 1;
  if ( temp->count==temp->compacity)
  {
    temp->compacity += temp->compacity_diff;
    void* temp2 = realloc( temp, sizeof(list_impl) + temp->compacity * temp->rec_size );
    if(!temp2)
      return -1;     
    *this = (list_impl*)temp2 + 1;
    temp = temp2;
  }
  temp->count++;
  if(item)
    memcpy( (char*)(*this) + (temp->count-1) * temp->rec_size,(char*)item,temp->rec_size);
  return temp->count-1;
}

int vec_setsize(void** this, size_t newsize)
{
  list_impl* temp = (list_impl*)*this - 1;
  
  if(temp->compacity>=newsize)
  {
    temp->count = newsize;
  }  
  else
  {
    temp->compacity = temp->count = newsize;
    void* temp2 = realloc( temp, sizeof(list_impl) + temp->compacity * temp->rec_size );
    if(!temp2)
      return -1;
    *this = (list_impl*)temp2 + 1;
  }
  return 0;      
}


size_t vec_count(void* this)
{
  list_impl* temp = (list_impl*)this - 1;
  return temp->count;
}

void vec_free(void* this)
{
  list_impl* temp = (list_impl*)this - 1;
  free(temp);
}

void* vec_clone(void* this)
{
  list_impl* temp = (list_impl*)this - 1;
  size_t count = sizeof(list_impl) + temp->compacity * temp->rec_size;
  void* temp2 = malloc (count);
  memcpy(temp2,temp,count);
  return temp2;
}
