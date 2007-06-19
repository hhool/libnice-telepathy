/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2006, 2007 Collabora Ltd.
 *  Contact: Dafydd Harries
 * (C) 2006, 2007 Nokia Corporation. All rights reserved.
 *  Contact: Kai Vehmanen
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Nice GLib ICE library.
 *
 * The Initial Developers of the Original Code are Collabora Ltd and Nokia
 * Corporation. All Rights Reserved.
 *
 * Contributors:
 *   Dafydd Harries, Collabora Ltd.
 *
 * Alternatively, the contents of this file may be used under the terms of the
 * the GNU Lesser General Public License Version 2.1 (the "LGPL"), in which
 * case the provisions of LGPL are applicable instead of those above. If you
 * wish to allow use of your version of this file only under the terms of the
 * LGPL and not to allow others to use your version of this file under the
 * MPL, indicate your decision by deleting the provisions above and replace
 * them with the notice and other provisions required by the LGPL. If you do
 * not delete the provisions above, a recipient may use your version of this
 * file under either the MPL or the LGPL.
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>

#include "udp-bsd.h"

int
main (void)
{
  NiceUDPSocketFactory factory;
  NiceUDPSocket server;
  NiceUDPSocket client;
  NiceAddress tmp;
  gchar buf[5];

  memset (&tmp, 0, sizeof (tmp));
  nice_udp_bsd_socket_factory_init (&factory);

  g_assert (nice_udp_socket_factory_make (&factory, &server, NULL));
  // not bound to a particular interface
  g_assert (server.addr.addr.addr_ipv4 == 0);
  // is bound to a particular port
  g_assert (server.addr.port != 0);

  g_assert (nice_udp_socket_factory_make (&factory, &client, NULL));
  // not bound to a particular interface
  g_assert (client.addr.addr.addr_ipv4 == 0);
  // is bound to a particular port
  g_assert (client.addr.port != 0);

  nice_udp_socket_send (&client, &server.addr, 5, "hello");
  g_assert (5 == nice_udp_socket_recv (&server, &tmp, 5, buf));
  g_assert (0 == strncmp (buf, "hello", 5));
  g_assert (tmp.port == client.addr.port);

  nice_udp_socket_send (&server, &client.addr, 5, "uryyb");
  g_assert (5 == nice_udp_socket_recv (&client, &tmp, 5, buf));
  g_assert (0 == strncmp (buf, "uryyb", 5));
  g_assert (tmp.port == server.addr.port);

  nice_udp_socket_close (&client);
  nice_udp_socket_close (&server);
  nice_udp_socket_factory_close (&factory);
  return 0;
}

