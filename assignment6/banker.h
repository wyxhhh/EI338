#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

void init(char *argv[]);
void auto_calc();
void set_max(int customer_num, int max_resource[]);
int safe_state();
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
void print_result();
