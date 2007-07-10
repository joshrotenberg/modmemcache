/* Copyright 2007 Josh Rotenberg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "mod_memcache.h"
#include "apr_memcache.h"
#include "util_time.h"

/* The sample content handler */

static int set = 0;
static int memcache_example_handler(request_rec *r)
{
  apr_memcache_t *mc;
  char *result;
  apr_size_t len;
  apr_uint16_t flags;
  char date[APR_RFC822_DATE_LEN];

  ap_recent_rfc822_date(date, apr_time_now());
  mc = ap_memcache_client(r->server);
  apr_memcache_set(mc, "date", date, strlen(date), 0, 0);
  apr_memcache_set(mc, "int", apr_itoa(r->pool, set), 
                   strlen(apr_itoa(r->pool, set)), 0, 0);

  if (strcmp(r->handler, "memcache_example")) {
    return DECLINED;
  }
  r->content_type = "text/html";      

  if (!r->header_only) {
    apr_hash_t *srvrs = ap_memcache_serverhash(r->server);
    apr_hash_index_t *hi;
    void *val;
    apr_memcache_server_t *this;

    for (hi = apr_hash_first(r->pool, srvrs);
         hi; 
         hi = apr_hash_next(hi)) {
      apr_hash_this(hi, NULL, NULL, &val);

      this = (apr_memcache_server_t *)val;
      if(apr_memcache_find_server(mc, this->host, this->port) != NULL) {
         ap_rprintf(r, "server: %s:%d<br>\n", this->host, this->port);
      }
    }
    apr_memcache_getp(mc, r->pool, "date", &result, &len, &flags);
	
    ap_rprintf(r, "set/retrieved the following key/value: date=%s<br>\n", 
               result);

    apr_memcache_getp(mc, r->pool, "int", &result, &len, &flags);

    ap_rprintf(r, "set/retrieved the following key/value: int=%s<br>\n", 
               result);
    set++;
    ap_rprintf(r, "next time it will probably be %d\n", set);
  }

  return OK;
}

static void memcache_example_register_hooks(apr_pool_t *p)
{
  ap_hook_handler(memcache_example_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA memcache_example_module = {
  STANDARD20_MODULE_STUFF, 
  NULL,                  /* create per-dir    config structures */
  NULL,                  /* merge  per-dir    config structures */
  NULL,                  /* create per-server config structures */
  NULL,                  /* merge  per-server config structures */
  NULL,                  /* table of config file commands       */
  memcache_example_register_hooks  /* register hooks                      */
};

