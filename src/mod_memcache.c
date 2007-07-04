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

module AP_MODULE_DECLARE_DATA memcache_module;

typedef struct memcache_cfg {
  apr_memcache_t *mc;
  apr_array_header_t *servers;
} memcache_cfg;

typedef struct memcache_server {
  const char *host;
  apr_port_t port;
  apr_uint32_t min;
  apr_uint32_t max;
  apr_uint32_t smax;
  apr_uint32_t ttl;
  struct memcache_server *next;
} memcache_server;

/* config create */
static void *memcache_create_cfg(apr_pool_t *p, server_rec *s)
{
  memcache_cfg *cfg = (memcache_cfg *) apr_pcalloc(p, sizeof(memcache_cfg));

  cfg->servers = apr_array_make(p, 5, sizeof(apr_memcache_server_t));
  return cfg;
}

/* config merge */
static void *memcache_merge(apr_pool_t *pool, void *b, void *a)
{
  memcache_cfg *base = (memcache_cfg *) base;
  memcache_cfg *add = (memcache_cfg *) add;
  memcache_cfg *cfg = apr_pcalloc(pool, sizeof(memcache_cfg));

  return (void *) cfg;
}

/* post-config */
static int memcache_postconfig(apr_pool_t *pconf, apr_pool_t *plog,
                               apr_pool_t *ptemp, server_rec *s)
{

  server_rec *sp;
  memcache_cfg *cfg;

  for(sp = s; sp; sp = sp->next) {
    apr_status_t rv;
    apr_memcache_server_t *ms;
    int i;

    cfg = (memcache_cfg *)ap_get_module_config(sp->module_config, 
                                               &memcache_module);

    if(cfg->servers == NULL || !cfg->servers->nelts) {
      continue;
    }

    rv = apr_memcache_create(pconf, 10, 0, &cfg->mc);

    if(rv != APR_SUCCESS) {
      ap_log_error(APLOG_MARK, APLOG_DEBUG, rv, sp, "no");
      return rv;
    }

    ms = (apr_memcache_server_t *) cfg->servers->elts;
    for(i = 0; i < cfg->servers->nelts; i++) {

      rv = apr_memcache_add_server(cfg->mc, &ms[i]);
      if(rv != APR_SUCCESS) {
        ap_log_error(APLOG_MARK, APLOG_DEBUG, rv, sp, "no");
        return rv;
      }
    }
  }
  
  return OK;
}

/* parse config options */
static const char *memcache_srvr (cmd_parms *cmd, void *doof, int argc,
                                  char *const argv[])
{
  int i;
  char *w;
  apr_status_t rv;
  apr_memcache_server_t *ms;
  char *server = NULL;
  char *port = NULL;
  apr_uint32_t min =0 ;
  apr_uint32_t max = 0;
  apr_uint32_t smax = 0;
  apr_uint32_t ttl = 0;
  memcache_cfg *cfg = 
    (memcache_cfg *)ap_get_module_config(cmd->server->module_config, 
                                         &memcache_module);

  for(i = 0; i < argc; i++) {
    w = argv[i];

    if(!strncasecmp(w, "Min=", 4)) {
      min = atoi(&w[4]);
    }
    else if(!strncasecmp(w, "Max=", 4)) {
      max = atoi(&w[4]);
    }
    else if(!strncasecmp(w, "Ttl=", 4)) {
      ttl = atoi(&w[4]);
    }
    else if(!strncasecmp(w, "Smax=", 5)) {
      smax = atoi(&w[5]);
    }
    else {
      server = apr_pstrdup(cmd->pool, w);
      port = strchr(server, ':');
      
      if(port) {
        *(port++) = '\0';
      }
      
      if(port == NULL || server == NULL) {
        return "Server must be in the format <host>:<port>";
      }
    }
  }

  ms = apr_array_push(cfg->servers);  

  if(ms == NULL) {
    return "Unable to allocate new memcache server";
  }

  rv = 
    apr_memcache_server_create(cmd->pool, server, atoi(port), 
                               min, smax, max, ttl, &ms);

  if(rv != APR_SUCCESS) {
    return "Unable to connect to server";
  }

  return NULL;
}

/* client function to return a memcache object */
MEMCACHE_DECLARE_NONSTD(apr_memcache_t *) ap_memcache_client(server_rec *s)
{
  memcache_cfg *cfg = (memcache_cfg *)ap_get_module_config(s->module_config,
                                                           &memcache_module);
  if(cfg->mc != NULL) {
    return cfg->mc;
  }
  
  return NULL;
}

/* client function to return an array of  memcache serverst */
MEMCACHE_DECLARE_NONSTD(apr_array_header_t *) ap_memcache_serverlist(server_rec *s)
{
  memcache_cfg *cfg = (memcache_cfg *)ap_get_module_config(s->module_config,
                                                           &memcache_module);
  if(cfg->servers != NULL) {
    return cfg->servers;
  }
  
  return NULL;
}

static const command_rec memcache_cmds[] = {
  AP_INIT_TAKE_ARGV("MemcacheServer", memcache_srvr, NULL, RSRC_CONF,
                   "memcached host, port, and other options"),
  {NULL}
};

static void memcache_hooks(apr_pool_t *pool)
{

  APR_REGISTER_OPTIONAL_FN(ap_memcache_client);
  APR_REGISTER_OPTIONAL_FN(ap_memcache_serverlist);

  ap_hook_post_config(memcache_postconfig, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA memcache_module = {
  STANDARD20_MODULE_STUFF,
  NULL,
  NULL,
  memcache_create_cfg,
  memcache_merge,
  memcache_cmds,
  memcache_hooks
};
