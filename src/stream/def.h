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

#ifndef STREAM_DEF_H
# define STREAM_DEF_H

#if defined(_WIN32) || defined(__CYGWIN__)
# define MSG_MORE 0x8000
#endif

#include <stdint.h>

struct stream_hdr
{
    uint32_t magic;

    uint32_t len;

    /**
     * 0 = No XOR
     * 1 = XOR
     * 2 = send UID
     * 3 = XOR key
     * 4 = ACK
     */
    uint32_t xor;
};

#endif /* STREAM_DEF_H */
