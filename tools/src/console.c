/** \copyright
 * Copyright (c) 2012, Stuart W. Baker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "console.h"

#ifndef MAX_ARGS
#define MAX_ARGS 10
#endif

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/** Metadata for a command.
 */
typedef struct command
{
    const char     *cmd;      /**< name of command */
    ConsoleFunction function; /**< function callback for the command */
    void           *context;  /**< context to pass to the command callback */
    struct command *next;     /**< next command in the command list */
} Command;

static int helpCommand(FILE *fp, int argc, const char *argv[], void *context);

static Command head = {"help", helpCommand, NULL, NULL};

/** Print out all the commands and their help.
 * @param fp file pointer to calling console
 * @param argc number of arguments
 * @param argv arguments
 * @param context context pointer
 */
static int helpCommand(FILE *fp, int argc, const char *argv[], void *context)
{
    fprintf(fp, "%15s - print this help menu\n", "?");
    fprintf(fp, "%15s - print this help menu\n", "help");

    const char *args[2];
    args[1] = "                  ";

    Command *current = head.next;

    pthread_mutex_lock(&mutex);
    while (current)
    {
        if (strcmp(current->cmd, "?"))
        {
            fprintf(fp, "%15s - ", current->cmd);
            args[0] = current->cmd;
            (*current->function)(fp, 0, args, current->context);
        }
        current = current->next;
    }
    pthread_mutex_unlock(&mutex);
    return COMMAND_OK;
}

/** Terminate the program.
 * @param fp file pointer to calling console
 * @param argc number of arguments
 * @param argv arguments
 * @param context context pointer
 */
static int exitCommand(FILE *fp, int argc, const char *argv[], void *context)
{
    switch (argc)
    {
        default:
            return COMMAND_ERROR;
            break;
        case 0:
            fprintf(fp, "terminate the program\n");
            break;
        case 1:
            exit(0);
            break;
    }
    return COMMAND_OK;
}

/** Place a command prompt on the console.
 * @param fp file pointer to print prompt to
 */
static void prompt(FILE *fp)
{
    fputs("> ", fp);
    fflush(fp);
}

/** Call the correct command callback.
 * @param priv private data for the console
 * @param fp file pointer to calling console
 * @param argc number of arguments
 * @param argv arguments
 */
static void callback(FILE *fp, int argc, const char *argv[])
{
    Command *current = &head;

    pthread_mutex_lock(&mutex);
    while (current)
    {
        if (strcmp(current->cmd, argv[0]) == 0)
        {
            int result = (*current->function)(fp, argc, argv, current->context);
            pthread_mutex_unlock(&mutex);
            if (result != COMMAND_OK)
            {
                fprintf(fp, "invalid arguments\n");
            }
            return;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&mutex);
    fprintf(fp, "%s: command not found\n", argv[0]);
}

/** Main console thread.
 */
static void *thread(void* data)
{
    char *line = malloc(64);
    size_t size = 64;
    size_t index = 0;

    prompt(stdout);

    for (; /* forever */ ;)
    {
            ssize_t ssize = read(0, line + index, size - index);
            if (ssize < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                printf("read failed: %s\n", strerror(errno));
                abort();
            }
            else if (ssize == 0)
            {
                continue;
            }
            for (size_t j = 0; j < ssize; j++)
            {
                if (line[index + j] == '\n')
                {
                    int argc = 0;
                    const char *args[MAX_ARGS];
                    char last = '\0';

                    for (size_t k = 0; k <= (index + j); k++)
                    {
                        switch (line[k])
                        {
                            case '\r':
                            case '\n':
                            case ' ':
                                line[k] = '\0';
                                break;
                            case '"':
                            default:
                                if (last == '\0')
                                {
                                    args[argc] = &line[k];
                                    argc = (argc == MAX_ARGS) ? MAX_ARGS : argc + 1;
                                }
                                break;
                        }
                        last = line[k];
                    }

                    switch (argc)
                    {
                        case 0:
                            break;
                        case MAX_ARGS:
                            fprintf(stdout, "too many arguments\n");
                            break;
                        default:
                            callback(stdout, argc, args);
                            break;
                    }
                        
                    prompt(stdout);
                    index = 0;
                    ssize = ssize - (j + 1);
                    memcpy(line, &line[index + j + 1], index);
                }
            }
            index += ssize;
            if (index == size)
            {
                char *newline = malloc(size * 2);
                memcpy(newline, line, size);
                free(line);
                size *= 2;
                line = newline;
                fprintf(stdout, "expanded buffer to %zu bytes\n", size);
            }
    }
    return NULL;
}

/*
 * console_add
 */
void console_add(const char *cmd, ConsoleFunction callback, void *context)
{
    if (cmd == NULL || callback == NULL)
    {
        /* invalid parameter */
        return;
    }

    Command *command = malloc(sizeof(Command));

    pthread_mutex_lock(&mutex);
    command->cmd = cmd;
    command->function = callback;
    command->context = context;
    command->next = head.next;
    head.next = command;
    pthread_mutex_unlock(&mutex);
}

/*
 * console_start
 */
void console_start(int consume)
{
    console_add("exit", exitCommand, NULL);
    console_add("?", helpCommand, NULL);


    if (consume)
    {
        thread(NULL);
    }

    pthread_attr_t attr;
    pthread_t      thread_handle;
    int result = pthread_attr_init(&attr);
    if (result != 0)
    {
        return;
    }
#if !defined(__linux__) /* Linux allocataes stack as needed */
    result = pthread_attr_setstacksize(&attr, 4096);
    if (result != 0)
    {
        return;
    }
#endif

    pthread_create(&thread_handle, &attr, thread, NULL);
}
