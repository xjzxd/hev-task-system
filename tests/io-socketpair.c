/*
 ============================================================================
 Name        : io-socketpair.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2018 everyone.
 Description : IO SocketPair Test
 ============================================================================
 */

#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include <hev-task.h>
#include <hev-task-system.h>
#include <hev-task-io.h>
#include <hev-task-io-socket.h>

static void
task_entry (void *data)
{
    int result, fds[2];

    result = hev_task_io_socket_socketpair (PF_LOCAL, SOCK_STREAM, 0, fds);
    assert (result == 0);
    assert (fds[0] >= 0);
    assert (fds[1] >= 0);

    assert ((fcntl (fds[0], F_GETFL) & O_NONBLOCK) == O_NONBLOCK);
    assert ((fcntl (fds[1], F_GETFL) & O_NONBLOCK) == O_NONBLOCK);

    close (fds[0]);
    close (fds[1]);
}

int
main (int argc, char *argv[])
{
    HevTask *task;

    assert (hev_task_system_init () == 0);

    task = hev_task_new (-1);
    assert (task);
    hev_task_run (task, task_entry, NULL);

    hev_task_system_run ();

    hev_task_system_fini ();

    return 0;
}
