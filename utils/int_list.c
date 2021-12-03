#include "int_list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Create a list
 * 
 * @return int_list_t* 
 */
void create_int_list(int_list_t *list)
{
  list = malloc(sizeof(int_list_t));
  list->size = 0;
}

/**
 * @brief Add element to the list if it not exist
 * 
 * @param list 
 * @param element 
 */
void add_element_to_list(int_list_t *list, int element)
{
  if (is_element_in_list(list, element))
    return;

  list->size++;
  list->data = realloc(list->data, list->size * sizeof(int));
  list->data[list->size - 1] = element;
}

/**
 * @brief Check if the element is in the list
 * 
 * @param list 
 * @param element 
 * @return int 
 */
int is_element_in_list(int_list_t *list, int element)
{
  int i;
  for (i = 0; i < list->size; i++)
  {
    if (list->data[i] == element)
    {
      return EXISTS_IN_LIST;
    }
  }
  return NOT_EXISTS_IN_LIST;
}

/**
 * @brief Delete element from the list
 * 
 * @param list 
 * @param element 
 */
void delete_element_from_list(int_list_t *list, int element)
{
  int i;
  for (i = 0; i < list->size; i++)
  {
    if (list->data[i] == element)
    {
      list->data[i] = list->data[list->size - 1];
      list->size--;
      list->data = realloc(list->data, list->size * sizeof(int));
      return;
    }
  }
}

/**
 * @brief Destroy list
 * 
 * @param list 
 */
void destroy_list(int_list_t *list)
{
  free(list->data);
  free(list);
}