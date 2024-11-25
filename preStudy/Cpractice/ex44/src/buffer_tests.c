#include "minunit.h"
#include <list.h>
#include <assert.h>
#include "ringbuffer.h"

RingBuffer *buffer = NULL;
char *test_create()
{

    buffer = RingBuffer_create(100);
    mu_assert(buffer != NULL, "Failed to make buffer.");
    return NULL;
}

char *test_destroy()
{
    RingBuffer_destroy(buffer);
    return NULL;
}

char *test_write_read()
{

    int rc = RingBuffer_write(buffer, "hello", 5);
    mu_assert(rc == 5, "Failed to write hello.");
    mu_assert(RingBuffer_available_data(buffer) == 5, "Wrong data length.");

    rc = RingBuffer_write(buffer, "Zed", 3);
    mu_assert(rc == 3, "Failed to write Zed.");
    mu_assert(RingBuffer_available_data(buffer) == 8, "Wrong data length.");

    char target[8] = {'a'};
    rc = RingBuffer_read(buffer, target, 8);
    mu_assert(rc == 8, "Failed to read.");
    mu_assert(target[0] == 'h', "Wrong first char.");
    mu_assert(target[7] == 'd', "Wrong last char.");
    mu_assert(RingBuffer_available_data(buffer) == 0, "Wrong data length after read.");

    return NULL;
}

char *test_all(){
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_write_read);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(test_all);
