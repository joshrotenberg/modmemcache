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

#include <ctype.h>
#include "mod_memcache.h"

extern module AP_MODULE_DECLARE_DATA memcache_module;

typedef struct memcache_cfg {
  

} memcache_cfg;

static void *memcache_cfg(apr_pool_t *p, server_rec *s)
{
  memcache_cfg *cfg = (memcache_cfg *) apr_pcalloc(p, sizeof(memcache_cfg));


}

static const char *memcache_srvr (cmd_parms *cmd, void *mconfig,
                     char *server, char *arg)
{

}

static const command_rec memcache_cmds[] = {
  AP_INIT_ITERATE2("MemcacheServer", memcache_srvr, NULL, RSRC_CONF,
                   "hi "),
  {NULL}
};

static void memcache_hooks(apr_pool_t *pool)
{
  ap_hook_child_init((void *)memcache_setup_init, NULL, NULL, APR_HOOK_MIDDLE);

}

module AP_MODULE_DECLARE_DATA memcache_module = {
  STANDARD20_MODULE_STUFF,
  NULL,
  NULL,
  memcache_cfg,
  memcache_merge,
  memcache_cmds,
  memcache_hooks
};
