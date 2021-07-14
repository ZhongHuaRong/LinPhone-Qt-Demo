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

#ifndef _LINPHONE_EVENT_LOG_HH
#define _LINPHONE_EVENT_LOG_HH

#include <string>
#include "event_log.hh"
#include "object.hh"




struct _LinphoneEventLog;


namespace linphone {

	class Call;
	class ChatMessage;
	class Address;

	/**
	 * @brief Object that represents an event that must be stored in database. 
	 *
	 * For example, all chat related events are wrapped in an @ref EventLog, and many
	 * callbacks use this kind of type as parameter.
	 * Use getType() to get the @ref Type it refers to, and then you can use one of
	 * the accessor methods to get the underlying object, for example getChatMessage()
	 * for a @ref ChatMessage. 
	 */
	class EventLog: public Object {
	
		public:
			/**
			* @ref SecurityEventType is used to indicate the type of security event. 
			*
			*/
			enum class SecurityEventType {
				/**
				* Event is not a security event. 
				*/
				None,
				/**
				* Chatroom security level downgraded event. 
				*/
				SecurityLevelDowngraded,
				/**
				* Participant has exceeded the maximum number of device event. 
				*/
				ParticipantMaxDeviceCountExceeded,
				/**
				* Peer device instant messaging encryption identity key has changed event. 
				*/
				EncryptionIdentityKeyChanged,
				/**
				* Man in the middle detected event. 
				*/
				ManInTheMiddleDetected
			};

			/**
			* @ref Type is used to indicate the type of an event. 
			*
			*/
			enum class Type {
				/**
				* No defined event. 
				*/
				None,
				/**
				* Conference (created) event. 
				*/
				ConferenceCreated,
				/**
				* Conference (terminated) event. 
				*/
				ConferenceTerminated,
				/**
				* Conference call (start) event. 
				*/
				ConferenceCallStart,
				/**
				* Conference call (end) event. 
				*/
				ConferenceCallEnd,
				/**
				* Conference chat message event. 
				*/
				ConferenceChatMessage,
				/**
				* Conference participant (added) event. 
				*/
				ConferenceParticipantAdded,
				/**
				* Conference participant (removed) event. 
				*/
				ConferenceParticipantRemoved,
				/**
				* Conference participant (set admin) event. 
				*/
				ConferenceParticipantSetAdmin,
				/**
				* Conference participant (unset admin) event. 
				*/
				ConferenceParticipantUnsetAdmin,
				/**
				* Conference participant device (added) event. 
				*/
				ConferenceParticipantDeviceAdded,
				/**
				* Conference participant device (removed) event. 
				*/
				ConferenceParticipantDeviceRemoved,
				/**
				* Conference subject event. 
				*/
				ConferenceSubjectChanged,
				/**
				* Conference encryption security event. 
				*/
				ConferenceSecurityEvent,
				/**
				* Conference ephemeral message (ephemeral message lifetime changed) event. 
				*/
				ConferenceEphemeralMessageLifetimeChanged,
				/**
				* Conference ephemeral message (ephemeral message enabled) event. 
				*/
				ConferenceEphemeralMessageEnabled,
				/**
				* Conference ephemeral message (ephemeral message disabled) event. 
				*/
				ConferenceEphemeralMessageDisabled
			};


			EventLog(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneEventLog *cPtr() {return (_LinphoneEventLog *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Returns the call of a conference call event. 
			 *
			 * @return The conference @ref Call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getCall() const;
			
			/**
			 * @brief Returns the chat message of a conference chat message event. 
			 *
			 * @return The conference @ref ChatMessage.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> getChatMessage() const;
			
			/**
			 * @brief Returns the creation time of a event log. 
			 *
			 * @return The event creation time 
			 */
			LINPHONECXX_PUBLIC time_t getCreationTime() const;
			
			/**
			 * @brief Returns the device address of a conference participant device event. 
			 *
			 * @return The conference device @ref Address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getDeviceAddress() const;
			
			/**
			 * @brief Returns the ephemeral message lifetime of a conference ephemeral message
			 * event. 
			 *
			 * Ephemeral lifetime means the time before an ephemeral message which has been
			 * viewed gets deleted. 
			 * @return The ephemeral message lifetime. 
			 */
			LINPHONECXX_PUBLIC long getEphemeralMessageLifetime() const;
			
			/**
			 * @brief Returns the local address of a conference event. 
			 *
			 * @return The local @ref Address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getLocalAddress() const;
			
			/**
			 * @brief Returns the notify id of a conference notified event. 
			 *
			 * @return The conference notify id. 
			 */
			LINPHONECXX_PUBLIC unsigned int getNotifyId() const;
			
			/**
			 * @brief Returns the participant address of a conference participant event. 
			 *
			 * @return The conference participant @ref Address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getParticipantAddress() const;
			
			/**
			 * @brief Returns the peer address of a conference event. 
			 *
			 * @return The peer @ref Address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getPeerAddress() const;
			
			/**
			 * @brief Returns the faulty device address of a conference security event. 
			 *
			 * @return The @ref Address of the faulty device.     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Address> getSecurityEventFaultyDeviceAddress() const;
			
			/**
			 * @brief Returns the type of security event. 
			 *
			 * @return The @ref SecurityEventType type. 
			 */
			LINPHONECXX_PUBLIC SecurityEventType getSecurityEventType() const;
			
			/**
			 * @brief Returns the subject of a conference subject event. 
			 *
			 * @return The conference subject.   
			 */
			LINPHONECXX_PUBLIC std::string getSubject() const;
			
			/**
			 * @brief Returns the type of a event log. 
			 *
			 * @return The @ref Type type 
			 */
			LINPHONECXX_PUBLIC Type getType() const;
			
			/**
			 * @brief Delete event log from database. 
			 *
			 */
			LINPHONECXX_PUBLIC void deleteFromDatabase();
			
			
	};

};

#endif // _LINPHONE_EVENT_LOG_HH
