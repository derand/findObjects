/*
 *
 *		fastvec.c
 *
 *  Created by Andrey Derevyagin and Boris Trofimoff on 13/3/10.
 *  Copyright 2010 Andrey Derevyagin. All rights reserved.
 *
 */

//! \brief constructor
int  vec_init(void** ptr,              //!< result ptr
              size_t rec_size,         //!< record size
              size_t start_count,      //!< initial count of element
              size_t compacity_diff);  //!< compacity difference between count and real count of elements - used for fast element inserting

//! count
size_t vec_count  (void* this);

//! element removing without resizing
void vec_remove (void* this,size_t num);

//! element removing with resizing 
//void vec_del (void** this,size_t num);

//! element adding
int vec_add (void** this,           //!< this object
               void* item);         //!< element to inserting, if item==NULL then count++ but item wouldn't copy into vector

//! new count setting
int vec_setsize (void** this,size_t newsize);

//! destructor
void vec_free(void* this);

//! close
void* vec_close  (void* this);
