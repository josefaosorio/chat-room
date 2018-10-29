/*
 * Filename: macros.h
 * Name: Herman Tong, Josefa Osorio, Jessica Hardey
 * Netid: ktong1, josorio2, jhardey
 *
 */

#pragma once

#define SEM_CHECK(f) \
    if ((rc = (f)) != 0) { \
        std::cerr << strerror(rc) << std::endl; \
        exit(EXIT_FAILURE); \
    }

#define Sem_init(s, p, v) \
    SEM_CHECK(sem_init(s, p, v));

#define Sem_wait(s) \
    SEM_CHECK(sem_wait(s));

#define Sem_post(s) \
    SEM_CHECK(sem_post(s));
