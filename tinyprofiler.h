#pragma once

#ifdef USE_TINYPROFILER

#ifndef TINYPROFILER_MAX_NUM_OF_THREADS
#define TINYPROFILER_MAX_NUM_OF_THREADS 4
#endif

#ifndef TINYPROFILER_MAX_SCOPE_NAME_LENGTH
#define TINYPROFILER_MAX_SCOPE_NAME_LENGTH 100
#endif

#ifndef TINYPROFILER_MAX_SINGLE_JSON_LINE_LENGTH
#define TINYPROFILER_MAX_SINGLE_JSON_LINE_LENGTH 10000
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct _tinyprofiler_sample_t {
  unsigned long ts;
  int pid;
  int tid;
  char ph;
  char name[TINYPROFILER_MAX_SCOPE_NAME_LENGTH];
};

struct {
  struct _tinyprofiler_sample_t * s;
  size_t sample_count;
  int i;
} _prof_data[TINYPROFILER_MAX_NUM_OF_THREADS] = {0};

unsigned long _tinyprofiler_prof_time_start = 0;
#ifdef _WIN32
unsigned long long _tinyprofiler_perf_freq = 0;
#endif

#ifdef _WIN32
static inline unsigned long _prof_time() {
  unsigned long long counter;
  QueryPerformanceCounter((LARGE_INTEGER *)&counter);
  double result = (double)counter / ((double)_tinyprofiler_perf_freq / 1000000.0);
  return (unsigned long)result;
}
#else
static inline unsigned long _prof_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return 1000000UL * tv.tv_sec + tv.tv_usec;
}
#endif

static inline void _prof(int thread_id, char ph, unsigned long ts, int pid, int tid, size_t size, const char * name) {
  int ti = thread_id;
  int i  = _prof_data[ti].i;
  _prof_data[ti].s[i].ts  = ts;
  _prof_data[ti].s[i].pid = pid;
  _prof_data[ti].s[i].tid = tid;
  _prof_data[ti].s[i].ph  = ph;
  memcpy(_prof_data[ti].s[i].name, name, size);
  _prof_data[ti].i += 1;
}

static inline void profAlloc(size_t sample_count_per_thread) {
#ifdef _WIN32
  BOOL query_perf_freq_fail = QueryPerformanceFrequency((LARGE_INTEGER *)&_tinyprofiler_perf_freq);
  assert(query_perf_freq_fail != 0);
  _tinyprofiler_prof_time_start = _prof_time();
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  _tinyprofiler_prof_time_start = tv.tv_sec;
#endif
  for (int t = 0; t < TINYPROFILER_MAX_NUM_OF_THREADS; t += 1) {
    _prof_data[t].sample_count = sample_count_per_thread;
    _prof_data[t].s = (struct _tinyprofiler_sample_t *)calloc(sample_count_per_thread, sizeof(struct _tinyprofiler_sample_t));
  }
}

#ifndef _TINYPROFILER_DIFF
#ifdef _WIN32
#define _TINYPROFILER_DIFF 0
#else
#define _TINYPROFILER_DIFF (1000000UL * _tinyprofiler_prof_time_start)
#endif
#endif

#ifndef _TINYPROFILER_PRINT
#ifdef _WIN32
#define _TINYPROFILER_PRINT(x) OutputDebugStringA(x)
#else
#define _TINYPROFILER_PRINT(x) fprintf(stderr, x)
#endif
#endif

static inline void profPrintAndFree() {
  unsigned long self_t_begin = _prof_time();
  size_t line_byte_size = TINYPROFILER_MAX_SINGLE_JSON_LINE_LENGTH * sizeof(char);
  char * line = (char *)calloc(TINYPROFILER_MAX_SINGLE_JSON_LINE_LENGTH, sizeof(char));
  snprintf(line, line_byte_size, "{\"traceEvents\":[{}\n"); _TINYPROFILER_PRINT(line);
  for (int t = 0; t < TINYPROFILER_MAX_NUM_OF_THREADS; t += 1) {
    for (size_t i = 0; i < _prof_data[t].sample_count; i += 1) {
      if (_prof_data[t].s[i].ph) {
        snprintf(line, line_byte_size, ",{\"ph\":\"%c\",\"ts\":%lu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n",
                _prof_data[t].s[i].ph,  _prof_data[t].s[i].ts - _TINYPROFILER_DIFF,
                _prof_data[t].s[i].pid, _prof_data[t].s[i].tid, _prof_data[t].s[i].name);
        _TINYPROFILER_PRINT(line);
      } else break;
    }
  }
  for (int t = 0; t < TINYPROFILER_MAX_NUM_OF_THREADS; t += 1) free(_prof_data[t].s);
  snprintf(line, line_byte_size, ",{\"ph\":\"%c\",\"ts\":%lu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n", 'B', self_t_begin - _TINYPROFILER_DIFF, 0, 0, __func__); _TINYPROFILER_PRINT(line);
  snprintf(line, line_byte_size, ",{\"ph\":\"%c\",\"ts\":%lu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n", 'E', _prof_time() - _TINYPROFILER_DIFF, 0, 0, __func__); _TINYPROFILER_PRINT(line);
  snprintf(line, line_byte_size, "]}\n"); _TINYPROFILER_PRINT(line);
  free(line);
}

static inline void profB(const char * name) { _prof(0, 'B', _prof_time(), 0, 0, sizeof(name), name); }
static inline void profE(const char * name) { _prof(0, 'E', _prof_time(), 0, 0, sizeof(name), name); }
static inline void profBmt(int tid, const char * name) { _prof(tid, 'B', _prof_time(), 0, tid, sizeof(name), name); }
static inline void profEmt(int tid, const char * name) { _prof(tid, 'E', _prof_time(), 0, tid, sizeof(name), name); }

#else // USE_TINYPROFILER

static inline void profAlloc(size_t sample_count_per_thread) {}
static inline void profPrintAndFree() {}
static inline void profB(const char * name) {}
static inline void profE(const char * name) {}
static inline void profBmt(int tid, const char * name) {}
static inline void profEmt(int tid, const char * name) {}

#endif // USE_TINYPROFILER