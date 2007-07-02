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

static apr_hash_t *memcache_servers;

typedef struct memcache_cfg {
  apr_memcache_t *mc;

} memcache_cfg;

/* config create */
static void *memcache_cfg(apr_pool_t *p, server_rec *s)
{
  memcache_cfg *cfg = (memcache_cfg *) apr_pcalloc(p, sizeof(memcache_cfg));

  
  return cfg;
}

/* config merge */
static void *memcache_merge(apr_pool_t *pool, void *base, void *add)
{
  memcache_cfg *base = (memcache_cfg *) base;
  memcache_cfg *add = (memcache_cfg *) add;
  memcache_cfg *cfg = apr_pcalloc(pool, sizeof(memcache_cfg));


  return (void *) cfg;
}

/* pre-config */
static int memcache_preconfig(apr_pool_t *p, apr_pool_t *plog, 
                              apr_pool_t *ptemp)
{
  memcache_servers = apr_hash_make(ptemp);
  return OK;
}

/* post-config */
static int memcache_postconfig(apr_pool_t *pconf, apr_pool_t *plog,
                               apr_pool_t *ptemp, server_rec *s)
{
  apr_status_t rv;

  return OK;
}

/* parse config options */
static const char *memcache_srvr (cmd_parms *cmd, void *mconfig,
                     char *server, char *arg)
{
  memcache_cfg *cfg = (memcache_cfg *) mconfig;

  ap_log_perror(APLOG_MARK, APLOG_DEBUG, 0, cmd->pol, 
                "%s %s", server, arg);

  return NULL
}

/* client function to return a memcache object */
MEMCACHE_DECLARE_NONSTD(apr_memcache_t *) ap_memcache_client(server_rec *s)
{

  return NULL;
}

static const command_rec memcache_cmds[] = {
  AP_INIT_ITERATE2("MemcacheServer", memcache_srvr, NULL, RSRC_CONF,
                   "memcached host, port, and other options"),
  {NULL}
};

static void memcache_hooks(apr_pool_t *pool)
{
  ap_hook_child_init((void *)memcache_setup_init, NULL, NULL, APR_HOOK_MIDDLE);
  APR_REGISTER_OPTION_FN(ap_memcache_client);
  ap_hook_pre_config(memcache_preconfig, NULL, NULL, APR_HOOK_MIDDLE);
  ap_hook_post_config(memcache_postconfig, NULL, NULL, APR_HOOK_MIDDLE);
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
