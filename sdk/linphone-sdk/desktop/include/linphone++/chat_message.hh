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

#ifndef _LINPHONE_CHAT_MESSAGE_HH
#define _LINPHONE_CHAT_MESSAGE_HH

#include <string>
#include <list>
#include <cstdint>
#include "chat_message.hh"
#include "object.hh"




struct _LinphoneChatMessage;


namespace linphone {

	class ChatRoom;
	class Content;
	class Core;
	class ErrorInfo;
	class Address;
	class ParticipantImdnState;
	class ChatMessageListener;

	/**
	 * @brief An chat message is the object that is sent or received through a @ref
	 * ChatRoom. 
	 *
	 * To create a @ref ChatMessage, use ChatRoom::createEmptyMessage(), then either
	 * add text using addUtf8TextContent() or a @ref Content with file informations
	 * using addFileContent(). A valid @ref Content for file transfer must contain a
	 * type and subtype, the name of the file and it's size. Finally call send() to
	 * send it.
	 * To send files through a @ref ChatMessage, you need to have configured a file
	 * transfer server URL with Core::setFileTransferServer(). On the receiving side,
	 * either use downloadContent() to download received files or enable auto-download
	 * in the @ref Core using Core::setMaxSizeForAutoDownloadIncomingFiles(), -1
	 * disabling the feature and 0 always downloading files no matter it's size.
	 * Keep in mind a @ref ChatMessage created by a @ref ChatRoomBackend::Basic @ref
	 * ChatRoom can only contain one @ref Content, either text or file. 
	 */
	class ChatMessage: public MultiListenableObject {
	
		public:
			/**
			* @ref Direction is used to indicate if a message is outgoing or incoming. 
			*
			*/
			enum class Direction {
				/**
				* Incoming message. 
				*/
				Incoming,
				/**
				* Outgoing message. 
				*/
				Outgoing
			};

			/**
			* @ref State is used to notify if messages have been successfully delivered or
			* not. 
			*
			*/
			enum class State {
				/**
				* Initial state. 
				*/
				Idle,
				/**
				* Delivery in progress. 
				*/
				InProgress,
				/**
				* Message successfully delivered and acknowledged by the server. 
				*/
				Delivered,
				/**
				* Message was not delivered. 
				*/
				NotDelivered,
				/**
				* Message was received and acknowledged but cannot get file from server. 
				*/
				FileTransferError,
				/**
				* File transfer has been completed successfully. 
				*/
				FileTransferDone,
				/**
				* Message successfully delivered an acknowledged by the remote user. 
				*/
				DeliveredToUser,
				/**
				* Message successfully displayed to the remote user. 
				*/
				Displayed,
				/**
				*/
				FileTransferInProgress
			};


			ChatMessage(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneChatMessage *cPtr() {return (_LinphoneChatMessage *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<ChatMessageListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<ChatMessageListener> &listener);
	
		public:
			
		
			/**
			 * @brief Linphone message has an app-specific field that can store a text. 
			 *
			 * The application might want to use it for keeping data over restarts, like
			 * thumbnail image path. 
			 * @return the application-specific data or nullptr if none has been stored.   
			 */
			LINPHONECXX_PUBLIC std::string getAppdata() const;
			
			/**
			 * @brief Linphone message has an app-specific field that can store a text. 
			 *
			 * The application might want to use it for keeping data over restarts, like
			 * thumbnail image path.
			 * Invoking this function will attempt to update the message storage to reflect
			 * the change if it is enabled.
			 * @param data the data to store into the message.   
			 */
			LINPHONECXX_PUBLIC void setAppdata(const std::string & data);
			
			/**
			 * @brief Gets the callId accociated with the message. 
			 *
			 * @return the call Id   
			 */
			LINPHONECXX_PUBLIC std::string getCallId() const;
			
			/**
			 * @brief Returns the chatroom this message belongs to. 
			 *
			 * @return the @ref ChatRoom in which this message has been sent or received.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatRoom> getChatRoom() const;
			
			/**
			 * @brief Get the content type of a chat message. 
			 *
			 * @return The content type of the chat message   
			 */
			LINPHONECXX_PUBLIC std::string getContentType() const;
			
			/**
			 * @brief Set the content type of a chat message. 
			 *
			 * This content type must match a content that is text representable, such as
			 * text/plain, text/html or image/svg+xml. 
			 * @param contentType The new content type of the chat message   
			 */
			LINPHONECXX_PUBLIC void setContentType(const std::string & contentType);
			
			/**
			 * @brief Returns the list of contents in the message. 
			 *
			 * @return The list of @ref Content.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Content>> getContents() const;
			
			/**
			 * @brief Returns back pointer to @ref Core object. 
			 *
			 * @return the @ref Core object associated with this message.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> getCore() const;
			
			/**
			 * @brief Returns the real time at which an ephemeral message expires and will be
			 * deleted. 
			 *
			 * @see isEphemeral() 
			 * @return the time at which an ephemeral message expires. 0 means the message has
			 * not been read. 
			 */
			LINPHONECXX_PUBLIC time_t getEphemeralExpireTime() const;
			
			/**
			 * @brief Returns lifetime of an ephemeral message. 
			 *
			 * The lifetime is the duration after which the ephemeral message will disappear
			 * once viewed. @see isEphemeral() 
			 * @return the lifetime of an ephemeral message, by default 86400s. 
			 */
			LINPHONECXX_PUBLIC long getEphemeralLifetime() const;
			
			/**
			 * @brief Get full details about delivery error of a chat message. 
			 *
			 * @return a @ref ErrorInfo describing the details.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::ErrorInfo> getErrorInfo() const;
			
			/**
			 * @brief Linphone message can carry external body as defined by rfc2017. 
			 *
			 * @return external body url or nullptr if not present.   
			 */
			LINPHONECXX_PUBLIC std::string getExternalBodyUrl() const;
			
			/**
			 * @brief Linphone message can carry external body as defined by rfc2017. 
			 *
			 * @param externalBodyUrl ex: access-type=URL; URL="http://www.foo.com/file"   
			 */
			LINPHONECXX_PUBLIC void setExternalBodyUrl(const std::string & externalBodyUrl);
			
			/**
			 * @brief Get the file_transfer_information (used by call backs to recover
			 * informations during a rcs file transfer) 
			 *
			 * @return a pointer to the @ref Content structure or nullptr if not present.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Content> getFileTransferInformation() const;
			
			/**
			 * @brief Gets the forward info if available as a string. 
			 *
			 * @return the original sender of the message if it has been forwarded, nullptr
			 * otherwise.   
			 */
			LINPHONECXX_PUBLIC std::string getForwardInfo() const;
			
			/**
			 * @brief Get origin of the message. 
			 *
			 * @return the @ref Address of the sender.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getFromAddress() const;
			
			/**
			 * @brief Returns wether the chat message is an ephemeral message or not. 
			 *
			 * An ephemeral message will automatically disappear from the recipient's screen
			 * after the message has been viewed. 
			 * @return true if it is an ephemeral message, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isEphemeral() const;
			
			/**
			 * @brief Return whether or not a chat message is a file transfer. 
			 *
			 * @return Whether or not the message is a file transfer 
			 * @deprecated 06/07/2020 check if getContents() contains a @ref Content for which
			 * Content::isFileTransfer() returns true. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED bool isFileTransfer() const;
			
			/**
			 * @brief Gets whether or not a file is currently being downloaded or uploaded. 
			 *
			 * @return true if download or upload is in progress, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isFileTransferInProgress() const;
			
			/**
			 * @brief Returns wether the chat message is a forward message or not. 
			 *
			 * @return true if it is a forward message, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isForward();
			
			/**
			 * @brief Returns wehther the message has been sent or received. 
			 *
			 * @return true if message has been sent, false if it has been received. 
			 */
			LINPHONECXX_PUBLIC bool isOutgoing() const;
			
			/**
			 * @brief Returns wether the message has been read or not. 
			 *
			 * @return true if message has been marked as read, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isRead() const;
			
			/**
			 * @brief Get if the message was encrypted when transfered. 
			 *
			 * @return true if the message was encrypted when transfered, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isSecured() const;
			
			/**
			 * @brief Return whether or not a chat message is a text. 
			 *
			 * @return Whether or not the message is a text 
			 * @deprecated 06/07/2020 check if getContents() contains a @ref Content with a
			 * PlainText content type. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED bool isText() const;
			
			/**
			 * @brief Returns the local address the message was sent or received with. 
			 *
			 * @return the @ref Address of the local address used to send/receive this
			 * message.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getLocalAddress() const;
			
			/**
			 * @brief Get the message identifier. 
			 *
			 * It is used to identify a message so that it can be notified as delivered and/or
			 * displayed. 
			 * @return The message identifier.   
			 */
			LINPHONECXX_PUBLIC std::string getMessageId() const;
			
			/**
			 * @brief Get the state of the message. 
			 *
			 * @return the current @ref State of the message. 
			 */
			LINPHONECXX_PUBLIC State getState() const;
			
			/**
			 * @brief Gets the text content if available as a string. 
			 *
			 * @return the @ref Content buffer if available in System Locale, null otherwise. 
			 *  
			 * @deprecated 01/07/2020. Use getUtf8Text() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::string getTextContent() const;
			
			/**
			 * @brief Get the time the message was sent. 
			 *
			 * @return the timestamp of when the message was sent. 
			 */
			LINPHONECXX_PUBLIC time_t getTime() const;
			
			/**
			 * @brief Get destination of the message. 
			 *
			 * @return the @ref Address of the recipient.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getToAddress() const;
			
			/**
			 * @brief Get if a chat message is to be stored. 
			 *
			 * @return Whether or not the message is to be stored 
			 */
			LINPHONECXX_PUBLIC bool getToBeStored() const;
			
			/**
			 * @brief Set if a chat message is to be stored. 
			 *
			 * This content type must match a content that is text representable, such as
			 * text/plain, text/html or image/svg+xml. 
			 * @param toBeStored Whether or not the chat message is to be stored 
			 */
			LINPHONECXX_PUBLIC void setToBeStored(bool toBeStored);
			
			/**
			 * @brief Get text part of this message. 
			 *
			 * Introduced in 01/07/2020 
			 * @return The text in UTF8 or nullptr if no text.   
			 */
			LINPHONECXX_PUBLIC std::string getUtf8Text() const;
			
			/**
			 * @brief Set a chat message text to be sent by linphone_chat_room_send_message. 
			 *
			 * Introduced in 01/07/2020 
			 * @param text The text in UTF8 to set.   
			 * @return 0 if succeed. 
			 */
			LINPHONECXX_PUBLIC int setUtf8Text(const std::string & text);
			
			/**
			 * @brief Adds a content to the ChatMessage. 
			 *
			 * @param content the @ref Content object to add.   
			 */
			LINPHONECXX_PUBLIC void addContent(const std::shared_ptr<linphone::Content> & content);
			
			/**
			 * @brief Add custom headers to the message. 
			 *
			 * @param headerValue header value   
			 */
			LINPHONECXX_PUBLIC void addCustomHeader(const std::string & headerName, const std::string & headerValue);
			
			/**
			 * @brief Adds a file content to the ChatMessage. 
			 *
			 * @param content the @ref Content object to add.   
			 */
			LINPHONECXX_PUBLIC void addFileContent(const std::shared_ptr<linphone::Content> & content);
			
			/**
			 * @brief Creates a @ref Content of type PlainText with the given text as body. 
			 *
			 * @param text The text in System Locale to add to the message.   
			 * @deprecated 01/07/2020. Use addUtf8TextContent() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED void addTextContent(const std::string & text);
			
			/**
			 * @brief Creates a @ref Content of type PlainText with the given text as body. 
			 *
			 * Introduced in 01/07/2020 
			 * @param text The text in UTF8 to add to the message.   
			 */
			LINPHONECXX_PUBLIC void addUtf8TextContent(const std::string & text);
			
			/**
			 * @brief Cancel an ongoing file transfer attached to this message. 
			 *
			 * (upload or download) 
			 */
			LINPHONECXX_PUBLIC void cancelFileTransfer();
			
			/**
			 * @brief Start the download of the @ref Content referenced in the @ref
			 * ChatMessage from remote server. 
			 *
			 * @param content the @ref Content object to download (must have the
			 * Content::isFileTransfer() method return true).   
			 * @return false if there is an error, true otherwise. 
			 */
			LINPHONECXX_PUBLIC bool downloadContent(const std::shared_ptr<linphone::Content> & content);
			
			/**
			 * @brief Retrieve a custom header value given its name. 
			 *
			 * @param headerName header name searched   
			 * @return the custom header value or nullptr if not found.   
			 */
			LINPHONECXX_PUBLIC std::string getCustomHeader(const std::string & headerName) const;
			
			/**
			 * @brief Gets the list of participants for which the imdn state has reached the
			 * specified state and the time at which they did. 
			 *
			 * @param state The LinphoneChatMessageState the imdn have reached (only use
			 * LinphoneChatMessageStateDelivered, LinphoneChatMessageStateDeliveredToUser,
			 * LinphoneChatMessageStateDisplayed and LinphoneChatMessageStateNotDelivered) 
			 * @return The list of participants.      
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::ParticipantImdnState>> getParticipantsByImdnState(State state) const;
			
			/**
			 * @brief Returns wether the chat message has a text content or not. 
			 *
			 * @return true if it has one, false otherwise. 
			 * @deprecated 27/10/2020. Check if getContents() contains a @ref Content for
			 * which it's content type is PlainText. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED bool hasTextContent() const;
			
			/**
			 * @brief Fulfill a chat message char by char. 
			 *
			 * Message linked to a Real Time Text Call send char in realtime following RFC
			 * 4103/T.140 To commit a message, use linphone_chat_room_send_message 
			 * @param character T.140 char 
			 * @return 0 if succeed. 
			 */
			LINPHONECXX_PUBLIC linphone::Status putChar(uint32_t character);
			
			/**
			 * @brief Removes a content from the ChatMessage. 
			 *
			 * @param content the @ref Content object to remove.   
			 */
			LINPHONECXX_PUBLIC void removeContent(const std::shared_ptr<linphone::Content> & content);
			
			/**
			 * @brief Removes a custom header from the message. 
			 *
			 * @param headerName name of the header to remove   
			 */
			LINPHONECXX_PUBLIC void removeCustomHeader(const std::string & headerName);
			
			/**
			 * @brief Send a chat message. 
			 *
			 */
			LINPHONECXX_PUBLIC void send();
			
			
		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_CHAT_MESSAGE_HH
