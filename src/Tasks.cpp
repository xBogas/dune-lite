#include "Tasks.h"
#include "Timer.h"

struct ElemTasks
{
  Tasks* index;
  ElemTasks* next;
};

struct LTasks_t
{
  ElemTasks* first;
  uint16_t size;
};

static LTasks_t list_tasks = {
  .first = NULL,
  .size = 0
};

void add_task(Tasks *obj)
{
  obj->debug("Adding Task");
  ElemTasks* ptr =  list_tasks.first;
  ElemTasks* new_obj = new ElemTasks{obj, ptr};
  list_tasks.first = new_obj;
  list_tasks.size++;
}

void start_tasks()
{
  debug("start_tasks()" , "Starting tasks");
  debug("start_tasks()" , "List has %d tasks", list_tasks.size);
  
  uint16_t number = list_tasks.size;
  ElemTasks* ptr = list_tasks.first;
  while (number > 0)
  {
    ptr->index->debug("Requesting task to start");
    ptr->index->start();
    ptr = ptr->next;
    number--;
  }

}

Tasks::Tasks(const char* _name): 
  name(_name), 
  timer(NULL)
{
  add_task(this);
}

Tasks::~Tasks()
{
  debug("Ending task");
}

void Tasks::stop()
{
  debug("Stopping task ");
}

extern bool serial_ready;

void Tasks::debug(const char* format, ...)
{
  char bfr[128];
  va_list args;
  va_start(args, format);
  size_t size = vsprintf(bfr, format, args);
  va_end(args);
  char task_info[256];
  sprintf(task_info, "[%s]: ", name);
  strncat(task_info, bfr, size);
  if (serial_ready)
    Serial.println(task_info);
  else
    add_msg(task_info);
}