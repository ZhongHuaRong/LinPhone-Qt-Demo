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

#ifndef _LINPHONE_CALL_HH
#define _LINPHONE_CALL_HH

#include <string>
#include "call.hh"
#include "enums.hh"
#include "object.hh"




struct _LinphoneCall;


namespace linphone {

	class CallStats;
	class CallLog;
	class ChatRoom;
	class Conference;
	class Core;
	class CallParams;
	class Address;
	class ErrorInfo;
	class AudioDevice;
	class Player;
	class Call;
	class InfoMessage;
	class CallListener;

	/**
	 * @brief This object represents a call issued or received by the @ref Core. 
	 *
	 * You may have multiple calls at the same time, but only one will be in @ref
	 * State::StreamsRunning at any time unless they are merged into a @ref
	 * Conference, others will be paused.
	 * You can get the @ref State of the call using getState(), it's current @ref
	 * CallParams with getCurrentParams() and the latest statistics by calling
	 * getAudioStats() or getVideoStats(). 
	 */
	class Call: public MultiListenableObject {
	
		public:
			/**
			* Enum representing the status of a call. 
			*
			*/
			enum class Status {
				/**
				* The call was sucessful. 
				*/
				Success,
				/**
				* The call was aborted (caller hanged up) 
				*/
				Aborted,
				/**
				* The call was missed (incoming call timed out without being answered or hanged
				* up) 
				*/
				Missed,
				/**
				* The call was declined, either locally or by remote end. 
				*/
				Declined,
				/**
				* The call was aborted before being advertised to the application - for protocol
				* reasons. 
				*/
				EarlyAborted,
				/**
				* The call was answered on another device. 
				*/
				AcceptedElsewhere,
				/**
				* The call was declined on another device. 
				*/
				DeclinedElsewhere
			};

			/**
			* Enum representing the direction of a call. 
			*
			*/
			enum class Dir {
				/**
				* outgoing calls 
				*/
				Outgoing,
				/**
				* incoming calls 
				*/
				Incoming
			};

			/**
			* @ref State enum represents the different states a call can reach into. 
			*
			*/
			enum class State {
				/**
				* Initial state. 
				*/
				Idle,
				/**
				* Incoming call received. 
				*/
				IncomingReceived,
				/**
				* PushIncoming call received. 
				*/
				PushIncomingReceived,
				/**
				* Outgoing call initialized. 
				*/
				OutgoingInit,
				/**
				* Outgoing call in progress. 
				*/
				OutgoingProgress,
				/**
				* Outgoing call ringing. 
				*/
				OutgoingRinging,
				/**
				* Outgoing call early media. 
				*/
				OutgoingEarlyMedia,
				/**
				* Connected. 
				*/
				Connected,
				/**
				* Streams running. 
				*/
				StreamsRunning,
				/**
				* Pausing. 
				*/
				Pausing,
				/**
				* Paused. 
				*/
				Paused,
				/**
				* Resuming. 
				*/
				Resuming,
				/**
				* Referred. 
				*/
				Referred,
				/**
				* Error. 
				*/
				Error,
				/**
				* Call end. 
				*/
				End,
				/**
				* Paused by remote. 
				*/
				PausedByRemote,
				/**
				* The call's parameters are updated for example when video is asked by remote. 
				*/
				UpdatedByRemote,
				/**
				* We are proposing early media to an incoming call. 
				*/
				IncomingEarlyMedia,
				/**
				* We have initiated a call update. 
				*/
				Updating,
				/**
				* The call object is now released. 
				*/
				Released,
				/**
				* The call is updated by remote while not yet answered (SIP UPDATE in early
				* dialog received) 
				*/
				EarlyUpdatedByRemote,
				/**
				* We are updating the call while not yet answered (SIP UPDATE in early dialog
				* sent) 
				*/
				EarlyUpdating
			};


			Call(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneCall *cPtr() {return (_LinphoneCall *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<CallListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<CallListener> &listener);
	
		public:
			
		
			/**
			 * @brief Returns a copy of the call statistics for the audio stream. 
			 *
			 * @return a @ref CallStats object for the audio stream or nullptr if it isn't
			 * available.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallStats> getAudioStats();
			
			/**
			 * @brief Returns the ZRTP authentication token to verify. 
			 *
			 * @return the authentication token to verify or nullptr if ZRTP isn't enabled.   
			 */
			LINPHONECXX_PUBLIC std::string getAuthenticationToken();
			
			/**
			 * @brief Returns whether ZRTP authentication token is verified. 
			 *
			 * If not, it must be verified by users as described in ZRTP procedure. Once done,
			 * the application must inform of the results with
			 * setAuthenticationTokenVerified(). 
			 * @return true if authentication token is verifed, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getAuthenticationTokenVerified() const;
			
			/**
			 * @brief Set the result of ZRTP short code verification by user. 
			 *
			 * If remote party also does the same, it will update the ZRTP cache so that
			 * user's verification will not be required for the two users. 
			 * @param verified whether the ZRTP SAS is verified. 
			 */
			LINPHONECXX_PUBLIC void setAuthenticationTokenVerified(bool verified);
			
			/**
			 * @brief Returns call quality averaged over all the duration of the call. 
			 *
			 * See getCurrentQuality() for more details about quality measurement. 
			 * @return the call average quality since tbe beginning of the call. 
			 */
			LINPHONECXX_PUBLIC float getAverageQuality() const;
			
			/**
			 * @brief Gets the call log associated to this call. 
			 *
			 * @return The @ref CallLog associated with the specified @ref Call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallLog> getCallLog() const;
			
			/**
			 * @brief Returns if camera pictures are allowed to be sent to the remote party. 
			 *
			 * @return true if local video stream is being sent, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool cameraEnabled() const;
			
			/**
			 * @brief Indicate whether camera input should be sent to remote end. 
			 *
			 * @param enabled wether or not to send local video stream. 
			 */
			LINPHONECXX_PUBLIC void enableCamera(bool enabled);
			
			/**
			 * @brief Create a new chat room for real time messaging from a call if not
			 * already existing, else return existing one. 
			 *
			 * No reference is given to the caller: the chat room will be deleted when the
			 * call is ended. 
			 * @return @ref ChatRoom where real time messaging can take place or nullptr if
			 * chat room couldn't be created.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatRoom> getChatRoom();
			
			/**
			 * @brief Return the associated conference object. 
			 *
			 * @return A pointer on @ref Conference or nullptr if the call is not part of any
			 * conference.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Conference> getConference() const;
			
			/**
			 * @brief Get the core that has created the specified call. 
			 *
			 * @return The @ref Core object that has created the specified call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> getCore() const;
			
			/**
			 * @brief Returns current parameters associated to the call. 
			 *
			 * @return the current @ref CallParams of this call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::CallParams> getCurrentParams();
			
			/**
			 * @brief Obtain real-time quality rating of the call. 
			 *
			 * Based on local RTP statistics and RTCP feedback, a quality rating is computed
			 * and updated during all the duration of the call. This function returns its
			 * value at the time of the function call. It is expected that the rating is
			 * updated at least every 5 seconds or so. The rating is a floating point number
			 * comprised between 0 and 5.
			 * 4-5 = good quality  3-4 = average quality  2-3 = poor quality  1-2 = very poor
			 * quality  0-1 = can't be worse, mostly unusable 
			 * @return The function returns -1 if no quality measurement is available, for
			 * example if no active audio stream exist. Otherwise it returns the quality
			 * rating. 
			 */
			LINPHONECXX_PUBLIC float getCurrentQuality() const;
			
			/**
			 * @brief Returns direction of the call (incoming or outgoing). 
			 *
			 * @return the @ref Dir 
			 */
			LINPHONECXX_PUBLIC Dir getDir() const;
			
			/**
			 * @brief Returns the diversion address associated to this call. 
			 *
			 * @return the diversion address as @ref Address or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getDiversionAddress() const;
			
			/**
			 * @brief Returns call's duration in seconds. 
			 *
			 * @return the call's duration in seconds. 
			 */
			LINPHONECXX_PUBLIC int getDuration() const;
			
			/**
			 * @brief Returns if echo cancellation is enabled. 
			 *
			 * @return true if echo cancellation is enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool echoCancellationEnabled() const;
			
			/**
			 * @brief Enables or disable echo cancellation for this call. 
			 *
			 * @param enable wether to enable echo cancellation or not. 
			 */
			LINPHONECXX_PUBLIC void enableEchoCancellation(bool enable);
			
			/**
			 * @brief Returns if echo limiter is enabled. 
			 *
			 * @return true if echo limiter is enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool echoLimiterEnabled() const;
			
			/**
			 * @brief Enables or disable echo limiter for this call. 
			 *
			 * @param enable wether to enable echo limiter or not. 
			 */
			LINPHONECXX_PUBLIC void enableEchoLimiter(bool enable);
			
			/**
			 * @brief Returns full details about call errors or termination reasons. 
			 *
			 * @return @ref ErrorInfo object holding the reason error.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::ErrorInfo> getErrorInfo() const;
			
			/**
			 * @brief Gets the current input device for this call. 
			 *
			 * @return the @ref AudioDevice used by this call as input or nullptr if there is
			 * currently no soundcard configured (depending on the state of the call)   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AudioDevice> getInputAudioDevice() const;
			
			/**
			 * @brief Sets the given @ref AudioDevice as input for this call only. 
			 *
			 * @param audioDevice The @ref AudioDevice. nullptr does nothing.   
			 */
			LINPHONECXX_PUBLIC void setInputAudioDevice(const std::shared_ptr<linphone::AudioDevice> & audioDevice);
			
			/**
			 * @brief Returns whether or not the call is currently being recorded. 
			 *
			 * @return true if recording is in progress, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isRecording();
			
			/**
			 * @brief Get microphone muted state. 
			 *
			 * @return The microphone muted state. 
			 */
			LINPHONECXX_PUBLIC bool getMicrophoneMuted() const;
			
			/**
			 * @brief Set microphone muted state. 
			 *
			 * @param muted The microphone muted state. 
			 */
			LINPHONECXX_PUBLIC void setMicrophoneMuted(bool muted);
			
			/**
			 * @brief Get microphone volume gain. 
			 *
			 * If the sound backend supports it, the returned gain is equal to the gain set
			 * with the system mixer. 
			 * @return double Percentage of the max supported volume gain. Valid values are in
			 * [ 0.0 : 1.0 ]. In case of failure, a negative value is returned 
			 */
			LINPHONECXX_PUBLIC float getMicrophoneVolumeGain() const;
			
			/**
			 * @brief Set microphone volume gain. 
			 *
			 * If the sound backend supports it, the new gain will synchronized with the
			 * system mixer. 
			 * @param volume Percentage of the max supported gain. Valid values are in [ 0.0 :
			 * 1.0 ]. 
			 */
			LINPHONECXX_PUBLIC void setMicrophoneVolumeGain(float volume);
			
			/**
			 * @brief Get the native window handle of the video window, casted as an unsigned
			 * long. 
			 *
			 * @return the native video window id (type may vary depending on platform).   
			 */
			LINPHONECXX_PUBLIC void * getNativeVideoWindowId() const;
			
			/**
			 * @brief Set the native video window id where the video is to be displayed. 
			 *
			 * For MacOS, Linux, Windows: if not set or 0 a window will be automatically
			 * created, unless the special id -1 is given. 
			 * @param windowId the native video window id.   
			 */
			LINPHONECXX_PUBLIC void setNativeVideoWindowId(void * windowId);
			
			/**
			 * @brief Gets the current output device for this call. 
			 *
			 * @return the @ref AudioDevice used by this call as output or nullptr if there is
			 * currently no soundcard configured (depending on the state of the call)   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AudioDevice> getOutputAudioDevice() const;
			
			/**
			 * @brief Sets the given @ref AudioDevice as output for this call only. 
			 *
			 * @param audioDevice The @ref AudioDevice. nullptr does nothing.   
			 */
			LINPHONECXX_PUBLIC void setOutputAudioDevice(const std::shared_ptr<linphone::AudioDevice> & audioDevice);
			
			/**
			 * @brief Returns local parameters associated with the call. 
			 *
			 * This is typically the parameters passed at call initiation to
			 * Core::inviteAddressWithParams() or acceptWithParams(), or some default
			 * parameters if no @ref CallParams was explicitely passed during call initiation. 
			 * @return the call's local parameters.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::CallParams> getParams();
			
			/**
			 * @brief Set call parameters - advanced and not recommended feature - use with
			 * caution. 
			 *
			 * Local call parameters applicable to an outgoing or incoming shall usually be
			 * passed to Core::inviteAddressWithParams() or acceptWithParams(). However, in
			 * some cases it might be desirable from a software design standpoint to modify
			 * local parameters outside of the application layer, typically in the purpose of
			 * implementing a custom logic including special headers in INVITE or 200Ok
			 * requests, driven by a call_state_changed listener method. This function accepts
			 * to assign a new @ref CallParams only in #LinphoneCallOutgoingInit and
			 * #LinphoneCallIncomingReceived states.
			 * @param params the @ref CallParams object   
			 */
			LINPHONECXX_PUBLIC void setParams(const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Get the mesured playback volume level (received from remote) in dbm0. 
			 *
			 * @return float Volume level in percentage. 
			 */
			LINPHONECXX_PUBLIC float getPlayVolume() const;
			
			/**
			 * @brief Gets a player associated with the call to play a local file and stream
			 * it to the remote peer. 
			 *
			 * @return A @ref Player object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Player> getPlayer();
			
			/**
			 * @brief Returns the reason for a call termination (either error or normal
			 * termination) 
			 *
			 * @return the @ref Reason of the call termination. 
			 */
			LINPHONECXX_PUBLIC linphone::Reason getReason() const;
			
			/**
			 * @brief Get the mesured record volume level (sent to remote) in dbm0. 
			 *
			 * @return float Volume level in percentage. 
			 */
			LINPHONECXX_PUBLIC float getRecordVolume() const;
			
			/**
			 * @brief Gets the refer-to uri (if the call was transfered). 
			 *
			 * @return The refer-to uri of the call (if it was transfered).   
			 */
			LINPHONECXX_PUBLIC std::string getReferTo() const;
			
			/**
			 * @brief Returns the remote address associated to this call. 
			 *
			 * @return The @ref Address of the remote end of the call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getRemoteAddress() const;
			
			/**
			 * @brief Returns the remote address associated to this call as a string. 
			 *
			 * The result string must be freed by user using ms_free(). 
			 * @return the remote address as a string.     
			 * @deprecated 06/07/2020 use getRemoteAddress() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::string getRemoteAddressAsString() const;
			
			/**
			 * @brief Returns the far end's sip contact as a string, if available. 
			 *
			 * @return the remote contact or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::string getRemoteContact();
			
			/**
			 * @brief Returns call parameters proposed by remote. 
			 *
			 * This is useful when receiving an incoming call, to know whether the remote
			 * party supports video, encryption or whatever.
			 * @return the @ref CallParams suggested by the remote or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::CallParams> getRemoteParams();
			
			/**
			 * @brief Returns the far end's user agent description string, if available. 
			 *
			 * @return the remote user agent or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::string getRemoteUserAgent();
			
			/**
			 * @brief Returns the call object this call is replacing, if any. 
			 *
			 * Call replacement can occur during call transfers. By default, the core
			 * automatically terminates the replaced call and accept the new one. This
			 * function allows the application to know whether a new incoming call is a one
			 * that replaces another one. 
			 * @return the @ref Call object this call is replacing or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getReplacedCall();
			
			/**
			 * @brief Get speaker muted state. 
			 *
			 * @return The speaker muted state. 
			 */
			LINPHONECXX_PUBLIC bool getSpeakerMuted() const;
			
			/**
			 * @brief Set speaker muted state. 
			 *
			 * @param muted The speaker muted state. 
			 */
			LINPHONECXX_PUBLIC void setSpeakerMuted(bool muted);
			
			/**
			 * @brief Get speaker volume gain. 
			 *
			 * If the sound backend supports it, the returned gain is equal to the gain set
			 * with the system mixer. 
			 * @return Percentage of the max supported volume gain. Valid values are in [ 0.0
			 * : 1.0 ]. In case of failure, a negative value is returned 
			 */
			LINPHONECXX_PUBLIC float getSpeakerVolumeGain() const;
			
			/**
			 * @brief Set speaker volume gain. 
			 *
			 * If the sound backend supports it, the new gain will synchronized with the
			 * system mixer. 
			 * @param volume Percentage of the max supported gain. Valid values are in [ 0.0 :
			 * 1.0 ]. 
			 */
			LINPHONECXX_PUBLIC void setSpeakerVolumeGain(float volume);
			
			/**
			 * @brief Retrieves the call's current state. 
			 *
			 * @return the current @ref State of this call. 
			 */
			LINPHONECXX_PUBLIC State getState() const;
			
			/**
			 * @brief Returns the number of stream for the given call. 
			 *
			 * @return the amount of streams for this call. 
			 */
			LINPHONECXX_PUBLIC int getStreamCount() const;
			
			/**
			 * @brief Returns a copy of the call statistics for the text stream. 
			 *
			 * @return a @ref CallStats object for the text stream or nullptr if it isn't
			 * available.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallStats> getTextStats();
			
			/**
			 * @brief Returns the to address with its headers associated to this call. 
			 *
			 * @return the #LinphoneAdress matching the TO of the call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getToAddress() const;
			
			/**
			 * @brief Returns the current transfer state, if a transfer has been initiated
			 * from this call. 
			 *
			 * @see linphone_core_transfer_call , linphone_core_transfer_call_to_another 
			 * @return the @ref State. 
			 */
			LINPHONECXX_PUBLIC State getTransferState();
			
			/**
			 * @brief When this call has received a transfer request, returns the new call
			 * that was automatically created as a result of the transfer. 
			 *
			 * @return the transfer @ref Call created.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getTransferTargetCall() const;
			
			/**
			 * @brief Gets the transferer if this call was started automatically as a result
			 * of an incoming transfer request. 
			 *
			 * The call in which the transfer request was received is returned in this case. 
			 * @return The transferer @ref Call if the specified call was started
			 * automatically as a result of an incoming transfer request, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getTransfererCall() const;
			
			/**
			 * @brief Returns a copy of the call statistics for the video stream. 
			 *
			 * @return a @ref CallStats object for the video stream or nullptr if it isn't
			 * available.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallStats> getVideoStats();
			
			/**
			 * @brief Accept an incoming call. 
			 *
			 * Basically the application is notified of incoming calls within the
			 * call_state_changed callback of the LinphoneCoreVTable structure, where it will
			 * receive a @ref Dir::Incoming event with the associated @ref Call object. The
			 * application can later accept the call using this method. 
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status accept();
			
			/**
			 * @brief Accept an early media session for an incoming call. 
			 *
			 * This is identical as calling acceptEarlyMediaWithParams() with nullptr
			 * parameters. 
			 * @return 0 if successful, -1 otherwise 
			 * @see acceptEarlyMediaWithParams() 
			 */
			LINPHONECXX_PUBLIC linphone::Status acceptEarlyMedia();
			
			/**
			 * @brief When receiving an incoming, accept to start a media session as
			 * early-media. 
			 *
			 * This means the call is not accepted but audio & video streams can be
			 * established if the remote party supports early media. However, unlike after
			 * call acceptance, mic and camera input are not sent during early-media, though
			 * received audio & video are played normally. The call can then later be fully
			 * accepted using accept() or acceptWithParams(). 
			 * @param params The call parameters to use (can be nullptr).   
			 * @return 0 if successful, -1 otherwise 
			 */
			LINPHONECXX_PUBLIC linphone::Status acceptEarlyMediaWithParams(const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Accept call modifications initiated by other end. 
			 *
			 * This call may be performed in response to a #LinphoneCallUpdatedByRemote state
			 * notification. When such notification arrives, the application can decide to
			 * call deferUpdate() so that it can have the time to prompt the user.
			 * getRemoteParams() can be used to get information about the call parameters
			 * requested by the other party, such as whether a video stream is requested.
			 * When the user accepts or refuse the change, acceptUpdate() can be done to
			 * answer to the other party. If params is nullptr, then the same call parameters
			 * established before the update request will continue to be used (no change). If
			 * params is not nullptr, then the update will be accepted according to the
			 * parameters passed. Typical example is when a user accepts to start video, then
			 * params should indicate that video stream should be used (see
			 * CallParams::enableVideo()). 
			 * @param params A @ref CallParams object describing the call parameters to
			 * accept.   
			 * @return 0 if successful, -1 otherwise (actually when this function call is
			 * performed outside ot #LinphoneCallUpdatedByRemote state) 
			 */
			LINPHONECXX_PUBLIC linphone::Status acceptUpdate(const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Accept an incoming call, with parameters. 
			 *
			 * Basically the application is notified of incoming calls within the
			 * call_state_changed callback of the LinphoneCoreVTable structure, where it will
			 * receive a @ref Dir::Incoming event with the associated @ref Call object. The
			 * application can later accept the call using this method. 
			 * @param params The specific parameters for this call, for example whether video
			 * is accepted or not. Use nullptr to use default parameters.   
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status acceptWithParams(const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Tell whether a call has been asked to autoanswer. 
			 *
			 * @return A boolean value telling whether the call has been asked to autoanswer 
			 */
			LINPHONECXX_PUBLIC bool askedToAutoanswer();
			
			/**
			 * @brief Stop current DTMF sequence sending. 
			 *
			 * Please note that some DTMF could be already sent, depending on when this
			 * function call is delayed from sendDtmfs(). This function will be automatically
			 * called if call state change to anything but LinphoneCallStreamsRunning. 
			 */
			LINPHONECXX_PUBLIC void cancelDtmfs();
			
			/**
			 * @brief Decline a pending incoming call, with a reason. 
			 *
			 * @param reason The reason for rejecting the call: @ref Reason::Declined or @ref
			 * Reason::Busy 
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status decline(linphone::Reason reason);
			
			/**
			 * @brief Decline a pending incoming call, with a @ref ErrorInfo object. 
			 *
			 * @param ei @ref ErrorInfo containing more information on the call rejection.   
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC int declineWithErrorInfo(const std::shared_ptr<const linphone::ErrorInfo> & ei);
			
			/**
			 * @brief When receiving a #LinphoneCallUpdatedByRemote state notification,
			 * prevent @ref Core from performing an automatic answer. 
			 *
			 * When receiving a #LinphoneCallUpdatedByRemote state notification (ie an
			 * incoming reINVITE), the default behaviour of @ref Core is defined by the
			 * "defer_update_default" option of the "sip" section of the config. If this
			 * option is 0 (the default) then the @ref Core automatically answers the reINIVTE
			 * with call parameters unchanged. However when for example when the remote party
			 * updated the call to propose a video stream, it can be useful to prompt the user
			 * before answering. This can be achieved by calling
			 * linphone_core_defer_call_update during the call state notification, to
			 * deactivate the automatic answer that would just confirm the audio but reject
			 * the video. Then, when the user responds to dialog prompt, it becomes possible
			 * to call acceptUpdate() to answer the reINVITE, with eventually video enabled in
			 * the @ref CallParams argument.
			 * The #LinphoneCallUpdatedByRemote notification can also arrive when receiving an
			 * INVITE without SDP. In such case, an unchanged offer is made in the 200Ok, and
			 * when the ACK containing the SDP answer is received,
			 * #LinphoneCallUpdatedByRemote is triggered to notify the application of possible
			 * changes in the media session. However in such case defering the update has no
			 * meaning since we just generating an offer.
			 * @return 0 if successful, -1 if the deferUpdate() was done outside a valid
			 * #LinphoneCallUpdatedByRemote notification 
			 */
			LINPHONECXX_PUBLIC linphone::Status deferUpdate();
			
			/**
			 * @brief Returns a copy of the call statistics for a particular stream type. 
			 *
			 * @param type the @ref StreamType 
			 * @return a @ref CallStats object for the given stream or nullptr if stream isn't
			 * available.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallStats> getStats(linphone::StreamType type);
			
			/**
			 * @brief Returns the value of the header name. 
			 *
			 * @param headerName the name of the header to check.   
			 * @return the value of the header if exists.   
			 * @deprecated 27/10/2020. Use CallParams::getCustomHeader() on getRemoteParams()
			 * instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::string getToHeader(const std::string & headerName) const;
			
			/**
			 * @brief Returns if this calls has received a transfer that has not been executed
			 * yet. 
			 *
			 * Pending transfers are executed when this call is being paused or closed,
			 * locally or by remote endpoint. If the call is already paused while receiving
			 * the transfer request, the transfer immediately occurs. 
			 * @return true if transfer is pending, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool hasTransferPending() const;
			
			/**
			 * @brief Indicates whether an operation is in progress at the media side. 
			 *
			 * It can be a bad idea to initiate signaling operations (adding video, pausing
			 * the call, removing video, changing video parameters) while the media is busy in
			 * establishing the connection (typically ICE connectivity checks). It can result
			 * in failures generating loss of time in future operations in the call.
			 * Applications are invited to check this function after each call state change to
			 * decide whether certain operations are permitted or not. 
			 * @return true if media is busy in establishing the connection, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool mediaInProgress() const;
			
			/**
			 * @brief Calls generic OpenGL render for a given call. 
			 *
			 */
			LINPHONECXX_PUBLIC void oglRender() const;
			
			/**
			 * @brief Pauses the call. 
			 *
			 * If a music file has been setup using Core::setPlayFile(), this file will be
			 * played to the remote user. The only way to resume a paused call is to call
			 * resume(). 
			 * @return 0 on success, -1 on failure 
			 * @see resume() 
			 */
			LINPHONECXX_PUBLIC linphone::Status pause();
			
			/**
			 * @brief Redirect the specified call to the given redirect URI. 
			 *
			 * @param redirectUri The URI to redirect the call to   
			 * @return 0 if successful, -1 on error. 
			 * @deprecated 27/10/2020. Use redirectTo() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED linphone::Status redirect(const std::string & redirectUri);
			
			/**
			 * @brief Redirect the specified call to the given redirect Address. 
			 *
			 * @param redirectAddress The @ref Address to redirect the call to   
			 * @return 0 if successful, -1 on error. 
			 */
			LINPHONECXX_PUBLIC linphone::Status redirectTo(const std::shared_ptr<const linphone::Address> & redirectAddress);
			
			/**
			 * @brief Request the callback passed to
			 * linphone_call_cbs_set_next_video_frame_decoded to be called the next time the
			 * video decoder properly decodes a video frame. 
			 *
			 */
			LINPHONECXX_PUBLIC void requestNotifyNextVideoFrameDecoded();
			
			/**
			 * @brief Resumes a call. 
			 *
			 * The call needs to have been paused previously with pause(). 
			 * @return 0 on success, -1 on failure 
			 * @see pause() 
			 */
			LINPHONECXX_PUBLIC linphone::Status resume();
			
			/**
			 * @brief Send the specified dtmf. 
			 *
			 * The dtmf is automatically played to the user. 
			 * @param dtmf The dtmf name specified as a char, such as '0', '#' etc... 
			 * @return 0 if successful, -1 on error. 
			 */
			LINPHONECXX_PUBLIC linphone::Status sendDtmf(char dtmf);
			
			/**
			 * @brief Send a list of dtmf. 
			 *
			 * The dtmfs are automatically sent to remote, separated by some needed
			 * customizable delay. Sending is canceled if the call state changes to something
			 * not LinphoneCallStreamsRunning. 
			 * @param dtmfs A dtmf sequence such as '123#123123'   
			 * @return -2 if there is already a DTMF sequence, -1 if call is not ready, 0
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC linphone::Status sendDtmfs(const std::string & dtmfs);
			
			/**
			 * @brief Sends an info message through an established call. 
			 *
			 * @param info the @ref InfoMessage to send   
			 */
			LINPHONECXX_PUBLIC linphone::Status sendInfoMessage(const std::shared_ptr<const linphone::InfoMessage> & info);
			
			/**
			 * @brief Request remote side to send us a Video Fast Update. 
			 *
			 */
			LINPHONECXX_PUBLIC void sendVfuRequest();
			
			/**
			 * @brief Starts call recording. 
			 *
			 * Video record is only available if this function is called in state
			 * StreamRunning. The output file where audio is recorded must be previously
			 * specified with CallParams::setRecordFile(). 
			 */
			LINPHONECXX_PUBLIC void startRecording();
			
			/**
			 * @brief Stops call recording. 
			 *
			 */
			LINPHONECXX_PUBLIC void stopRecording();
			
			/**
			 * @brief Take a photo of currently captured video and write it into a jpeg file. 
			 *
			 * Note that the snapshot is asynchronous, an application shall not assume that
			 * the file is created when the function returns. 
			 * @param filePath a path where to write the jpeg content.   
			 * @return 0 if successfull, -1 otherwise (typically if jpeg format is not
			 * supported). 
			 */
			LINPHONECXX_PUBLIC linphone::Status takePreviewSnapshot(const std::string & filePath);
			
			/**
			 * @brief Take a photo of currently received video and write it into a jpeg file. 
			 *
			 * Note that the snapshot is asynchronous, an application shall not assume that
			 * the file is created when the function returns. 
			 * @param filePath a path where to write the jpeg content.   
			 * @return 0 if successfull, -1 otherwise (typically if jpeg format is not
			 * supported). 
			 */
			LINPHONECXX_PUBLIC linphone::Status takeVideoSnapshot(const std::string & filePath);
			
			/**
			 * @brief Terminates a call. 
			 *
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status terminate();
			
			/**
			 * @brief Terminates a call. 
			 *
			 * @param ei @ref ErrorInfo   
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status terminateWithErrorInfo(const std::shared_ptr<const linphone::ErrorInfo> & ei);
			
			/**
			 * @brief Performs a simple call transfer to the specified destination. 
			 *
			 * The remote endpoint is expected to issue a new call to the specified
			 * destination. The current call remains active and thus can be later paused or
			 * terminated. It is possible to follow the progress of the transfer provided that
			 * transferee sends notification about it. In this case, the
			 * transfer_state_changed callback of the LinphoneCoreVTable is invoked to notify
			 * of the state of the new call at the other party. The notified states are
			 * #LinphoneCallOutgoingInit , #LinphoneCallOutgoingProgress,
			 * #LinphoneCallOutgoingRinging and #LinphoneCallConnected. 
			 * @param referTo The destination the call is to be refered to.   
			 * @return 0 on success, -1 on failure 
			 * @deprecated 27/10/2020. Use transferTo() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED linphone::Status transfer(const std::string & referTo);
			
			/**
			 * @brief Performs a simple call transfer to the specified destination. 
			 *
			 * The remote endpoint is expected to issue a new call to the specified
			 * destination. The current call remains active and thus can be later paused or
			 * terminated. It is possible to follow the progress of the transfer provided that
			 * transferee sends notification about it. In this case, the
			 * transfer_state_changed callback of the LinphoneCoreVTable is invoked to notify
			 * of the state of the new call at the other party. The notified states are
			 * #LinphoneCallOutgoingInit , #LinphoneCallOutgoingProgress,
			 * #LinphoneCallOutgoingRinging and #LinphoneCallConnected. 
			 * @param referTo The @ref Address the call is to be refered to.   
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status transferTo(const std::shared_ptr<const linphone::Address> & referTo);
			
			/**
			 * @brief Transfers a call to destination of another running call. 
			 *
			 * This is used for "attended transfer" scenarios. The transfered call is supposed
			 * to be in paused state, so that it is able to accept the transfer immediately.
			 * The destination call is a call previously established to introduce the
			 * transfered person. This method will send a transfer request to the transfered
			 * person. The phone of the transfered is then expected to automatically call to
			 * the destination of the transfer. The receiver of the transfer will then
			 * automatically close the call with us (the 'dest' call). It is possible to
			 * follow the progress of the transfer provided that transferee sends notification
			 * about it. In this case, the transfer_state_changed callback of the
			 * LinphoneCoreVTable is invoked to notify of the state of the new call at the
			 * other party. The notified states are #LinphoneCallOutgoingInit ,
			 * #LinphoneCallOutgoingProgress, #LinphoneCallOutgoingRinging and
			 * #LinphoneCallConnected. 
			 * @param dest A running call whose remote person will receive the transfer   
			 * @return 0 on success, -1 on failure 
			 */
			LINPHONECXX_PUBLIC linphone::Status transferToAnother(const std::shared_ptr<linphone::Call> & dest);
			
			/**
			 * @brief Updates a running call according to supplied call parameters or
			 * parameters changed in the LinphoneCore. 
			 *
			 * It triggers a SIP reINVITE in order to perform a new offer/answer of media
			 * capabilities. Changing the size of the transmitted video after calling
			 * linphone_core_set_preferred_video_size can be used by passing nullptr as params
			 * argument. In case no changes are requested through the @ref CallParams
			 * argument, then this argument can be omitted and set to nullptr. WARNING:
			 * Updating a call in the #LinphoneCallPaused state will still result in a paused
			 * call even if the media directions set in the params are sendrecv. To resume a
			 * paused call, you need to call resume().
			 * @param params The new call parameters to use (may be nullptr).   
			 * @return 0 if successful, -1 otherwise. 
			 */
			LINPHONECXX_PUBLIC linphone::Status update(const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Perform a zoom of the video displayed during a call. 
			 *
			 * The zoom ensures that all the screen is fullfilled with the video. 
			 * @param cy a floating point number pointing the vertical center of the zoom to
			 * be applied. This value should be between 0.0 and 1.0. 
			 */
			LINPHONECXX_PUBLIC void zoom(float zoomFactor, float cx, float cy);
			
			
		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_CALL_HH
