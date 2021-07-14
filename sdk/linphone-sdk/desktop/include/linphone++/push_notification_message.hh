/*
Copyright (C) 2017 Belledonne Communications SARL

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _LINPHONE_PUSH_NOTIFICATION_MESSAGE_HH
#define _LINPHONE_PUSH_NOTIFICATION_MESSAGE_HH

#include "object.hh"




struct _LinphonePushNotificationMessage;


namespace linphone {


	/**
	 * @brief Object holding chat message data received by a push notification on iOS
	 * platform only. 
	 *
	 * This object is a subset of @ref ChatMessage, so only a few methods of it's
	 * parent are available, like linphone_push_notification_message_get_text_content
	 * and linphone_push_notification_message_get_subject, just enough to be able to
	 * build a notification to show the user. 
	 */
	class PushNotificationMessage: public Object {
	
		public:

			PushNotificationMessage(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphonePushNotificationMessage *cPtr() {return (_LinphonePushNotificationMessage *)mPrivPtr;}

	
		public:
			
		
			
	};

};

#endif // _LINPHONE_PUSH_NOTIFICATION_MESSAGE_HH
