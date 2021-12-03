#ifndef LIST_H
#define LIST_H

#define EXISTS_IN_LIST 1
#define NOT_EXISTS_IN_LIST 0

/**
 * @brief List of integers
 * 
 */
typedef struct List
{
  int *data;
  int size;
} int_list_t;

void create_int_list(int_list_t *);
void add_element_to_list(int_list_t *list, int element);
int is_element_in_list(int_list_t *list, int element);
void delete_element_from_list(int_list_t *list, int index);
void destroy_list(int_list_t *list);

#endif