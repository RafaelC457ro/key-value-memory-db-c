/**
 * MIT License
 *
 * Copyright (c) 2017 Rafael Castro
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "queue.h"

static void should_init_queue(void **state)
{
    Queue *q = queue_init();
    assert_non_null(q);
    assert_null(q->front);
    assert_null(q->rear);
    queue_destroy(q);
}

static void should_enqueue(void **state)
{
    Queue *q = queue_init();
    queue_enqueue(q, 1);
    assert_int_equal(q->front->data, 1);
    assert_int_equal(q->rear->data, 1);
    assert_null(q->front->next);
    queue_destroy(q);
}

static void should_dequeue(void **state)
{
    Queue *q = queue_init();
    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    queue_enqueue(q, 3);
    assert_int_equal(queue_dequeue(q), 1);
    assert_int_equal(queue_dequeue(q), 2);
    assert_int_equal(queue_dequeue(q), 3);
    assert_int_equal(queue_dequeue(q), -1); // TODO: not good since -1 is a valid value, I should fix this later
    queue_destroy(q);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(should_init_queue),
        cmocka_unit_test(should_enqueue),
        cmocka_unit_test(should_dequeue),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}