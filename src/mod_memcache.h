/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file mod_memcache.h
 * @brief Memcached Client Extenstion module for Apache
 *
 */

#ifndef MEMCACHE_H
#define MEMCACHE_H

#if !defined(WIN32)
#define MEMCACHE_DECLARE(type)            type
#define MEMCACHE_DECLARE_NONSTD(type)     type
#define MEMCACHE_DECLARE_DATA
#elif defined(MEMCACHE_DECLARE_STATIC)
#define MEMCACHE_DECLARE(type)            type __stdcall
#define MEMCACHE_DECLARE_NONSTD(type)     type
#define MEMCACHE_DECLARE_DATA
#elif defined(MEMCACHE_DECLARE_EXPORT)
#define MEMCACHE_DECLARE(type)            __declspec(dllexport) type __stdcall
#define MEMCACHE_DECLARE_NONSTD(type)     __declspec(dllexport) type
#define MEMCACHE_DECLARE_DATA             __declspec(dllexport)
#else
#define MEMCACHE_DECLARE(type)            __declspec(dllimport) type __stdcall
#define MEMCACHE_DECLARE_NONSTD(type)     __declspec(dllimport) type
#define MEMCACHE_DECLARE_DATA             __declspec(dllimport)
#endif

#include <httpd.h>
#include <apr_optional.h>
#include <apr_hash.h>

typedef struct {
  apr_memcache_t *handle;
  apr_pool_t *pool;
} ap_memcache_t;

MEMCACHE_DECLARE_NONSTD(ap_memcache_t*) ap_memcache_client(apr_memcache_t **mc);

#endif


