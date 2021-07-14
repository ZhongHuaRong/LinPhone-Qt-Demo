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

#ifndef _LINPHONE_CHAT_ROOM_HH
#define _LINPHONE_CHAT_ROOM_HH

#include <cstdint>
#include <list>
#include <string>
#include "enums.hh"
#include "chat_room.hh"
#include "object.hh"




struct _LinphoneChatRoom;


namespace linphone {

	class Call;
	class Address;
	class Core;
	class ChatRoomParams;
	class ChatMessage;
	class Participant;
	class Content;
	class EventLog;
	class ParticipantDeviceIdentity;
	class ChatRoomListener;

	/**
	 * @brief A chat room is the place where @ref ChatMessage are exchanged. 
	 *
	 * To create (or find) a @ref ChatRoom, you first need a @ref ChatRoomParams
	 * object. A chat room is uniquely identified by it's local and remote SIP
	 * addresses, meaning you can only have one chat room between two accounts (unless
	 * the backend is @ref ChatRoomBackend::FlexisipChat). Then you can call
	 * Core::searchChatRoom() or Core::createChatRoom().
	 * Be careful as a @ref ChatRoomBackend::FlexisipChat backend @ref ChatRoom will
	 * be created asynchronously, so make sure you add a LinphoneChatRoomCbs to the
	 * returned object to be notified when it will be in state @ref State::Created.
	 * All chat rooms are loaded from database when the @ref Core starts, and you can
	 * get them using Core::getChatRooms(). This method doesn't return empty chat
	 * rooms nor ones for which the local address doesn't match an existing @ref
	 * ProxyConfig identity, unless you specify otherwise in the [misc] section of
	 * your configuration file by setting hide_empty_chat_rooms=0 and/or
	 * hide_chat_rooms_from_removed_proxies=0. 
	 */
	class ChatRoom: public MultiListenableObject {
	
		public:
			/**
			* @ref State is used to indicate the current state of a chat room. 
			*
			*/
			enum class State {
				/**
				* Initial state. 
				*/
				None,
				/**
				* Chat room is now instantiated on local. 
				*/
				Instantiated,
				/**
				* One creation request was sent to the server. 
				*/
				CreationPending,
				/**
				* Chat room was created on the server. 
				*/
				Created,
				/**
				* Chat room creation failed. 
				*/
				CreationFailed,
				/**
				* Wait for chat room termination. 
				*/
				TerminationPending,
				/**
				* Chat room exists on server but not in local. 
				*/
				Terminated,
				/**
				* The chat room termination failed. 
				*/
				TerminationFailed,
				/**
				* Chat room was deleted on the server. 
				*/
				Deleted
			};


			ChatRoom(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneChatRoom *cPtr() {return (_LinphoneChatRoom *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<ChatRoomListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<ChatRoomListener> &listener);
	
		public:
			
		
			/**
			 * @brief Gets the current call associated to this chatroom if any To commit a
			 * message, use linphone_chat_room_send_message 
			 *
			 * @return @ref Call or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getCall() const;
			
			/**
			 * @brief Get the capabilities of a chat room. 
			 *
			 * @return The capabilities of the chat room (as a
			 * LinphoneChatRoomCapabilitiesMask) 
			 */
			LINPHONECXX_PUBLIC unsigned int getCapabilities() const;
			
			/**
			 * @brief When realtime text is enabled CallParams::realtimeTextEnabled(),
			 * LinphoneCoreIsComposingReceivedCb is call everytime a char is received from
			 * peer. 
			 *
			 * At the end of remote typing a regular @ref ChatMessage is received with
			 * committed data from LinphoneCoreMessageReceivedCb. 
			 * @return RFC 4103/T.140 char 
			 */
			LINPHONECXX_PUBLIC uint32_t getChar();
			
			/**
			 * @brief Gets the list of participants that are currently composing. 
			 *
			 * @return List of addresses that are in the is_composing state.  
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Address>> getComposingAddresses();
			
			/**
			 * @brief Get the conference address of the chat room. 
			 *
			 * @return The conference address of the chat room or nullptr if this type of chat
			 * room is not conference based.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getConferenceAddress() const;
			
			/**
			 * @brief Set the conference address of a group chat room. 
			 *
			 * This function needs to be called from the
			 * LinphoneChatRoomCbsConferenceAddressGenerationCb callback and only there. This
			 * function is meaningful only for server implementation of chatroom, and shall
			 * not by used by client applications. 
			 * @param conferenceAddress The conference @ref Address to be used by the group
			 * chat room   
			 */
			LINPHONECXX_PUBLIC void setConferenceAddress(const std::shared_ptr<const linphone::Address> & conferenceAddress);
			
			/**
			 * @brief Returns back pointer to @ref Core object. 
			 *
			 * @return the @ref Core object this chat room is attached to.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> getCore() const;
			
			/**
			 * @brief Returns current parameters associated with the chat room. 
			 *
			 * This is typically the parameters passed at chat room chat_roomeation to
			 * linphone_core_chat_roomeate_chat_room() or some default parameters if no @ref
			 * ChatRoomParams was explicitely passed during chat room chat_roomeation. 
			 * @return the current @ref ChatRoomParams parameters.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::ChatRoomParams> getCurrentParams() const;
			
			/**
			 * @brief Returns whether or not the ephemeral message feature is enabled in the
			 * chat room. 
			 *
			 * @return true if ephemeral is enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool ephemeralEnabled() const;
			
			/**
			 * @brief Enable or disable the ephemeral message feature in the chat room. 
			 *
			 * Works only for conference based chat room. An ephemeral message will
			 * automatically disappear from the recipient's schat_roomeen after the message
			 * has been viewed. 
			 * @param enable true if the ephemeral message feature is enabled, false
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC void enableEphemeral(bool enable);
			
			/**
			 * @brief Get lifetime (in seconds) for all new ephemeral messages in the chat
			 * room. 
			 *
			 * After the message is read, it will be deleted after "time" seconds. @see
			 * ephemeralEnabled() 
			 * @return the ephemeral lifetime (in secoonds) 
			 */
			LINPHONECXX_PUBLIC long getEphemeralLifetime() const;
			
			/**
			 * @brief Set lifetime (in seconds) for all new ephemral messages in the chat
			 * room. 
			 *
			 * After the message is read, it will be deleted after "time" seconds. @see
			 * ephemeralEnabled() 
			 * @param time The ephemral lifetime, default 24h, 86400s 
			 */
			LINPHONECXX_PUBLIC void setEphemeralLifetime(long time);
			
			/**
			 * @brief Gets the number of events in a chat room. 
			 *
			 * @return the number of events. 
			 */
			LINPHONECXX_PUBLIC int getHistoryEventsSize();
			
			/**
			 * @brief Gets the number of messages in a chat room. 
			 *
			 * @return the number of messages. 
			 */
			LINPHONECXX_PUBLIC int getHistorySize();
			
			/**
			 * @brief Returns whether or not a @ref ChatRoom has at least one @ref ChatMessage
			 * or not. 
			 *
			 * @return true if there are no @ref ChatMessage, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isEmpty();
			
			/**
			 * @brief Tells whether the remote is currently composing a message. 
			 *
			 * @return true if the remote is currently composing a message, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isRemoteComposing() const;
			
			/**
			 * @brief Gets the last chat message sent or received in this chat room. 
			 *
			 * @return the latest @ref ChatMessage or nullptr if no message.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> getLastMessageInHistory();
			
			/**
			 * @brief Return the last updated time for the chat room. 
			 *
			 * @return the last updated time 
			 */
			LINPHONECXX_PUBLIC time_t getLastUpdateTime() const;
			
			/**
			 * @brief Get the local address associated to this chat room. 
			 *
			 * @return The local address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getLocalAddress();
			
			/**
			 * @brief Get the participant representing myself in the chat room. 
			 *
			 * @return The participant representing myself in the conference or nullptr if me
			 * is not set.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Participant> getMe() const;
			
			/**
			 * @brief Get the number of participants in the chat room (that is without
			 * ourselves). 
			 *
			 * @return The number of participants in the chat room 
			 */
			LINPHONECXX_PUBLIC int getNbParticipants() const;
			
			/**
			 * @brief Get the list of participants of a chat room. 
			 *
			 * @return A  of the participants 
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Participant>> getParticipants() const;
			
			/**
			 * @brief Get the peer address associated to this chat room. 
			 *
			 * @return The peer address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getPeerAddress();
			
			/**
			 * @brief Get the security level of a chat room. 
			 *
			 * @return The current @ref ChatRoomSecurityLevel of the chat room 
			 */
			LINPHONECXX_PUBLIC linphone::ChatRoomSecurityLevel getSecurityLevel();
			
			/**
			 * @brief Get the state of the chat room. 
			 *
			 * @return The current @ref State of the chat room 
			 */
			LINPHONECXX_PUBLIC State getState() const;
			
			/**
			 * @brief Get the subject of a chat room. 
			 *
			 * @return The subject of the chat room.   
			 */
			LINPHONECXX_PUBLIC std::string getSubject() const;
			
			/**
			 * @brief Set the subject of a chat room. 
			 *
			 * @param subject The new subject to set for the chat room   
			 */
			LINPHONECXX_PUBLIC void setSubject(const std::string & subject);
			
			/**
			 * @brief Gets the number of unread messages in the chatroom. 
			 *
			 * @return the number of unread messages. 
			 */
			LINPHONECXX_PUBLIC int getUnreadMessagesCount();
			
			/**
			 * @brief Add a participant to a chat room. 
			 *
			 * This may fail if this type of chat room does not handle participants. Use
			 * canHandleParticipants() to know if this chat room handles participants. 
			 * @param addr The address of the participant to add to the chat room   
			 */
			LINPHONECXX_PUBLIC void addParticipant(const std::shared_ptr<const linphone::Address> & addr);
			
			/**
			 * @brief Add several participants to a chat room at once. 
			 *
			 * This may fail if this type of chat room does not handle participants. Use
			 * canHandleParticipants() to know if this chat room handles participants. 
			 * @param addresses The participants to add.    
			 * @return true if everything is OK, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool addParticipants(const std::list<std::shared_ptr<linphone::Address>> & addresses);
			
			/**
			 * @brief Allow cpim on a basic chat room   . 
			 *
			 */
			LINPHONECXX_PUBLIC void allowCpim();
			
			/**
			 * @brief Allow multipart on a basic chat room   . 
			 *
			 */
			LINPHONECXX_PUBLIC void allowMultipart();
			
			/**
			 * @brief Tells whether a chat room is able to handle participants. 
			 *
			 * @return true if the chat room can handle participants, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool canHandleParticipants() const;
			
			/**
			 * @brief Notifies the destination of the chat message being composed that the
			 * user is typing a new message. 
			 *
			 */
			LINPHONECXX_PUBLIC void compose();
			
			/**
			 * @brief Creates an empty message attached to the given chat room. 
			 *
			 * @return a new @ref ChatMessage   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> createEmptyMessage();
			
			/**
			 * @brief Creates a message attached to the given chat room with a particular
			 * content. 
			 *
			 * Use linphone_chat_room_send_message to initiate the transfer 
			 * @param initialContent @ref Content initial content.   
			 * @return a new @ref ChatMessage   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> createFileTransferMessage(const std::shared_ptr<linphone::Content> & initialContent);
			
			/**
			 * @brief Creates a forward message attached to the given chat room with a
			 * particular message. 
			 *
			 * @param message @ref ChatMessage message to be forwarded.   
			 * @return a new @ref ChatMessage   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> createForwardMessage(const std::shared_ptr<linphone::ChatMessage> & message);
			
			/**
			 * @brief Creates a message attached to the given chat room with a plain text
			 * content filled with the given message. 
			 *
			 * @param message text message, nullptr if absent.   
			 * @return a new @ref ChatMessage   
			 * @deprecated 01/07/2020. Use createMessageFromUtf8() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatMessage> createMessage(const std::string & message);
			
			/**
			 * @brief Creates a message attached to the given chat room with a plain text
			 * content filled with the given message. 
			 *
			 * Introduced in 01/07/2020 
			 * @param message text message in UTF8, nullptr if absent.   
			 * @return a new @ref ChatMessage   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> createMessageFromUtf8(const std::string & message);
			
			/**
			 * @brief Delete all messages from the history. 
			 *
			 */
			LINPHONECXX_PUBLIC void deleteHistory();
			
			/**
			 * @brief Delete a message from the chat room history. 
			 *
			 * @param message The @ref ChatMessage object to remove.   
			 */
			LINPHONECXX_PUBLIC void deleteMessage(const std::shared_ptr<linphone::ChatMessage> & message);
			
			/**
			 * @brief Uses linphone spec to check if all participants support ephemeral
			 * messages. 
			 *
			 * It doesn't prevent to send ephemeral messages in the room but those who don't
			 * support it won't delete messages after lifetime has expired. @see
			 * ephemeralEnabled() 
			 * @return true if all participants in the chat room support ephemeral messages,
			 * false otherwise 
			 */
			LINPHONECXX_PUBLIC bool ephemeralSupportedByAllParticipants() const;
			
			/**
			 * @brief Gets the chat message sent or received in this chat room that matches
			 * the message_id. 
			 *
			 * @param messageId The id of the message to find   
			 * @return the @ref ChatMessage if found or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatMessage> findMessage(const std::string & messageId);
			
			/**
			 * @brief Find a participant of a chat room from its address. 
			 *
			 * @param address The @ref Address to search in the list of participants of the
			 * chat room   
			 * @return The participant if found, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Participant> findParticipant(const std::shared_ptr<const linphone::Address> & address) const;
			
			/**
			 * @brief Gets nb_message most recent messages from chat_room chat room, sorted
			 * from oldest to most recent. 
			 *
			 * @param nbMessage Number of message to retrieve. 0 means everything. 
			 * @return A list of chat messages.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::ChatMessage>> getHistory(int nbMessage);
			
			/**
			 * @brief Gets nb_events most recent events from chat_room chat room, sorted from
			 * oldest to most recent. 
			 *
			 * @param nbEvents Number of events to retrieve. 0 means everything. 
			 * @return The list of the most recent events.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::EventLog>> getHistoryEvents(int nbEvents);
			
			/**
			 * @brief Gets nb_events most recent chat message events from chat_room chat room,
			 * sorted from oldest to most recent. 
			 *
			 * @param nbEvents Number of events to retrieve. 0 means everything. 
			 * @return A list    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::EventLog>> getHistoryMessageEvents(int nbEvents);
			
			/**
			 * @brief Gets the partial list of messages in the given range, sorted from oldest
			 * to most recent. 
			 *
			 * @param end The last message of the range to be retrieved. History oldest
			 * message has index of history size - 1 (use getHistorySize() to retrieve history
			 * size) 
			 * @return A list of chat messages.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::ChatMessage>> getHistoryRange(int begin, int end);
			
			/**
			 * @brief Gets the partial list of events in the given range, sorted from oldest
			 * to most recent. 
			 *
			 * @param end The last event of the range to be retrieved. History oldest event
			 * has index of history size - 1 
			 * @return The list of the found events.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::EventLog>> getHistoryRangeEvents(int begin, int end);
			
			/**
			 * @brief Gets the partial list of chat message events in the given range, sorted
			 * from oldest to most recent. 
			 *
			 * @param end The last event of the range to be retrieved. History oldest event
			 * has index of history size - 1 
			 * @return The list of chat message events.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::EventLog>> getHistoryRangeMessageEvents(int begin, int end);
			
			/**
			 * @brief Return whether or not the chat room has been left. 
			 *
			 * @return true if the chat room has been left, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool hasBeenLeft() const;
			
			/**
			 * @brief Check if a chat room has given capabilities. 
			 *
			 * @param mask a LinphoneChatRoomCapabilitiesMask mask 
			 * @return true if the mask matches, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool hasCapability(int mask) const;
			
			/**
			 * @brief Leave a chat room. 
			 *
			 */
			LINPHONECXX_PUBLIC void leave();
			
			/**
			 * @brief Returns wether lime is available for given peer or not. 
			 *
			 */
			LINPHONECXX_PUBLIC bool limeAvailable();
			
			/**
			 * @brief Mark all messages of the conversation as read. 
			 *
			 */
			LINPHONECXX_PUBLIC void markAsRead();
			
			/**
			 * @brief Notify the chatroom that a participant device has just registered. 
			 *
			 * This function is meaningful only for server implementation of chatroom, and
			 * shall not by used by client applications. 
			 * @param participantDevice list of the participant devices to be used by the
			 * group chat room   
			 */
			LINPHONECXX_PUBLIC void notifyParticipantDeviceRegistration(const std::shared_ptr<const linphone::Address> & participantDevice);
			
			/**
			 * @brief Used to receive a chat message when using async mechanism with IM
			 * enchat_roomyption engine. 
			 *
			 * @param message @ref ChatMessage object   
			 */
			LINPHONECXX_PUBLIC void receiveChatMessage(const std::shared_ptr<linphone::ChatMessage> & message);
			
			/**
			 * @brief Remove a participant of a chat room. 
			 *
			 * @param participant The participant to remove from the chat room   
			 */
			LINPHONECXX_PUBLIC void removeParticipant(const std::shared_ptr<linphone::Participant> & participant);
			
			/**
			 * @brief Remove several participants of a chat room at once. 
			 *
			 * @param participants The participants to remove.    
			 */
			LINPHONECXX_PUBLIC void removeParticipants(const std::list<std::shared_ptr<linphone::Participant>> & participants);
			
			/**
			 * @brief Change the admin status of a participant of a chat room (you need to be
			 * an admin yourself to do this). 
			 *
			 * @param isAdmin A boolean value telling whether the participant should now be an
			 * admin or not 
			 */
			LINPHONECXX_PUBLIC void setParticipantAdminStatus(const std::shared_ptr<linphone::Participant> & participant, bool isAdmin);
			
			/**
			 * @brief Set the list of participant devices in the form of SIP URIs with GRUUs
			 * for a given participant. 
			 *
			 * This function is meaningful only for server implementation of chatroom, and
			 * shall not by used by client applications. 
			 * @param deviceIdentities List of the participant devices to be used by the group
			 * chat room    
			 */
			LINPHONECXX_PUBLIC void setParticipantDevices(const std::shared_ptr<const linphone::Address> & participantAddress, const std::list<std::shared_ptr<linphone::ParticipantDeviceIdentity>> & deviceIdentities);
			
			
		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_CHAT_ROOM_HH
