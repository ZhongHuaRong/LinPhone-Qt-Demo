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

#ifndef _LINPHONE_CHAT_ROOM_PARAMS_HH
#define _LINPHONE_CHAT_ROOM_PARAMS_HH

#include <string>
#include "enums.hh"
#include "object.hh"




struct _LinphoneChatRoomParams;


namespace linphone {


	/**
	 * @brief Object defining parameters for a @ref ChatRoom. 
	 *
	 * Can be created with Core::createDefaultChatRoomParams(). You can use isValid()
	 * to check if your configuration is valid or not.
	 * If the @ref ChatRoom backend is @ref ChatRoomBackend::Basic, then no other
	 * parameter is required, but @ref ChatMessage sent and received won't benefit
	 * from all features a @ref ChatRoomBackend::FlexisipChat can offer like
	 * conversation with multiple participants and a subject, end-to-end encryption,
	 * ephemeral messages, etc... but this type is the only one that can interoperate
	 * with other SIP clients or with non-flexisip SIP proxies. 
	 */
	class ChatRoomParams: public Object {
	
		public:

			ChatRoomParams(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneChatRoomParams *cPtr() {return (_LinphoneChatRoomParams *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Get the backend implementation of the chat room associated with the
			 * given parameters. 
			 *
			 * @return the @ref ChatRoomBackend 
			 */
			LINPHONECXX_PUBLIC linphone::ChatRoomBackend getBackend() const;
			
			/**
			 * @brief Set the backend implementation of these chat room parameters. 
			 *
			 * @param backend The @ref ChatRoomBackend enum value 
			 */
			LINPHONECXX_PUBLIC void setBackend(linphone::ChatRoomBackend backend);
			
			/**
			 * @brief Get the encryption implementation of the chat room associated with the
			 * given parameters. 
			 *
			 * @return the @ref ChatRoomEncryptionBackend 
			 */
			LINPHONECXX_PUBLIC linphone::ChatRoomEncryptionBackend getEncryptionBackend() const;
			
			/**
			 * @brief Set the encryption backend implementation of these chat room parameters. 
			 *
			 * @param backend The @ref ChatRoomEncryptionBackend enum value 
			 */
			LINPHONECXX_PUBLIC void setEncryptionBackend(linphone::ChatRoomEncryptionBackend backend);
			
			/**
			 * @brief Get the encryption status of the chat room associated with the given
			 * parameters. 
			 *
			 * @return true if encryption is enabled, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool encryptionEnabled() const;
			
			/**
			 * @brief Enables or disables encryption for the chat room associated with the
			 * given parameters. 
			 *
			 */
			LINPHONECXX_PUBLIC void enableEncryption(bool encrypted);
			
			/**
			 * @brief Get the group chat status of the chat room associated with the given
			 * parameters. 
			 *
			 * @return true if group chat is enabled, false if one-to-one 
			 */
			LINPHONECXX_PUBLIC bool groupEnabled() const;
			
			/**
			 * @brief Enables or disables group chat for the chat room associated with the
			 * given parameters. 
			 *
			 */
			LINPHONECXX_PUBLIC void enableGroup(bool group);
			
			/**
			 * @brief Returns whether the given parameters are valid or not. 
			 *
			 * @return true if the given parameters are valid, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isValid() const;
			
			/**
			 * @brief Get the real time text status of the chat room associated with the given
			 * parameters. 
			 *
			 * @return true if real time text is enabled, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool rttEnabled() const;
			
			/**
			 * @brief Enables or disables real time text for the chat room associated with the
			 * given parameters. 
			 *
			 */
			LINPHONECXX_PUBLIC void enableRtt(bool rtt);
			
			/**
			 * @brief Get the subject of the chat room. 
			 *
			 * @return The subject.   
			 */
			LINPHONECXX_PUBLIC std::string getSubject();
			
			/**
			 * @brief Set the subject of the chat room. 
			 *
			 * @param subject The subject to set.   
			 */
			LINPHONECXX_PUBLIC void setSubject(const std::string & subject);
			
			
	};

};

#endif // _LINPHONE_CHAT_ROOM_PARAMS_HH
