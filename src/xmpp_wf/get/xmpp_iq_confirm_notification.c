/**
 * WarfaceBot, a blind XMPP client for Warface (FPS)
 * Copyright (C) 2015 Levak Borok <levak92@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <wb_tools.h>
#include <wb_stream.h>
#include <wb_session.h>
#include <wb_xmpp_wf.h>

#include <stdio.h>

enum e_notif_type
{
    NOTIF_ACHIEVEMENT = 4,
    NOTIF_FRIEND_REQUEST = 64,
    NOTIF_STATUS_UPDATE = 128,
    NOTIF_CONS_LOGIN = 256,
    NOTIF_ANNOUNCEMENT = 512,
};

void xmpp_iq_confirm_notification(const char *notif)
{
    char *notif_id = get_info(notif, "id='", "'", NULL);
    enum e_notif_type notif_type = get_info_int(notif, "type='", "'", NULL);

    switch (notif_type)
    {
        /* Confirm consecutive logins */
        case NOTIF_CONS_LOGIN:
            puts("Getting consecutive reward");
        /* Accept any friend requests */
        case NOTIF_FRIEND_REQUEST:
            send_stream_format(session.wfs,
                               "<iq to='masterserver@warface/%s' type='get'>"
                               " <query xmlns='urn:cryonline:k01'>"
                               "  <confirm_notification>"
                               "   <notif id='%s' type='%d'>"
                               "    <confirmation result='0' status='%d'"
                               "                  location=''/>"
                               "   </notif>"
                               "  </confirm_notification>"
                               " </query>"
                               "</iq>",
                               session.channel, notif_id,
                               notif_type, session.status);
            break;
        /* Old fashion peer_status_update */
        case NOTIF_STATUS_UPDATE:
        {
            char *jid = get_info(notif, "jid='", "'", NULL);
            char *nick = get_info(notif, "nickname='", "'", NULL);
            char *pid = get_info(notif, "profile_id='", "'", NULL);
            int status = get_info_int(notif, "status='", "'", NULL);
            int exp = get_info_int(notif, "experience='", "'", NULL);

            if (status <= STATUS_OFFLINE)
                jid = NULL;

            friend_list_add(jid, nick, pid, status, exp);
            xmpp_iq_peer_status_update(jid);

            free(jid);
            free(nick);
            free(pid);
        }
        break;
        default:
            break;
    }

    free(notif_id);
}
