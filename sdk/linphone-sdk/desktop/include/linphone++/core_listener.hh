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

#ifndef _LINPHONE_CORE_LISTENER_HH
#define _LINPHONE_CORE_LISTENER_HH

#include <string>
#include "enums.hh"
#include "call.hh"
#include "core.hh"
#include "conference.hh"
#include "chat_room.hh"
#include "object.hh"






namespace linphone {

	class Core;
	class ProxyConfig;
	class Call;
	class Friend;
	class PresenceModel;
	class AuthInfo;
	class CallLog;
	class ChatRoom;
	class ChatMessage;
	class CallStats;
	class InfoMessage;
	class Event;
	class Content;
	class FriendList;
	class Conference;
	class AudioDevice;

	/**
	 *
	 */
	class CoreListener: public Listener {
	
		public:


	
		public:
			
		
			/**
			 * @brief Global state notification callback. 
			 *
			 * @param message informational message.   
			 */
			LINPHONECXX_PUBLIC virtual void onGlobalStateChanged(const std::shared_ptr<linphone::Core> & core, linphone::GlobalState state, const std::string & message) {};
			
			/**
			 * @brief Registration state notification callback prototype. 
			 *
			 * @param message a non nullptr informational message about the state   
			 */
			LINPHONECXX_PUBLIC virtual void onRegistrationStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ProxyConfig> & proxyConfig, linphone::RegistrationState state, const std::string & message) {};
			
			/**
			 * @brief Call state notification callback. 
			 *
			 * @param message a non nullptr informational message about the state.   
			 */
			LINPHONECXX_PUBLIC virtual void onCallStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, linphone::Call::State state, const std::string & message) {};
			
			/**
			 * @brief Report status change for a friend previously added to the LinphoneCore
			 * with linphone_core_add_friend(). 
			 *
			 * @param linphoneFriend Updated LinphoneFriend   
			 */
			LINPHONECXX_PUBLIC virtual void onNotifyPresenceReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Friend> & linphoneFriend) {};
			
			/**
			 * @brief Reports presence model change for a specific URI or phone number of a
			 * friend. 
			 *
			 * @param presenceModel The new LinphonePresenceModel   
			 */
			LINPHONECXX_PUBLIC virtual void onNotifyPresenceReceivedForUriOrTel(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Friend> & linphoneFriend, const std::string & uriOrTel, const std::shared_ptr<const linphone::PresenceModel> & presenceModel) {};
			
			/**
			 * @brief Reports that a new subscription request has been received and wait for a
			 * decision. 
			 *
			 * @note A subscription request is notified by this function only if the
			 * LinphoneSubscribePolicy for the given LinphoneFriend has been set to
			 * LinphoneSPWait. See linphone_friend_set_inc_subscribe_policy. 
			 * @param url URI of the subscriber   
			 */
			LINPHONECXX_PUBLIC virtual void onNewSubscriptionRequested(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Friend> & linphoneFriend, const std::string & url) {};
			
			/**
			 * @brief Callback for requesting authentication information to application or
			 * user. 
			 *
			 * @param method the type of authentication requested as LinphoneAuthMethod enum  
			 * Application shall reply to this callback using linphone_core_add_auth_info. 
			 */
			LINPHONECXX_PUBLIC virtual void onAuthenticationRequested(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::AuthInfo> & authInfo, linphone::AuthMethod method) {};
			
			/**
			 * @brief Callback to notify a new call-log entry has been added. 
			 *
			 * This is done typically when a call terminates. 
			 * @param callLog the new LinphoneCallLog entry added.   
			 */
			LINPHONECXX_PUBLIC virtual void onCallLogUpdated(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::CallLog> & callLog) {};
			
			/**
			 * @brief Callback to notify the callid of a call has been updated. 
			 *
			 * This is done typically when a call retry. 
			 * @param currentCallId the new callid.   
			 */
			LINPHONECXX_PUBLIC virtual void onCallIdUpdated(const std::shared_ptr<linphone::Core> & core, const std::string & previousCallId, const std::string & currentCallId) {};
			
			/**
			 * @brief Chat message callback prototype. 
			 *
			 * @param message LinphoneChatMessage incoming message   
			 */
			LINPHONECXX_PUBLIC virtual void onMessageReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom, const std::shared_ptr<linphone::ChatMessage> & message) {};
			
			/**
			 * @brief Called after the #send method of the LinphoneChatMessage was called. 
			 *
			 * The message will be in state InProgress. In case of resend this callback won't
			 * be called. 
			 * @param message LinphoneChatMessage outgoing message   
			 */
			LINPHONECXX_PUBLIC virtual void onMessageSent(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom, const std::shared_ptr<linphone::ChatMessage> & message) {};
			
			/**
			 * @brief Chat room marked as read callback. 
			 *
			 * @param chatRoom LinphoneChatRoom that has been marked as read.   
			 */
			LINPHONECXX_PUBLIC virtual void onChatRoomRead(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom) {};
			
			/**
			 * @brief Chat message not decrypted callback prototype. 
			 *
			 * @param message LinphoneChatMessage incoming message   
			 */
			LINPHONECXX_PUBLIC virtual void onMessageReceivedUnableDecrypt(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom, const std::shared_ptr<linphone::ChatMessage> & message) {};
			
			/**
			 * @brief Is composing notification callback prototype. 
			 *
			 * @param chatRoom LinphoneChatRoom involved in the conversation.   
			 */
			LINPHONECXX_PUBLIC virtual void onIsComposingReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom) {};
			
			/**
			 * @brief Callback for being notified of DTMFs received. 
			 *
			 * @param dtmf the ascii code of the dtmf 
			 */
			LINPHONECXX_PUBLIC virtual void onDtmfReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, int dtmf) {};
			
			/**
			 * @brief Callback prototype for when a refer is received. 
			 *
			 * @param referTo the address of the refer   
			 */
			LINPHONECXX_PUBLIC virtual void onReferReceived(const std::shared_ptr<linphone::Core> & core, const std::string & referTo) {};
			
			/**
			 * @brief Call encryption changed callback. 
			 *
			 * @param authenticationToken an authentication_token, currently set for ZRTP kind
			 * of encryption only.   
			 */
			LINPHONECXX_PUBLIC virtual void onCallEncryptionChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, bool mediaEncryptionEnabled, const std::string & authenticationToken) {};
			
			/**
			 * @brief Callback for notifying progresses of transfers. 
			 *
			 * @param callState the LinphoneCallState of the call to transfer target at the
			 * far end. 
			 */
			LINPHONECXX_PUBLIC virtual void onTransferStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & transfered, linphone::Call::State callState) {};
			
			/**
			 * @brief Callback prototype when using the buddy plugin. 
			 *
			 * @param linphoneFriend the LinphoneFriend that has been updated   
			 */
			LINPHONECXX_PUBLIC virtual void onBuddyInfoUpdated(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Friend> & linphoneFriend) {};
			
			/**
			 * @brief Callback for receiving quality statistics for calls. 
			 *
			 * @param callStats the call statistics.   
			 */
			LINPHONECXX_PUBLIC virtual void onCallStatsUpdated(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, const std::shared_ptr<const linphone::CallStats> & callStats) {};
			
			/**
			 * @brief Callback prototype for receiving info messages. 
			 *
			 * @param message the info message.   
			 */
			LINPHONECXX_PUBLIC virtual void onInfoReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call, const std::shared_ptr<const linphone::InfoMessage> & message) {};
			
			/**
			 * @brief Callback prototype for notifying the application about changes of
			 * subscription states, including arrival of new subscriptions. 
			 *
			 * @param state the new LinphoneSubscriptionState 
			 */
			LINPHONECXX_PUBLIC virtual void onSubscriptionStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Event> & linphoneEvent, linphone::SubscriptionState state) {};
			
			/**
			 * @brief Callback prototype for notifying the application about notification
			 * received from the network. 
			 *
			 * @param body the LinphoneContent of the event   
			 */
			LINPHONECXX_PUBLIC virtual void onNotifyReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Event> & linphoneEvent, const std::string & notifiedEvent, const std::shared_ptr<const linphone::Content> & body) {};
			
			/**
			 * @brief Callback prototype for notifying the application about subscription
			 * received from the network. 
			 *
			 * @param body the LinphoneContent of the event   
			 */
			LINPHONECXX_PUBLIC virtual void onSubscribeReceived(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Event> & linphoneEvent, const std::string & subscribeEvent, const std::shared_ptr<const linphone::Content> & body) {};
			
			/**
			 * @brief Callback prototype for notifying the application about changes of
			 * publish states. 
			 *
			 * @param state the new LinphonePublishState 
			 */
			LINPHONECXX_PUBLIC virtual void onPublishStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Event> & linphoneEvent, linphone::PublishState state) {};
			
			/**
			 * @brief Callback prototype for configuring status changes notification. 
			 *
			 * @param message informational message.   
			 */
			LINPHONECXX_PUBLIC virtual void onConfiguringStatus(const std::shared_ptr<linphone::Core> & core, linphone::ConfiguringState status, const std::string & message) {};
			
			/**
			 * @brief Callback prototype for reporting network change either automatically
			 * detected or notified by linphone_core_set_network_reachable. 
			 *
			 * @param reachable true if network is reachable. 
			 */
			LINPHONECXX_PUBLIC virtual void onNetworkReachable(const std::shared_ptr<linphone::Core> & core, bool reachable) {};
			
			/**
			 * @brief Callback prototype for reporting log collection upload state change. 
			 *
			 * @param info Additional information: error message in case of error state, URL
			 * of uploaded file in case of success.   
			 */
			LINPHONECXX_PUBLIC virtual void onLogCollectionUploadStateChanged(const std::shared_ptr<linphone::Core> & core, linphone::Core::LogCollectionUploadState state, const std::string & info) {};
			
			/**
			 * @brief Callback prototype for reporting log collection upload progress
			 * indication. 
			 *
			 * @param total the total number of bytes to upload 
			 */
			LINPHONECXX_PUBLIC virtual void onLogCollectionUploadProgressIndication(const std::shared_ptr<linphone::Core> & core, size_t offset, size_t total) {};
			
			/**
			 * @brief Callback prototype for reporting when a friend list has been added to
			 * the core friends list. 
			 *
			 * @param friendList LinphoneFriendList object   
			 */
			LINPHONECXX_PUBLIC virtual void onFriendListCreated(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::FriendList> & friendList) {};
			
			/**
			 * @brief Callback prototype for reporting when a friend list has been removed
			 * from the core friends list. 
			 *
			 * @param friendList LinphoneFriendList object   
			 */
			LINPHONECXX_PUBLIC virtual void onFriendListRemoved(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::FriendList> & friendList) {};
			
			/**
			 * @brief Callback notifying that a new LinphoneCall (either incoming or outgoing)
			 * has been created. 
			 *
			 * @param call The newly created LinphoneCall object   
			 */
			LINPHONECXX_PUBLIC virtual void onCallCreated(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Call> & call) {};
			
			/**
			 * @brief Callback prototype for reporting the result of a version update check. 
			 *
			 * @param url The url where to download the new version if the result is
			 * #LinphoneVersionUpdateCheckNewVersionAvailable   
			 */
			LINPHONECXX_PUBLIC virtual void onVersionUpdateCheckResultReceived(const std::shared_ptr<linphone::Core> & core, linphone::VersionUpdateCheckResult result, const std::string & version, const std::string & url) {};
			
			/**
			 * @brief Callback prototype telling that a LinphoneConference state has changed. 
			 *
			 * @param state the current LinphoneChatRoomState 
			 */
			LINPHONECXX_PUBLIC virtual void onConferenceStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::Conference> & conference, linphone::Conference::State state) {};
			
			/**
			 * @brief Callback prototype telling that a LinphoneChatRoom state has changed. 
			 *
			 * @param state the current LinphoneChatRoomState 
			 */
			LINPHONECXX_PUBLIC virtual void onChatRoomStateChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom, linphone::ChatRoom::State state) {};
			
			/**
			 * @brief Callback prototype telling that a LinphoneChatRoom subject has changed. 
			 *
			 * @param chatRoom The LinphoneChatRoom object for which the subject has changed   
			 */
			LINPHONECXX_PUBLIC virtual void onChatRoomSubjectChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom) {};
			
			/**
			 * @brief Callback prototype telling that a LinphoneChatRoom ephemeral message has
			 * expired. 
			 *
			 * @param chatRoom The LinphoneChatRoom object for which a message has expired.   
			 */
			LINPHONECXX_PUBLIC virtual void onChatRoomEphemeralMessageDeleted(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::ChatRoom> & chatRoom) {};
			
			/**
			 * @brief Callback prototype telling that an Instant Message Encryption Engine
			 * user registered on the server with or without success. 
			 *
			 * @param info information about failure   
			 */
			LINPHONECXX_PUBLIC virtual void onImeeUserRegistration(const std::shared_ptr<linphone::Core> & core, const bool status, const std::string & userId, const std::string & info) {};
			
			/**
			 * @brief Callback prototype telling the result of decoded qrcode. 
			 *
			 * @param result The result of the decoded qrcode   
			 */
			LINPHONECXX_PUBLIC virtual void onQrcodeFound(const std::shared_ptr<linphone::Core> & core, const std::string & result) {};
			
			/**
			 * @brief Callback prototype telling a call has started (incoming or outgoing)
			 * while there was no other call. 
			 *
			 * @param core LinphoneCore object   
			 */
			LINPHONECXX_PUBLIC virtual void onFirstCallStarted(const std::shared_ptr<linphone::Core> & core) {};
			
			/**
			 * @brief Callback prototype telling the last call has ended
			 * (#LinphoneCore.get_calls_nb() returns 0) 
			 *
			 * @param core LinphoneCore object   
			 */
			LINPHONECXX_PUBLIC virtual void onLastCallEnded(const std::shared_ptr<linphone::Core> & core) {};
			
			/**
			 * @brief Callback prototype telling that the audio device for at least one call
			 * has changed. 
			 *
			 * @param audioDevice the newly used LinphoneAudioDevice object   
			 */
			LINPHONECXX_PUBLIC virtual void onAudioDeviceChanged(const std::shared_ptr<linphone::Core> & core, const std::shared_ptr<linphone::AudioDevice> & audioDevice) {};
			
			/**
			 * @brief Callback prototype telling the audio devices list has been updated. 
			 *
			 * Either a new device is available or a previously available device isn't
			 * anymore. You can call linphone_core_get_audio_devices to get the new list. 
			 * @param core LinphoneCore object   
			 */
			LINPHONECXX_PUBLIC virtual void onAudioDevicesListUpdated(const std::shared_ptr<linphone::Core> & core) {};
			
			/**
			 * @brief Function prototype used by
			 * #linphone_core_cbs_set_ec_calibrator_result(). 
			 *
			 * @param delayMs The measured delay if available. 
			 */
			LINPHONECXX_PUBLIC virtual void onEcCalibrationResult(const std::shared_ptr<linphone::Core> & core, linphone::EcCalibratorStatus status, int delayMs) {};
			
			/**
			 * @brief Function prototype used by
			 * #linphone_core_cbs_set_ec_calibrator_audio_init(). 
			 *
			 * @param core The LinphoneCore.   
			 */
			LINPHONECXX_PUBLIC virtual void onEcCalibrationAudioInit(const std::shared_ptr<linphone::Core> & core) {};
			
			/**
			 * @brief Function prototype used by
			 * #linphone_core_cbs_set_ec_calibrator_audio_uninit(). 
			 *
			 * @param core The LinphoneCore.   
			 */
			LINPHONECXX_PUBLIC virtual void onEcCalibrationAudioUninit(const std::shared_ptr<linphone::Core> & core) {};
			
			
	};

};

#endif // _LINPHONE_CORE_LISTENER_HH
