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

#ifndef _LINPHONE_CORE_HH
#define _LINPHONE_CORE_HH

#include <string>
#include <list>
#include "enums.hh"
#include "call.hh"
#include "presence_activity.hh"
#include "object.hh"




struct _LinphoneCore;


namespace linphone {

	class AudioDevice;
	class PayloadType;
	class Range;
	class AuthInfo;
	class CallLog;
	class Call;
	class ChatRoom;
	class Conference;
	class Config;
	class Address;
	class VideoDefinition;
	class FriendList;
	class ProxyConfig;
	class ImNotifPolicy;
	class NatPolicy;
	class PresenceModel;
	class Transports;
	class Tunnel;
	class VideoActivationPolicy;
	class AccountCreator;
	class CallParams;
	class ChatRoomParams;
	class ConferenceParams;
	class Content;
	class Friend;
	class InfoMessage;
	class Player;
	class MagicSearch;
	class Event;
	class PresenceActivity;
	class PresenceNote;
	class PresencePerson;
	class PresenceService;
	class XmlRpcSession;
	class CoreListener;

	/**
	 * @brief Main object to instanciate and on which to keep a reference. 
	 *
	 * This object is the first object to instanciante, and will allow you to perform
	 * all kind of tasks. To create it, use either Factory::createCore() or
	 * Factory::createCoreWithConfig(), see @ref Config for more information about
	 * factory and default config. On some platforms like Android or iOS you will need
	 * to give it the Context of your application.
	 * Once the @ref Core is in state @ref GlobalState::Ready, use start(). It will
	 * then go to state @ref GlobalState::On and from that you can start using it for
	 * calls and chat messages. It is recommended to add a LinphoneCoreCbs listener
	 * using linphone_core_add_listener to it to monitor different events.
	 * To be able to receive events from the network, you must schedule a call
	 * iterate() often, like every 20ms. On Android & iOS isAutoIterateEnabled() is
	 * enabled by default so you don't have to worry about that unless you disable it
	 * using setAutoIterateEnabled() or by setting in the [misc] section of your
	 * configuration auto_iterate=0. @warning Our API isn't thread-safe but also isn't
	 * blocking, so it is strongly recommend to always call our methods from the main
	 * thread.
	 * Once you don't need it anymore, call stop() and release the reference on it so
	 * it can gracefully shutdown. 
	 */
	class Core: public MultiListenableObject {
		friend class Factory;
	
		public:
			/**
			* Used to notify if log collection upload have been succesfully delivered or not. 
			*
			*/
			enum class LogCollectionUploadState {
				/**
				* Delivery in progress. 
				*/
				InProgress,
				/**
				* Log collection upload successfully delivered and acknowledged by remote end
				* point. 
				*/
				Delivered,
				/**
				* Log collection upload was not delivered. 
				*/
				NotDelivered
			};


			Core(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneCore *cPtr() {return (_LinphoneCore *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<CoreListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<CoreListener> &listener);
	
		public:
			
		
			/**
			 * @brief Returns which adaptive rate algorithm is currently configured for future
			 * calls. 
			 *
			 * @see setAdaptiveRateAlgorithm() 
			 * @return the adaptive rate algorithm. Currently two values are supported:
			 * 'advanced', which is the default value, or 'basic'.   
			 */
			LINPHONECXX_PUBLIC std::string getAdaptiveRateAlgorithm() const;
			
			/**
			 * @brief Sets adaptive rate algorithm. 
			 *
			 * It will be used for each new calls starting from now. Calls already started
			 * will not be updated. 
			 * @param algorithm the adaptive rate control algorithm. Currently two values are
			 * supported: 'advanced', which is the default value, or 'basic'.   
			 */
			LINPHONECXX_PUBLIC void setAdaptiveRateAlgorithm(const std::string & algorithm);
			
			/**
			 * @brief Returns whether adaptive rate control is enabled. 
			 *
			 * @see enableAdaptiveRateControl()
			 * @return true if adaptive rate control is enabled, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool adaptiveRateControlEnabled() const;
			
			/**
			 * @brief Enable adaptive rate control. 
			 *
			 * Adaptive rate control consists in using RTCP feedback provided information to
			 * dynamically control the output bitrate of the audio and video encoders, so that
			 * we can adapt to the network conditions and available bandwidth. Control of the
			 * audio encoder is done in case of audio-only call, and control of the video
			 * encoder is done for audio & video calls. Adaptive rate control feature is
			 * enabled by default.
			 * @param enabled true to enable adaptive rate control, false otherwise 
			 */
			LINPHONECXX_PUBLIC void enableAdaptiveRateControl(bool enabled);
			
			/**
			 * @brief Tells whether the audio adaptive jitter compensation is enabled. 
			 *
			 * @return true if the audio adaptive jitter compensation is enabled, false
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC bool audioAdaptiveJittcompEnabled();
			
			/**
			 * @brief Enable or disable the audio adaptive jitter compensation. 
			 *
			 * @param enable true to enable the audio adaptive jitter compensation, false to
			 * disable it. 
			 */
			LINPHONECXX_PUBLIC void enableAudioAdaptiveJittcomp(bool enable);
			
			/**
			 * @brief Returns a list of audio devices, with only the first device for each
			 * type To have the list of all audio devices, use getExtendedAudioDevices() 
			 *
			 * @return   A list with the first @ref AudioDevice of each type     
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::AudioDevice>> getAudioDevices() const;
			
			/**
			 * @brief Get the DSCP field for outgoing audio streams. 
			 *
			 * The DSCP defines the quality of service in IP packets. 
			 * @return The current DSCP value 
			 */
			LINPHONECXX_PUBLIC int getAudioDscp() const;
			
			/**
			 * @brief Set the DSCP field for outgoing audio streams. 
			 *
			 * The DSCP defines the quality of service in IP packets. 
			 * @param dscp The DSCP value to set 
			 */
			LINPHONECXX_PUBLIC void setAudioDscp(int dscp);
			
			/**
			 * @brief Returns the nominal audio jitter buffer size in milliseconds. 
			 *
			 * @return The nominal audio jitter buffer size in milliseconds 
			 */
			LINPHONECXX_PUBLIC int getAudioJittcomp();
			
			/**
			 * @brief Sets the nominal audio jitter buffer size in milliseconds. 
			 *
			 * The value takes effect immediately for all running and pending calls, if any. A
			 * value of 0 disables the jitter buffer.
			 * @param milliseconds the audio jitter buffer size to set in milliseconds 
			 */
			LINPHONECXX_PUBLIC void setAudioJittcomp(int milliseconds);
			
			/**
			 * @brief Use to get multicast address to be used for audio stream. 
			 *
			 * @return an ipv4/6 multicast address or default value.   
			 */
			LINPHONECXX_PUBLIC std::string getAudioMulticastAddr() const;
			
			/**
			 * @brief Use to set multicast address to be used for audio stream. 
			 *
			 * @param ip an ipv4/6 multicast address.   
			 * @return 0 in case of success 
			 */
			LINPHONECXX_PUBLIC linphone::Status setAudioMulticastAddr(const std::string & ip);
			
			/**
			 * @brief Use to get multicast state of audio stream. 
			 *
			 * @return true if subsequent calls will propose multicast ip set by
			 * setAudioMulticastAddr() 
			 */
			LINPHONECXX_PUBLIC bool audioMulticastEnabled() const;
			
			/**
			 * @brief Use to enable multicast rtp for audio stream. 
			 *
			 * If enabled, outgoing calls put a multicast address from getVideoMulticastAddr()
			 * into audio cline. In case of outgoing call audio stream is sent to this
			 * multicast address. For incoming calls behavior is unchanged. 
			 * @param yesno if yes, subsequent calls will propose multicast ip set by
			 * setAudioMulticastAddr() 
			 */
			LINPHONECXX_PUBLIC void enableAudioMulticast(bool yesno);
			
			/**
			 * @brief Use to get multicast ttl to be used for audio stream. 
			 *
			 * @return a time to leave value 
			 */
			LINPHONECXX_PUBLIC int getAudioMulticastTtl() const;
			
			/**
			 * @brief Use to set multicast ttl to be used for audio stream. 
			 *
			 * @param ttl value or -1 if not used. [0..255] default value is 1 
			 * @return 0 in case of success 
			 */
			LINPHONECXX_PUBLIC linphone::Status setAudioMulticastTtl(int ttl);
			
			/**
			 * @brief Return the list of the available audio payload types. 
			 *
			 * @return A freshly allocated list of the available payload types. The list must
			 * be destroyed with bctbx_list_free() after usage. The elements of the list
			 * haven't to be unref.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::PayloadType>> getAudioPayloadTypes();
			
			/**
			 * @brief Redefine the list of the available payload types. 
			 *
			 * @param payloadTypes The new list of payload types. The core does not take
			 * ownership on it.    
			 */
			LINPHONECXX_PUBLIC void setAudioPayloadTypes(const std::list<std::shared_ptr<linphone::PayloadType>> & payloadTypes);
			
			/**
			 * @brief Gets the UDP port used for audio streaming. 
			 *
			 * @return The UDP port used for audio streaming 
			 */
			LINPHONECXX_PUBLIC int getAudioPort() const;
			
			/**
			 * @brief Sets the UDP port used for audio streaming. 
			 *
			 * A value of -1 will request the system to allocate the local port randomly. This
			 * is recommended in order to avoid firewall warnings. 
			 * @param port The UDP port to use for audio streaming 
			 */
			LINPHONECXX_PUBLIC void setAudioPort(int port);
			
			/**
			 * @brief Get the audio port range from which is randomly chosen the UDP port used
			 * for audio streaming. 
			 *
			 * @return a @ref Range object     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Range> getAudioPortsRange() const;
			
			/**
			 * @brief Returns an unmodifiable list of currently entered @ref AuthInfo. 
			 *
			 * @return A list of @ref AuthInfo.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::AuthInfo>> getAuthInfoList() const;
			
			/**
			 * @brief Enable or disable the automatic schedule of iterate() method on Android
			 * & iOS. 
			 *
			 * If enabled, iterate() will be called on the main thread every 20ms
			 * automatically. If disabled, it is the application that must do this job. 
			 * @param enable true to enable auto iterate, false to disable 
			 */
			LINPHONECXX_PUBLIC void setAutoIterateEnabled(bool enable);
			
			/**
			 * @brief Return AVPF enablement. 
			 *
			 * See setAvpfMode() . 
			 * @return The current @ref AVPFMode mode 
			 */
			LINPHONECXX_PUBLIC linphone::AVPFMode getAvpfMode() const;
			
			/**
			 * @brief Enable RTCP feedback (also known as RTP/AVPF profile). 
			 *
			 * Setting @ref AVPFMode::Default is equivalent to LinphoneAVPFDisabled. This
			 * setting can be overriden per @ref ProxyConfig with ProxyConfig::setAvpfMode().
			 * The value set here is used for calls placed or received out of any proxy
			 * configured, or if the proxy config is configured with LinphoneAVPFDefault. 
			 * @param mode The AVPF mode to use. 
			 */
			LINPHONECXX_PUBLIC void setAvpfMode(linphone::AVPFMode mode);
			
			/**
			 * @brief Return the avpf report interval in seconds. 
			 *
			 * @return The current AVPF report interval in seconds 
			 */
			LINPHONECXX_PUBLIC int getAvpfRrInterval() const;
			
			/**
			 * @brief Set the avpf report interval in seconds. 
			 *
			 * This value can be overriden by the proxy config using
			 * ProxyConfig::setAvpfRrInterval(). 
			 * @param interval The report interval in seconds 
			 */
			LINPHONECXX_PUBLIC void setAvpfRrInterval(int interval);
			
			/**
			 * @brief Get the list of call logs (past calls). 
			 *
			 * @return A list of @ref CallLog.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::CallLog>> getCallLogs();
			
			/**
			 * @brief Gets the database filename where call logs will be stored. 
			 *
			 * @return filesystem path.   
			 */
			LINPHONECXX_PUBLIC std::string getCallLogsDatabasePath();
			
			/**
			 * @brief Sets the database filename where call logs will be stored. 
			 *
			 * If the file does not exist, it will be created.
			 * @param path filesystem path   
			 */
			LINPHONECXX_PUBLIC void setCallLogsDatabasePath(const std::string & path);
			
			/**
			 * @brief Special function to check if the callkit is enabled, False by default. 
			 *
			 * @return true if callkit is enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool callkitEnabled() const;
			
			/**
			 * @brief Special function to enable the callkit. 
			 *
			 * @param enabled true to enable callkit, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableCallkit(bool enabled);
			
			/**
			 * @brief Gets the current list of calls. 
			 *
			 * Note that this list is read-only and might be changed by the core after a
			 * function call to iterate(). Similarly the @ref Call objects inside it might be
			 * destroyed without prior notice. To hold references to @ref Call object into
			 * your program, you must use linphone_call_ref. 
			 * @return A list of @ref Call    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Call>> getCalls();
			
			/**
			 * @brief Get the number of Call. 
			 *
			 * @return The current number of calls 
			 */
			LINPHONECXX_PUBLIC int getCallsNb() const;
			
			/**
			 * @brief Get the camera sensor rotation. 
			 *
			 * This is needed on some mobile platforms to get the number of degrees the camera
			 * sensor is rotated relative to the screen. 
			 * @return The camera sensor rotation in degrees (0 to 360) or -1 if it could not
			 * be retrieved 
			 */
			LINPHONECXX_PUBLIC int getCameraSensorRotation();
			
			/**
			 * @brief Gets the name of the currently assigned sound device for capture. 
			 *
			 * @return The name of the currently assigned sound device for capture.   
			 */
			LINPHONECXX_PUBLIC std::string getCaptureDevice();
			
			/**
			 * @brief Sets the sound device used for capture. 
			 *
			 * @param devid The device name as returned by linphone_core_get_sound_devices   
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status setCaptureDevice(const std::string & devid);
			
			/**
			 * @brief Returns whether chat is enabled. 
			 *
			 * @return true if chat is enabled, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool chatEnabled() const;
			
			/**
			 * @brief Returns a list of chat rooms. 
			 *
			 * @return List of chat rooms.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::ChatRoom>> getChatRooms();
			
			/**
			 * @brief Get a pointer on the internal conference object. 
			 *
			 * @return A pointer on @ref Conference or nullptr if no conference are going on. 
			 *  
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Conference> getConference();
			
			/**
			 * @brief Get the set input volume of the local participant. 
			 *
			 * @return A value inside [0.0 ; 1.0] 
			 */
			LINPHONECXX_PUBLIC float getConferenceLocalInputVolume();
			
			/**
			 * @brief Tells whether the conference server feature is enabled. 
			 *
			 * @return A boolean value telling whether the conference server feature is
			 * enabled or not 
			 */
			LINPHONECXX_PUBLIC bool conferenceServerEnabled() const;
			
			/**
			 * @brief Enable the conference server feature. 
			 *
			 * This has the effect to listen of the conference factory uri to create new
			 * conferences when receiving INVITE messages there. 
			 * @param enable A boolean value telling whether to enable or disable the
			 * conference server feature 
			 */
			LINPHONECXX_PUBLIC void enableConferenceServer(bool enable);
			
			/**
			 * @brief Get the number of participant in the running conference. 
			 *
			 * The local participant is included in the count only if it is in the conference. 
			 * @return The number of participant 
			 */
			LINPHONECXX_PUBLIC int getConferenceSize();
			
			/**
			 * @brief Returns the config object used to manage the storage (config) file. 
			 *
			 * @return a @ref Config object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Config> getConfig() const;
			
			/**
			 * @brief Get my consolidated presence. 
			 *
			 * @return My @ref ConsolidatedPresence presence 
			 */
			LINPHONECXX_PUBLIC linphone::ConsolidatedPresence getConsolidatedPresence() const;
			
			/**
			 * @brief Set my consolidated presence. 
			 *
			 * @param presence @ref ConsolidatedPresence value 
			 */
			LINPHONECXX_PUBLIC void setConsolidatedPresence(linphone::ConsolidatedPresence presence);
			
			/**
			 * @brief Gets the current call. 
			 *
			 * @return The current call or nullptr if no call is running.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getCurrentCall() const;
			
			/**
			 * @brief Get the remote address of the current call. 
			 *
			 * @return The remote address of the current call or nullptr if there is no
			 * current call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::Address> getCurrentCallRemoteAddress();
			
			/**
			 * @brief Get the effective video definition provided by the camera for the
			 * captured video. 
			 *
			 * When preview is disabled or not yet started this function returns a 0x0 video
			 * definition. 
			 * @return The captured @ref VideoDefinition.  
			 * @see setPreviewVideoDefinition() 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::VideoDefinition> getCurrentPreviewVideoDefinition() const;
			
			/**
			 * @brief Retrieves the first list of @ref Friend from the core. 
			 *
			 * @return the first @ref FriendList object or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::FriendList> getDefaultFriendList() const;
			
			/**
			 * @brief Gets the default input audio device. 
			 *
			 * @return The default input audio device   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AudioDevice> getDefaultInputAudioDevice() const;
			
			/**
			 * @brief Sets the given @ref AudioDevice as default input for next calls. 
			 *
			 * @param audioDevice The @ref AudioDevice   
			 */
			LINPHONECXX_PUBLIC void setDefaultInputAudioDevice(const std::shared_ptr<linphone::AudioDevice> & audioDevice);
			
			/**
			 * @brief Gets the default output audio device. 
			 *
			 * @return The default output audio device   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AudioDevice> getDefaultOutputAudioDevice() const;
			
			/**
			 * @brief Sets the given @ref AudioDevice as default output for next calls. 
			 *
			 * @param audioDevice The @ref AudioDevice   
			 */
			LINPHONECXX_PUBLIC void setDefaultOutputAudioDevice(const std::shared_ptr<linphone::AudioDevice> & audioDevice);
			
			/**
			 * @brief Returns the default proxy configuration, that is the one used to
			 * determine the current identity. 
			 *
			 * @return The default proxy configuration.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ProxyConfig> getDefaultProxyConfig() const;
			
			/**
			 * @brief Sets the default proxy. 
			 *
			 * This default proxy must be part of the list of already entered
			 * LinphoneProxyConfig. Toggling it as default will make @ref Core use the
			 * identity associated with the proxy configuration in all incoming and outgoing
			 * calls. 
			 * @param config The proxy configuration to use as the default one.   
			 */
			LINPHONECXX_PUBLIC void setDefaultProxyConfig(const std::shared_ptr<linphone::ProxyConfig> & config);
			
			/**
			 * @brief Get the name of the default mediastreamer2 filter used for rendering
			 * video on the current platform. 
			 *
			 * This is for advanced users of the library, mainly to expose mediastreamer video
			 * filter name and status. 
			 * @return The default video display filter.   
			 */
			LINPHONECXX_PUBLIC std::string getDefaultVideoDisplayFilter();
			
			/**
			 * @brief Gets the delayed timeout See setDelayedTimeout() for details. 
			 *
			 * @return The current delayed timeout in seconds 
			 */
			LINPHONECXX_PUBLIC int getDelayedTimeout();
			
			/**
			 * @brief Set the in delayed timeout in seconds. 
			 *
			 * After this timeout period, a delayed call (internal call initialisation or
			 * resolution) is resumed. 
			 * @param seconds The new delayed timeout 
			 */
			LINPHONECXX_PUBLIC void setDelayedTimeout(int seconds);
			
			/**
			 * @brief Gets the current device orientation. 
			 *
			 * @return The current device orientation
			 * @see setDeviceRotation() 
			 */
			LINPHONECXX_PUBLIC int getDeviceRotation();
			
			/**
			 * @brief Tells the core the device current orientation. 
			 *
			 * This can be used by capture filters on mobile devices to select between
			 * portrait/landscape mode and to produce properly oriented images. The exact
			 * meaning of the value in rotation if left to each device specific
			 * implementations. IOS supported values are 0 for UIInterfaceOrientationPortrait
			 * and 270 for UIInterfaceOrientationLandscapeRight. 
			 * @param rotation The orientation to use 
			 */
			LINPHONECXX_PUBLIC void setDeviceRotation(int rotation);
			
			/**
			 * @brief Get whether the microphone will be completely deactivated when muted. 
			 *
			 * Please refer to setDisableRecordOnMute().
			 * @return True if you wish to entirely stop the audio recording when muting the
			 * microphone. 
			 */
			LINPHONECXX_PUBLIC bool getDisableRecordOnMute();
			
			/**
			 * @brief Calling this method with disable=true will cause the microhone to be
			 * completely deactivated when muted, when given possible by the implementation on
			 * the platform on which liblinphone is running. 
			 *
			 * Otherwise, sound recording remains active but silence is sent instead of
			 * recorded audio. Playback sound will be briefly interrupted while the audio is
			 * reconfigured. Currently only implemented for IOS, it will also disable Apple's
			 * microphone recording indicator when microphone is muted.
			 * @param disable True if you wish to entirely stop the audio recording when
			 * muting the microphone. 
			 */
			LINPHONECXX_PUBLIC void setDisableRecordOnMute(bool disable);
			
			/**
			 * @brief Tells whether DNS search (use of local domain if the fully qualified
			 * name did return results) is enabled. 
			 *
			 * @return true if DNS search is enabled, false if disabled. 
			 */
			LINPHONECXX_PUBLIC bool dnsSearchEnabled() const;
			
			/**
			 * @brief Enable or disable DNS search (use of local domain if the fully qualified
			 * name did return results). 
			 *
			 * @param enable true to enable DNS search, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableDnsSearch(bool enable);
			
			/**
			 * @brief Forces liblinphone to use the supplied list of dns servers, instead of
			 * system's ones. 
			 *
			 * @param servers A list of strings containing the IP addresses of DNS servers to
			 * be used.  Setting to nullptr restores default behaviour, which is to use the
			 * DNS server list provided by the system. The list is copied internally.   
			 */
			LINPHONECXX_PUBLIC void setDnsServers(const std::list<std::string> & servers);
			
			/**
			 * @brief Forces liblinphone to use the supplied list of dns servers, instead of
			 * system's ones and set dns_set_by_app at true or false according to value of
			 * servers list. 
			 *
			 * @param servers A list of strings containing the IP addresses of DNS servers to
			 * be used.  Setting to nullptr restores default behaviour, which is to use the
			 * DNS server list provided by the system. The list is copied internally.   
			 */
			LINPHONECXX_PUBLIC void setDnsServersApp(const std::list<std::string> & servers);
			
			/**
			 * @brief Tells if the DNS was set by an application. 
			 *
			 * @return true if DNS was set by app, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getDnsSetByApp();
			
			/**
			 * @brief Tells whether DNS SRV resolution is enabled. 
			 *
			 * @return true if DNS SRV resolution is enabled, false if disabled. 
			 */
			LINPHONECXX_PUBLIC bool dnsSrvEnabled() const;
			
			/**
			 * @brief Enable or disable DNS SRV resolution. 
			 *
			 * @param enable true to enable DNS SRV resolution, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableDnsSrv(bool enable);
			
			/**
			 * @brief Retrieve the maximum available download bandwidth. 
			 *
			 * This value was set by setDownloadBandwidth(). 
			 * @return the download bandiwdth in kbits/s, 0 for infinite 
			 */
			LINPHONECXX_PUBLIC int getDownloadBandwidth() const;
			
			/**
			 * @brief Sets maximum available download bandwidth This is IP bandwidth, in
			 * kbit/s. 
			 *
			 * This information is used signaled to other parties during calls (within SDP
			 * messages) so that the remote end can have sufficient knowledge to properly
			 * configure its audio & video codec output bitrate to not overflow available
			 * bandwidth.
			 * @param bandwidth the bandwidth in kbits/s, 0 for infinite 
			 */
			LINPHONECXX_PUBLIC void setDownloadBandwidth(int bandwidth);
			
			/**
			 * @brief Get audio packetization time linphone expects to receive from peer. 
			 *
			 * A value of zero means that ptime is not specified. 
			 * @return the download packetization time set 
			 */
			LINPHONECXX_PUBLIC int getDownloadPtime();
			
			/**
			 * @brief Set audio packetization time linphone expects to receive from peer. 
			 *
			 * A value of zero means that ptime is not specified. 
			 * @param ptime the download packetization time to set 
			 */
			LINPHONECXX_PUBLIC void setDownloadPtime(int ptime);
			
			/**
			 * @brief Returns true if echo cancellation is enabled. 
			 *
			 * @return A boolean value telling whether echo cancellation is enabled or
			 * disabled 
			 */
			LINPHONECXX_PUBLIC bool echoCancellationEnabled() const;
			
			/**
			 * @brief Enables or disable echo cancellation. 
			 *
			 * Value is saved and used for subsequent calls. This actually controls software
			 * echo cancellation. If hardware echo cancellation is available, it will be
			 * always used and activated for calls, regardless of the value passed to this
			 * function. When hardware echo cancellation is available, the software one is of
			 * course not activated. 
			 * @param enable A boolean value telling whether echo cancellation is to be
			 * enabled or disabled. 
			 */
			LINPHONECXX_PUBLIC void enableEchoCancellation(bool enable);
			
			/**
			 * @brief Get the name of the mediastreamer2 filter used for echo cancelling. 
			 *
			 * @return The name of the mediastreamer2 filter used for echo cancelling.   
			 */
			LINPHONECXX_PUBLIC std::string getEchoCancellerFilterName() const;
			
			/**
			 * @brief Set the name of the mediastreamer2 filter to be used for echo
			 * cancelling. 
			 *
			 * This is for advanced users of the library. 
			 * @param filtername The name of the mediastreamer2 filter to be used for echo
			 * cancelling.   
			 */
			LINPHONECXX_PUBLIC void setEchoCancellerFilterName(const std::string & filtername);
			
			/**
			 * @brief Tells whether echo limiter is enabled. 
			 *
			 * @return true if the echo limiter is enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool echoLimiterEnabled() const;
			
			/**
			 * @brief Enables or disable echo limiter. 
			 *
			 * @param enable true to enable echo limiter, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableEchoLimiter(bool enable);
			
			/**
			 * @brief Enable or disable the UPDATE method support. 
			 *
			 * @param value Enable or disable it 
			 */
			LINPHONECXX_PUBLIC void setEnableSipUpdate(int value) const;
			
			/**
			 * @brief Sets expected available upload bandwidth This is IP bandwidth, in
			 * kbit/s. 
			 *
			 * This information is used by liblinphone together with remote side available
			 * bandwidth signaled in SDP messages to properly configure audio & video codec's
			 * output bitrate.
			 * @param bandwidth the bandwidth in kbits/s, 0 for infinite 
			 */
			LINPHONECXX_PUBLIC void setExpectedBandwidth(int bandwidth);
			
			/**
			 * @brief Returns the list of all audio devices. 
			 *
			 * @return   A list of all @ref AudioDevice     
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::AudioDevice>> getExtendedAudioDevices() const;
			
			/**
			 * @brief Get the globaly set http file transfer server to be used for content
			 * type application/vnd.gsma.rcs-ft-http+xml. 
			 *
			 * Url may be like: "https://file.linphone.org/upload.php". 
			 * @return URL of the file server.   
			 */
			LINPHONECXX_PUBLIC std::string getFileTransferServer();
			
			/**
			 * @brief Globaly set an http file transfer server to be used for content type
			 * application/vnd.gsma.rcs-ft-http+xml. 
			 *
			 * Url may be like: "https://file.linphone.org/upload.php". This value can also be
			 * set for a dedicated account using
			 * linphone_proxy_config_set_file_transfer_server. 
			 * @param serverUrl URL of the file server.   
			 */
			LINPHONECXX_PUBLIC void setFileTransferServer(const std::string & serverUrl);
			
			/**
			 * @brief Indicates whether the ICE relay path is forcibly selected. 
			 *
			 * @return a boolean value indicating whether forced relay is enabled.
			 * @see enableForcedIceRelay(). 
			 */
			LINPHONECXX_PUBLIC bool forcedIceRelayEnabled() const;
			
			/**
			 * @brief Artificially cause the relay path to be selected when ICE is used. 
			 *
			 * This is mainly a function for test, for example to validate that the relay
			 * service (ever TURN or media-aware SIP proxy) is working as expected. Indeed, in
			 * many cases a path through host or server reflexive candidate will be found by
			 * ICE, which makes difficult to make sure that the relay service is working as
			 * expected. 
			 * @param enable boolean value 
			 */
			LINPHONECXX_PUBLIC void enableForcedIceRelay(bool enable);
			
			/**
			 * @brief Sets whether or not to start friend lists subscription when in
			 * foreground. 
			 *
			 * @param enable whether or not to enable the feature 
			 */
			LINPHONECXX_PUBLIC void enableFriendListSubscription(bool enable);
			
			/**
			 * @brief Gets the database filename where friends will be stored. 
			 *
			 * @return filesystem path.   
			 */
			LINPHONECXX_PUBLIC std::string getFriendsDatabasePath();
			
			/**
			 * @brief Sets the database filename where friends will be stored. 
			 *
			 * If the file does not exist, it will be created.
			 * @param path filesystem path.   
			 */
			LINPHONECXX_PUBLIC void setFriendsDatabasePath(const std::string & path);
			
			/**
			 * @brief Retrieves the list of @ref FriendList from the core. 
			 *
			 * @return A list of @ref FriendList.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::FriendList>> getFriendsLists() const;
			
			/**
			 * @brief Returns the global state of core. 
			 *
			 * @return a @ref GlobalState enum.   
			 */
			LINPHONECXX_PUBLIC linphone::GlobalState getGlobalState() const;
			
			/**
			 * @brief Returns true if hostname part of primary contact is guessed
			 * automatically. 
			 *
			 * @return true if guess hostname enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getGuessHostname();
			
			/**
			 * @brief Tells @ref Core to guess local hostname automatically in primary
			 * contact. 
			 *
			 * @param enable whether to enable the guess hostname feature or not 
			 */
			LINPHONECXX_PUBLIC void setGuessHostname(bool enable);
			
			/**
			 * @brief Get http proxy address to be used for signaling. 
			 *
			 * @return hostname of IP adress of the http proxy (can be nullptr to disable).   
			 */
			LINPHONECXX_PUBLIC std::string getHttpProxyHost() const;
			
			/**
			 * @brief Set http proxy address to be used for signaling during next channel
			 * connection. 
			 *
			 * Use setNetworkReachable() FASLE/true to force channel restart. 
			 * @param host Hostname of IP adress of the http proxy (can be nullptr to
			 * disable).   
			 */
			LINPHONECXX_PUBLIC void setHttpProxyHost(const std::string & host);
			
			/**
			 * @brief Get http proxy port to be used for signaling. 
			 *
			 * @return port of the http proxy. 
			 */
			LINPHONECXX_PUBLIC int getHttpProxyPort() const;
			
			/**
			 * @brief Set http proxy port to be used for signaling. 
			 *
			 * @param port of the http proxy. 
			 */
			LINPHONECXX_PUBLIC void setHttpProxyPort(int port);
			
			/**
			 * @brief Gets the default identity SIP address. 
			 *
			 * This is an helper function. If no default proxy is set, this will return the
			 * primary contact ( see getPrimaryContact() ). If a default proxy is set it
			 * returns the registered identity on the proxy. 
			 * @return The default identity SIP address.   
			 */
			LINPHONECXX_PUBLIC std::string getIdentity();
			
			/**
			 * @brief Get the @ref ImNotifPolicy object controlling the instant messaging
			 * notifications. 
			 *
			 * @return A @ref ImNotifPolicy object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ImNotifPolicy> getImNotifPolicy() const;
			
			/**
			 * @brief Gets the in call timeout See setInCallTimeout() for details. 
			 *
			 * @return The current in call timeout in seconds 
			 */
			LINPHONECXX_PUBLIC int getInCallTimeout();
			
			/**
			 * @brief Set the in call timeout in seconds. 
			 *
			 * After this timeout period, the call is automatically hangup. 
			 * @param seconds The new timeout in seconds 
			 */
			LINPHONECXX_PUBLIC void setInCallTimeout(int seconds);
			
			/**
			 * @brief Returns the incoming call timeout See setIncTimeout() for details. 
			 *
			 * @return The current incoming call timeout in seconds 
			 */
			LINPHONECXX_PUBLIC int getIncTimeout();
			
			/**
			 * @brief Set the incoming call timeout in seconds. 
			 *
			 * If an incoming call isn't answered for this timeout period, it is automatically
			 * declined. 
			 * @param seconds The new timeout in seconds 
			 */
			LINPHONECXX_PUBLIC void setIncTimeout(int seconds);
			
			/**
			 * @brief Gets the input audio device for the current call. 
			 *
			 * @return The input audio device for the current or first call, nullptr if there
			 * is no call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AudioDevice> getInputAudioDevice() const;
			
			/**
			 * @brief Sets the given @ref AudioDevice as input for all active calls. 
			 *
			 * @param audioDevice The @ref AudioDevice. nullptr does nothing.   
			 */
			LINPHONECXX_PUBLIC void setInputAudioDevice(const std::shared_ptr<linphone::AudioDevice> & audioDevice);
			
			/**
			 * @brief Tells whether IPv6 is enabled or not. 
			 *
			 * @return A boolean value telling whether IPv6 is enabled or not
			 * @see enableIpv6() for more details on how IPv6 is supported in liblinphone. 
			 */
			LINPHONECXX_PUBLIC bool ipv6Enabled();
			
			/**
			 * @brief Turns IPv6 support on or off. 
			 *
			 * @param enable A boolean value telling whether to enable IPv6 support 
			 */
			LINPHONECXX_PUBLIC void enableIpv6(bool enable);
			
			/**
			 * @brief Gets whether auto iterate is enabled or not (Android & iOS only). 
			 *
			 * @return true if iterate() is scheduled automatically, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isAutoIterateEnabled();
			
			/**
			 * @brief Check whether the device is echo canceller calibration is required. 
			 *
			 * @return true if it is required, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isEchoCancellerCalibrationRequired();
			
			/**
			 * @brief Returns whether or not friend lists subscription are enabled. 
			 *
			 * @return whether or not the feature is enabled 
			 */
			LINPHONECXX_PUBLIC bool isFriendListSubscriptionEnabled();
			
			/**
			 * @brief Gets whether the Core is considering itself in background or not. 
			 *
			 * The Core foreground/background state depends on the last call made to
			 * enterBackground() or enterForeground(). @return true if the Core is in
			 * background, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isInBackground() const;
			
			/**
			 * @brief Indicates whether the local participant is part of a conference. 
			 *
			 * @warning That function automatically fails in the case of conferences using a
			 * conferencet server (focus). If you use such a conference, you should use
			 * Conference::removeParticipant() instead. 
			 * @return true if the local participant is in a conference, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isInConference() const;
			
			/**
			 * @brief Tells whether there is an incoming invite pending. 
			 *
			 * @return A boolean telling whether an incoming invite is pending or not. 
			 */
			LINPHONECXX_PUBLIC bool isIncomingInvitePending();
			
			/**
			 * @brief Check if the configured media encryption is mandatory or not. 
			 *
			 * @return true if media encryption is mandatory; false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool isMediaEncryptionMandatory();
			
			/**
			 * @brief Returns whether the native ringing is enabled or not. 
			 *
			 * @return True if we use the native ringing, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isNativeRingingEnabled() const;
			
			/**
			 * @brief return network state either as positioned by the application or by
			 * linphone itself. 
			 *
			 * @return true if network is reachable, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isNetworkReachable();
			
			/**
			 * @brief Gets whether push notifications are available or not (Android & iOS
			 * only). 
			 *
			 * @return true if push notifications are available, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isPushNotificationAvailable();
			
			/**
			 * @brief Gets whether push notifications are enabled or not (Android & iOS only). 
			 *
			 * @return true if push notifications are enabled, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isPushNotificationEnabled();
			
			/**
			 * @brief Returns whether or not sender name is hidden in forward message. 
			 *
			 * @return whether or not the feature 
			 */
			LINPHONECXX_PUBLIC bool isSenderNameHiddenInForwardMessage();
			
			/**
			 * @brief Gets whether the device will vibrate while an incoming call is ringing
			 * (Android only). 
			 *
			 * @return true if the device will vibrate (if possible), false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isVibrationOnIncomingCallEnabled();
			
			/**
			 * @brief Is signaling keep alive enabled. 
			 *
			 * @return A boolean value telling whether signaling keep alive is enabled 
			 */
			LINPHONECXX_PUBLIC bool keepAliveEnabled();
			
			/**
			 * @brief Enables signaling keep alive, small udp packet sent periodically to keep
			 * udp NAT association. 
			 *
			 * @param enable A boolean value telling whether signaling keep alive is to be
			 * enabled 
			 */
			LINPHONECXX_PUBLIC void enableKeepAlive(bool enable);
			
			/**
			 * @brief Get the latest outgoing call log. 
			 *
			 * @return The last outgoing call log if any.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallLog> getLastOutgoingCallLog();
			
			/**
			 * @brief Tells wether LIME X3DH is enabled or not. 
			 *
			 * @return The current lime state 
			 */
			LINPHONECXX_PUBLIC bool limeX3DhEnabled() const;
			
			/**
			 * @brief Tells to LinphoneCore to use LIME X3DH. 
			 *
			 * @param enable A boolean value telling whether to enable or disable LIME X3DH 
			 */
			LINPHONECXX_PUBLIC void enableLimeX3Dh(bool enable);
			
			/**
			 * @brief Get the x3dh server url. 
			 *
			 * @return The x3dh server url.   
			 */
			LINPHONECXX_PUBLIC std::string getLimeX3DhServerUrl();
			
			/**
			 * @brief Set the x3dh server url. 
			 *
			 * If empty, this function will disable LIME X3DH from core. Otherwise, or if
			 * different from the existing value, this will (re-)initialize the LIME X3DH
			 * engine. 
			 * @param url The x3dh server url.   
			 */
			LINPHONECXX_PUBLIC void setLimeX3DhServerUrl(const std::string & url);
			
			/**
			 * @brief Get the linphone specs value telling what functionalities the linphone
			 * client supports. 
			 *
			 * @return The linphone specs telling what functionalities the linphone client
			 * supports  
			 * @deprecated 07/02/2019 Use linphone_core_get_linphone_specs_list instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::string getLinphoneSpecs() const;
			
			/**
			 * @brief Set the linphone specs value telling what functionalities the linphone
			 * client supports. 
			 *
			 * @param specs The linphone specs to set  
			 * @deprecated 07/02/2019 Use linphone_core_set_linphone_specs_list or
			 * linphone_core_add_linphone_spec instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED void setLinphoneSpecs(const std::string & specs);
			
			/**
			 * @brief Get the list of linphone specs string values representing what
			 * functionalities the linphone client supports. 
			 *
			 * @return A list of supported specs. The list must be freed with
			 * bctbx_list_free() after usage.    
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getLinphoneSpecsList();
			
			/**
			 * @brief Set the linphone specs list value telling what functionalities the
			 * linphone client supports. 
			 *
			 * @param specs The list of string specs to set.    
			 */
			LINPHONECXX_PUBLIC void setLinphoneSpecsList(const std::list<std::string> & specs);
			
			/**
			 * @brief Gets the url of the server where to upload the collected log files. 
			 *
			 * @return The url of the server where to upload the collected log files.   
			 */
			LINPHONECXX_PUBLIC std::string getLogCollectionUploadServerUrl();
			
			/**
			 * @brief Set the url of the server where to upload the collected log files. 
			 *
			 * @param serverUrl The url of the server where to upload the collected log files.
			 *   
			 */
			LINPHONECXX_PUBLIC void setLogCollectionUploadServerUrl(const std::string & serverUrl);
			
			/**
			 * @brief Get the maximum number of simultaneous calls Linphone core can manage at
			 * a time. 
			 *
			 * All new call above this limit are declined with a busy answer 
			 * @return max number of simultaneous calls 
			 */
			LINPHONECXX_PUBLIC int getMaxCalls();
			
			/**
			 * @brief Set the maximum number of simultaneous calls Linphone core can manage at
			 * a time. 
			 *
			 * All new call above this limit are declined with a busy answer 
			 * @param max number of simultaneous calls 
			 */
			LINPHONECXX_PUBLIC void setMaxCalls(int max);
			
			/**
			 * @brief Gets the size under which incoming files in chat messages will be
			 * downloaded automatically. 
			 *
			 * @return The size in bytes, -1 if autodownload feature is disabled, 0 to
			 * download them all no matter the size 
			 */
			LINPHONECXX_PUBLIC int getMaxSizeForAutoDownloadIncomingFiles();
			
			/**
			 * @brief Sets the size under which incoming files in chat messages will be
			 * downloaded automatically. 
			 *
			 * @param size The size in bytes, -1 to disable the autodownload feature, 0 to
			 * download them all no matter the size 
			 */
			LINPHONECXX_PUBLIC void setMaxSizeForAutoDownloadIncomingFiles(int size);
			
			/**
			 * @brief Gets the name of the currently assigned sound device for media. 
			 *
			 * @return The name of the currently assigned sound device for capture.   
			 */
			LINPHONECXX_PUBLIC std::string getMediaDevice();
			
			/**
			 * @brief Sets the sound device used for media. 
			 *
			 * @param devid The device name as returned by linphone_core_get_sound_devices   
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status setMediaDevice(const std::string & devid);
			
			/**
			 * @brief Get the media encryption policy being used for RTP packets. 
			 *
			 * @return The @ref MediaEncryption policy being used. 
			 */
			LINPHONECXX_PUBLIC linphone::MediaEncryption getMediaEncryption();
			
			/**
			 * @brief Choose the media encryption policy to be used for RTP packets. 
			 *
			 * @param menc The media encryption policy to be used. 
			 * @return 0 if successful, any other value otherwise. 
			 */
			LINPHONECXX_PUBLIC linphone::Status setMediaEncryption(linphone::MediaEncryption menc);
			
			/**
			 * @brief Define whether the configured media encryption is mandatory, if it is
			 * and the negotation cannot result in the desired media encryption then the call
			 * will fail. 
			 *
			 * If not an INVITE will be resent with encryption disabled. 
			 * @param mandatory true to set it mandatory; false otherwise. 
			 */
			LINPHONECXX_PUBLIC void setMediaEncryptionMandatory(bool mandatory);
			
			/**
			 * @brief This method is called by the application to notify the linphone core
			 * library when the media (RTP) network is reachable. 
			 *
			 * This is for advanced usage, when SIP and RTP layers are required to use
			 * different interfaces. Most applications just need setNetworkReachable().
			 * @param reachable true if network is reachable, false otherwise 
			 */
			LINPHONECXX_PUBLIC void setMediaNetworkReachable(bool reachable);
			
			/**
			 * @brief This function returns the media resource mode for this core. 
			 *
			 * @return The media resource mode 
			 */
			LINPHONECXX_PUBLIC linphone::MediaResourceMode getMediaResourceMode() const;
			
			/**
			 * @brief Sets the media resources mode. 
			 *
			 * Value values are: unique and shared. When the mode is set to unique, then only
			 * one call in the state StreamsRunning is allowed. While acepting a call, the
			 * core will try to free media resource used by the current call. If it is
			 * unsuccessful, then the call is not accepted. If mode is set to shared, then the
			 * media resources of the current call (if any) are not emptied when taking a new
			 * call. If the user whishes to free them, he/she is responsible to call
			 * linphone_core_preempt_sound_resources himself/herself 
			 * @param mode the chosen mode 
			 */
			LINPHONECXX_PUBLIC void setMediaResourceMode(linphone::MediaResourceMode mode);
			
			/**
			 * @brief Tells whether the microphone is enabled. 
			 *
			 * @return true if the microphone is enabled, false if disabled. 
			 */
			LINPHONECXX_PUBLIC bool micEnabled();
			
			/**
			 * @brief Enable or disable the microphone. 
			 *
			 * @param enable true to enable the microphone, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableMic(bool enable);
			
			/**
			 * @brief Get microphone gain in db. 
			 *
			 * @return The current microphone gain 
			 */
			LINPHONECXX_PUBLIC float getMicGainDb();
			
			/**
			 * @brief Allow to control microphone level: gain in db. 
			 *
			 * @param level The new microphone level 
			 */
			LINPHONECXX_PUBLIC void setMicGainDb(float level);
			
			/**
			 * @brief Get the number of missed calls. 
			 *
			 * Once checked, this counter can be reset with resetMissedCallsCount(). 
			 * @return The number of missed calls. 
			 */
			LINPHONECXX_PUBLIC int getMissedCallsCount();
			
			/**
			 * @brief Returns the maximum transmission unit size in bytes. 
			 *
			 */
			LINPHONECXX_PUBLIC int getMtu() const;
			
			/**
			 * @brief Sets the maximum transmission unit size in bytes. 
			 *
			 * This information is useful for sending RTP packets. Default value is 1500. 
			 * @param mtu The MTU in bytes 
			 */
			LINPHONECXX_PUBLIC void setMtu(int mtu);
			
			/**
			 * @brief Get the public IP address of NAT being used. 
			 *
			 * @return The public IP address of NAT being used.   
			 */
			LINPHONECXX_PUBLIC std::string getNatAddress() const;
			
			/**
			 * @brief Set the public IP address of NAT when using the firewall policy is set
			 * to use NAT. 
			 *
			 * @param addr The public IP address of NAT to use.   
			 */
			LINPHONECXX_PUBLIC void setNatAddress(const std::string & addr);
			
			/**
			 * @brief Get The policy that is used to pass through NATs/firewalls. 
			 *
			 * It may be overridden by a NAT policy for a specific proxy config. 
			 * @return @ref NatPolicy object in use.  
			 * @see ProxyConfig::getNatPolicy() 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::NatPolicy> getNatPolicy() const;
			
			/**
			 * @brief Set the policy to use to pass through NATs/firewalls. 
			 *
			 * It may be overridden by a NAT policy for a specific proxy config. 
			 * @param policy @ref NatPolicy object  
			 * @see ProxyConfig::setNatPolicy() 
			 */
			LINPHONECXX_PUBLIC void setNatPolicy(const std::shared_ptr<linphone::NatPolicy> & policy);
			
			/**
			 * @brief Get the native window handle of the video preview window. 
			 *
			 * @return The native window handle of the video preview window.   
			 */
			LINPHONECXX_PUBLIC void * getNativePreviewWindowId() const;
			
			/**
			 * @brief Set the native window id where the preview video (local camera) is to be
			 * displayed. 
			 *
			 * This has to be used in conjonction with usePreviewWindow(). MacOS, Linux,
			 * Windows: if not set or zero the core will create its own window, unless the
			 * special id -1 is given. 
			 * @param windowId The native window id where the preview video is to be
			 * displayed.   
			 */
			LINPHONECXX_PUBLIC void setNativePreviewWindowId(void * windowId);
			
			/**
			 * @brief Sets whether to use the native ringing (Android only). 
			 *
			 * @param enable True to enable native ringing, false otherwise 
			 */
			LINPHONECXX_PUBLIC void setNativeRingingEnabled(bool enable);
			
			/**
			 * @brief Get the native window handle of the video window. 
			 *
			 * @return The native window handle of the video window.   
			 */
			LINPHONECXX_PUBLIC void * getNativeVideoWindowId() const;
			
			/**
			 * @brief Set the native video window id where the video is to be displayed. 
			 *
			 * For MacOS, Linux, Windows: if not set or LINPHONE_VIDEO_DISPLAY_AUTO the core
			 * will create its own window, unless the special id LINPHONE_VIDEO_DISPLAY_NONE
			 * is given. 
			 * @param windowId The native window id where the remote video is to be displayed.
			 *   
			 */
			LINPHONECXX_PUBLIC void setNativeVideoWindowId(void * windowId);
			
			/**
			 * @brief This method is called by the application to notify the linphone core
			 * library when network is reachable. 
			 *
			 * Calling this method with true trigger linphone to initiate a registration
			 * process for all proxies. Calling this method disables the automatic network
			 * detection mode. It means you must call this method after each network state
			 * changes.
			 * @param reachable true if network is reachable, false otherwise 
			 */
			LINPHONECXX_PUBLIC void setNetworkReachable(bool reachable);
			
			/**
			 * @brief Gets the value of the no-rtp timeout. 
			 *
			 * When no RTP or RTCP packets have been received for a while @ref Core will
			 * consider the call is broken (remote end crashed or disconnected from the
			 * network), and thus will terminate the call. The no-rtp timeout is the duration
			 * above which the call is considered broken. 
			 * @return The value of the no-rtp timeout in seconds 
			 */
			LINPHONECXX_PUBLIC int getNortpTimeout() const;
			
			/**
			 * @brief Sets the no-rtp timeout value in seconds. 
			 *
			 * @param seconds The no-rtp timeout value to use in seconds
			 * @see getNortpTimeout() for details. 
			 */
			LINPHONECXX_PUBLIC void setNortpTimeout(int seconds);
			
			/**
			 * @brief Gets the output audio device for the current call. 
			 *
			 * @return The output audio device for the current or first call, nullptr if there
			 * is no call.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AudioDevice> getOutputAudioDevice() const;
			
			/**
			 * @brief Sets the given @ref AudioDevice as output for all active calls. 
			 *
			 * @param audioDevice The @ref AudioDevice. nullptr does nothing.   
			 */
			LINPHONECXX_PUBLIC void setOutputAudioDevice(const std::shared_ptr<linphone::AudioDevice> & audioDevice);
			
			/**
			 * @brief Get the wav file that is played when putting somebody on hold, or when
			 * files are used instead of soundcards (see setUseFiles()). 
			 *
			 * The file is a 16 bit linear wav file. 
			 * @return The path to the file that is played when putting somebody on hold.   
			 */
			LINPHONECXX_PUBLIC std::string getPlayFile() const;
			
			/**
			 * @brief Sets a wav file to be played when putting somebody on hold, or when
			 * files are used instead of soundcards (see setUseFiles()). 
			 *
			 * The file must be a 16 bit linear wav file. 
			 * @param file The path to the file to be played when putting somebody on hold.   
			 */
			LINPHONECXX_PUBLIC void setPlayFile(const std::string & file);
			
			/**
			 * @brief Gets the name of the currently assigned sound device for playback. 
			 *
			 * @return The name of the currently assigned sound device for playback.   
			 */
			LINPHONECXX_PUBLIC std::string getPlaybackDevice();
			
			/**
			 * @brief Sets the sound device used for playback. 
			 *
			 * @param devid The device name as returned by linphone_core_get_sound_devices   
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status setPlaybackDevice(const std::string & devid);
			
			/**
			 * @brief Get playback gain in db before entering sound card. 
			 *
			 * @return The current playback gain 
			 */
			LINPHONECXX_PUBLIC float getPlaybackGainDb();
			
			/**
			 * @brief Allow to control play level before entering sound card: gain in db. 
			 *
			 * @param level The new play level 
			 */
			LINPHONECXX_PUBLIC void setPlaybackGainDb(float level);
			
			/**
			 * @brief Returns the preferred video framerate, previously set by
			 * setPreferredFramerate(). 
			 *
			 * @return frame rate in number of frames per seconds. 
			 */
			LINPHONECXX_PUBLIC float getPreferredFramerate();
			
			/**
			 * @brief Set the preferred frame rate for video. 
			 *
			 * Based on the available bandwidth constraints and network conditions, the video
			 * encoder remains free to lower the framerate. There is no warranty that the
			 * preferred frame rate be the actual framerate. used during a call. Default value
			 * is 0, which means "use encoder's default fps value". 
			 * @param fps the target frame rate in number of frames per seconds. 
			 */
			LINPHONECXX_PUBLIC void setPreferredFramerate(float fps);
			
			/**
			 * @brief Get the preferred video definition for the stream that is captured and
			 * sent to the remote party. 
			 *
			 * @return The preferred @ref VideoDefinition   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::VideoDefinition> getPreferredVideoDefinition() const;
			
			/**
			 * @brief Set the preferred video definition for the stream that is captured and
			 * sent to the remote party. 
			 *
			 * All standard video definitions are accepted on the receive path. 
			 * @param videoDefinition @ref VideoDefinition object   
			 */
			LINPHONECXX_PUBLIC void setPreferredVideoDefinition(const std::shared_ptr<linphone::VideoDefinition> & videoDefinition);
			
			/**
			 * @brief Sets the preferred video definition by its name. 
			 *
			 * Call Factory::getSupportedVideoDefinitions() to have a list of supported video
			 * definitions.
			 * @param name The name of the definition to set   
			 */
			LINPHONECXX_PUBLIC void setPreferredVideoDefinitionByName(const std::string & name);
			
			/**
			 * @brief Get my presence model. 
			 *
			 * @return A @ref PresenceModel object, or nullptr if no presence model has been
			 * set.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceModel> getPresenceModel() const;
			
			/**
			 * @brief Set my presence model. 
			 *
			 * @param presence @ref PresenceModel   
			 */
			LINPHONECXX_PUBLIC void setPresenceModel(const std::shared_ptr<linphone::PresenceModel> & presence);
			
			/**
			 * @brief Get the definition of the captured video. 
			 *
			 * @return The captured @ref VideoDefinition if it was previously set by
			 * setPreviewVideoDefinition(), otherwise a 0x0 LinphoneVideoDefinition.   
			 * @see setPreviewVideoDefinition() 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::VideoDefinition> getPreviewVideoDefinition() const;
			
			/**
			 * @brief Set the video definition for the captured (preview) video. 
			 *
			 * This method is for advanced usage where a video capture must be set
			 * independently of the definition of the stream actually sent through the call.
			 * This allows for example to have the preview window in High Definition even if
			 * due to bandwidth constraint the sent video definition is small. Using this
			 * feature increases the CPU consumption, since a rescaling will be done
			 * internally. 
			 * @param videoDefinition @ref VideoDefinition object   
			 */
			LINPHONECXX_PUBLIC void setPreviewVideoDefinition(const std::shared_ptr<linphone::VideoDefinition> & videoDefinition);
			
			/**
			 * @brief Returns the default identity when no proxy configuration is used. 
			 *
			 * @return the primary contact identity   
			 */
			LINPHONECXX_PUBLIC std::string getPrimaryContact();
			
			/**
			 * @brief Sets the local "from" identity. 
			 *
			 * This data is used in absence of any proxy configuration or when no default
			 * proxy configuration is set. See @ref ProxyConfig 
			 * @param contact the contact to set   
			 * @return 0 if successful, -1 otherwise 
			 */
			LINPHONECXX_PUBLIC linphone::Status setPrimaryContact(const std::string & contact);
			
			/**
			 * @brief Same as getPrimaryContact() but the result is a @ref Address object
			 * instead of const char *. 
			 *
			 * @return a @ref Address object.    
			 * @deprecated 22/10/2018 Use createPrimaryContactParsed() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::Address> getPrimaryContactParsed();
			
			/**
			 * @brief Get provisioning URI. 
			 *
			 * @return the provisioning URI.   
			 */
			LINPHONECXX_PUBLIC std::string getProvisioningUri() const;
			
			/**
			 * @brief Set URI where to download xml configuration file at startup. 
			 *
			 * This can also be set from configuration file or factory config file, from
			 * [misc] section, item "config-uri". Calling this function does not load the
			 * configuration. It will write the value into configuration so that configuration
			 * from remote URI will take place at next @ref Core start. 
			 * @param uri the http or https uri to use in order to download the configuration.
			 * Passing nullptr will disable remote provisioning.   
			 * @return -1 if uri could not be parsed, 0 otherwise. Note that this does not
			 * check validity of URI endpoint nor scheme and download may still fail. 
			 */
			LINPHONECXX_PUBLIC linphone::Status setProvisioningUri(const std::string & uri);
			
			/**
			 * @brief Returns an unmodifiable list of entered proxy configurations. 
			 *
			 * @return A list of @ref ProxyConfig.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::ProxyConfig>> getProxyConfigList() const;
			
			/**
			 * @brief Returns the push incoming call timeout See setPushIncomingCallTimeout()
			 * for details. 
			 *
			 * @return The current push incoming call timeout in seconds 
			 */
			LINPHONECXX_PUBLIC int getPushIncomingCallTimeout() const;
			
			/**
			 * @brief Configure the minimum interval between a push notification and the
			 * corresponding incoming INVITE. 
			 *
			 * If exceeded, Linphone Call is transitioned to CallError and further incoming
			 * invite associated to this push is declined if any. 
			 * @param seconds The new timeout in seconds 
			 */
			LINPHONECXX_PUBLIC void setPushIncomingCallTimeout(int seconds);
			
			/**
			 * @brief Enable or disable push notifications on Android & iOS. 
			 *
			 * If enabled, it will try to get the push token add configure each proxy config
			 * with push_notification_allowed set to true with push parameters. 
			 * @param enable true to enable push notifications, false to disable 
			 */
			LINPHONECXX_PUBLIC void setPushNotificationEnabled(bool enable);
			
			/**
			 * @brief Tells whether QRCode is enabled in the preview. 
			 *
			 * @return A boolean value telling whether QRCode is enabled in the preview. 
			 */
			LINPHONECXX_PUBLIC bool qrcodeVideoPreviewEnabled() const;
			
			/**
			 * @brief Controls QRCode enablement. 
			 *
			 * @param enable A boolean value telling whether the QRCode is enabled in the
			 * preview. 
			 */
			LINPHONECXX_PUBLIC void enableQrcodeVideoPreview(bool enable);
			
			/**
			 * @brief Gets if realtime text is enabled or not. 
			 *
			 * @return true if realtime text is enabled, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool realtimeTextEnabled();
			
			/**
			 * @brief Get the wav file where incoming stream is recorded, when files are used
			 * instead of soundcards (see setUseFiles()). 
			 *
			 * This feature is different from call recording (CallParams::setRecordFile()) The
			 * file is a 16 bit linear wav file. 
			 * @return The path to the file where incoming stream is recorded.   
			 */
			LINPHONECXX_PUBLIC std::string getRecordFile() const;
			
			/**
			 * @brief Sets a wav file where incoming stream is to be recorded, when files are
			 * used instead of soundcards (see setUseFiles()). 
			 *
			 * This feature is different from call recording (CallParams::setRecordFile()) The
			 * file will be a 16 bit linear wav file. 
			 * @param file The path to the file where incoming stream is to be recorded.   
			 */
			LINPHONECXX_PUBLIC void setRecordFile(const std::string & file);
			
			/**
			 * @brief Get the ring back tone played to far end during incoming calls. 
			 *
			 * @return the path to the remote ring back tone to be played.   
			 */
			LINPHONECXX_PUBLIC std::string getRemoteRingbackTone() const;
			
			/**
			 * @brief Specify a ring back tone to be played to far end during incoming calls. 
			 *
			 * @param ring The path to the remote ring back tone to be played.   
			 */
			LINPHONECXX_PUBLIC void setRemoteRingbackTone(const std::string & ring);
			
			/**
			 * @brief Tells whether NACK context is enabled or not. 
			 *
			 * @return A boolean value telling whether NACK context is enabled or not 
			 */
			LINPHONECXX_PUBLIC bool retransmissionOnNackEnabled();
			
			/**
			 * @brief Turns NACK context on or off. 
			 *
			 * @param enable A boolean value telling whether to enable NACK context 
			 */
			LINPHONECXX_PUBLIC void enableRetransmissionOnNack(bool enable);
			
			/**
			 * @brief Returns the path to the wav file used for ringing. 
			 *
			 * @return The path to the wav file used for ringing.   
			 */
			LINPHONECXX_PUBLIC std::string getRing() const;
			
			/**
			 * @brief Sets the path to a wav file used for ringing. 
			 *
			 * The file must be a wav 16bit linear. If null, ringing is disable unless
			 * #linphone_core_get_use_native_ringing() is enabled, in which case we use the
			 * device ringtone. 
			 * @param path The path to a wav file to be used for ringing, null to disable or
			 * use device ringing depending on #linphone_core_get_use_native_ringing().   
			 */
			LINPHONECXX_PUBLIC void setRing(const std::string & path);
			
			/**
			 * @brief Tells whether the ring play is enabled during an incoming early media
			 * call. 
			 *
			 */
			LINPHONECXX_PUBLIC bool getRingDuringIncomingEarlyMedia() const;
			
			/**
			 * @brief Enable or disable the ring play during an incoming early media call. 
			 *
			 * @param enable A boolean value telling whether to enable ringing during an
			 * incoming early media call. 
			 */
			LINPHONECXX_PUBLIC void setRingDuringIncomingEarlyMedia(bool enable);
			
			/**
			 * @brief Returns the path to the wav file used for ringing back. 
			 *
			 * @return The path to the wav file used for ringing back.   
			 */
			LINPHONECXX_PUBLIC std::string getRingback() const;
			
			/**
			 * @brief Sets the path to a wav file used for ringing back. 
			 *
			 * Ringback means the ring that is heard when it's ringing at the remote party.
			 * The file must be a wav 16bit linear. 
			 * @param path The path to a wav file to be used for ringing back.   
			 */
			LINPHONECXX_PUBLIC void setRingback(const std::string & path);
			
			/**
			 * @brief Gets the name of the currently assigned sound device for ringing. 
			 *
			 * @return The name of the currently assigned sound device for ringing.   
			 */
			LINPHONECXX_PUBLIC std::string getRingerDevice();
			
			/**
			 * @brief Sets the sound device used for ringing. 
			 *
			 * @param devid The device name as returned by linphone_core_get_sound_devices   
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status setRingerDevice(const std::string & devid);
			
			/**
			 * @brief Gets the path to a file or folder containing the trusted root CAs (PEM
			 * format) 
			 *
			 * @return The path to a file or folder containing the trusted root CAs.   
			 */
			LINPHONECXX_PUBLIC std::string getRootCa();
			
			/**
			 * @brief Sets the path to a file or folder containing trusted root CAs (PEM
			 * format) 
			 *
			 * @param path The path to a file or folder containing trusted root CAs.   
			 */
			LINPHONECXX_PUBLIC void setRootCa(const std::string & path);
			
			/**
			 * @brief Sets the trusted root CAs (PEM format) 
			 *
			 * @param data The trusted root CAs as a string   
			 */
			LINPHONECXX_PUBLIC void setRootCaData(const std::string & data);
			
			/**
			 * @brief Returns whether RTP bundle mode (also known as Media Multiplexing) is
			 * enabled. 
			 *
			 * See https://tools.ietf.org/html/draft-ietf-mmusic-sdp-bundle-negotiation-54 for
			 * more information. 
			 * @return a boolean indicating the enablement of rtp bundle mode. 
			 */
			LINPHONECXX_PUBLIC bool rtpBundleEnabled() const;
			
			/**
			 * @brief Enables or disables RTP bundle mode (Media Multiplexing). 
			 *
			 * See https://tools.ietf.org/html/draft-ietf-mmusic-sdp-bundle-negotiation-54 for
			 * more information about the feature. When enabled, liblinphone will try to
			 * negociate the use of a single port for all streams when doing an outgoing call.
			 * It automatically enables rtcp-mux. This feature can also be enabled per-call
			 * using @ref CallParams. 
			 * @param value a boolean to indicate whether the feature is to be enabled. 
			 */
			LINPHONECXX_PUBLIC void enableRtpBundle(bool value);
			
			/**
			 * @brief Media offer control param for SIP INVITE. 
			 *
			 * @return true if INVITE has to be sent whitout SDP. 
			 */
			LINPHONECXX_PUBLIC bool sdp200AckEnabled() const;
			
			/**
			 * @brief Control when media offer is sent in SIP INVITE. 
			 *
			 * @param enable true if INVITE has to be sent whitout SDP. 
			 */
			LINPHONECXX_PUBLIC void enableSdp200Ack(bool enable);
			
			/**
			 * @brief Tells whether video self view during call is enabled or not. 
			 *
			 * @return A boolean value telling whether self view is enabled 
			 * @see enableSelfView() for details. 
			 */
			LINPHONECXX_PUBLIC bool selfViewEnabled() const;
			
			/**
			 * @brief Enables or disable self view during calls. 
			 *
			 * @param enable A boolean value telling whether to enable self view Self-view
			 * refers to having local webcam image inserted in corner of the video window
			 * during calls. This function works at any time, including during calls. 
			 */
			LINPHONECXX_PUBLIC void enableSelfView(bool enable);
			
			/**
			 * @brief Enable whether or not to hide sender name in forward message. 
			 *
			 * @param enable whether or not to enable the feature 
			 */
			LINPHONECXX_PUBLIC void enableSenderNameHiddenInForwardMessage(bool enable);
			
			/**
			 * @brief Check if the Session Timers feature is enabled. 
			 *
			 */
			LINPHONECXX_PUBLIC bool getSessionExpiresEnabled() const;
			
			/**
			 * @brief Enable the Session Timers support. 
			 *
			 * @param enabled Enable or disable it 
			 */
			LINPHONECXX_PUBLIC void setSessionExpiresEnabled(bool enabled) const;
			
			/**
			 * @brief Returns the session expires min value, 90 by default. 
			 *
			 */
			LINPHONECXX_PUBLIC int getSessionExpiresMinValue() const;
			
			/**
			 * @brief Sets the session expires minSE value, forced to a minimum of 90 by
			 * default. 
			 *
			 */
			LINPHONECXX_PUBLIC void setSessionExpiresMinValue(int min) const;
			
			/**
			 * @brief Returns the session expires refresher value. 
			 *
			 */
			LINPHONECXX_PUBLIC linphone::SessionExpiresRefresher getSessionExpiresRefresherValue() const;
			
			/**
			 * @brief Sets the session expires refresher value. 
			 *
			 * @param refresher The @ref SessionExpiresRefresher configuration value 
			 */
			LINPHONECXX_PUBLIC void setSessionExpiresRefresherValue(linphone::SessionExpiresRefresher refresher) const;
			
			/**
			 * @brief Returns the session expires value. 
			 *
			 */
			LINPHONECXX_PUBLIC int getSessionExpiresValue() const;
			
			/**
			 * @brief Sets the session expires value, 0 by default. 
			 *
			 */
			LINPHONECXX_PUBLIC void setSessionExpiresValue(int expires) const;
			
			/**
			 * @brief Get the DSCP field for SIP signaling channel. 
			 *
			 * The DSCP defines the quality of service in IP packets. 
			 * @return The current DSCP value 
			 */
			LINPHONECXX_PUBLIC int getSipDscp() const;
			
			/**
			 * @brief Set the DSCP field for SIP signaling channel. 
			 *
			 * The DSCP defines the quality of service in IP packets. 
			 * @param dscp The DSCP value to set 
			 */
			LINPHONECXX_PUBLIC void setSipDscp(int dscp);
			
			/**
			 * @brief This method is called by the application to notify the linphone core
			 * library when the SIP network is reachable. 
			 *
			 * This is for advanced usage, when SIP and RTP layers are required to use
			 * different interfaces. Most applications just need setNetworkReachable().
			 * @param reachable true if network is reachable, false otherwise 
			 */
			LINPHONECXX_PUBLIC void setSipNetworkReachable(bool reachable);
			
			/**
			 * @brief Get the SIP transport timeout. 
			 *
			 * @return The SIP transport timeout in milliseconds. 
			 */
			LINPHONECXX_PUBLIC int getSipTransportTimeout();
			
			/**
			 * @brief Set the SIP transport timeout. 
			 *
			 * @param timeoutMs The SIP transport timeout in milliseconds. 
			 */
			LINPHONECXX_PUBLIC void setSipTransportTimeout(int timeoutMs);
			
			/**
			 * @brief Gets the list of the available sound devices. 
			 *
			 * @return An unmodifiable array of strings contanining the names of the available
			 * sound devices that is nullptr terminated.      
			 * @deprecated 10/04/2021 Use getAudioDevices() instead.
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::list<std::string> getSoundDevicesList() const;
			
			/**
			 * @brief Get the path to the image file streamed when "Static picture" is set as
			 * the video device. 
			 *
			 * @return The path to the image file streamed when "Static picture" is set as the
			 * video device.   
			 */
			LINPHONECXX_PUBLIC std::string getStaticPicture();
			
			/**
			 * @brief Set the path to the image file to stream when "Static picture" is set as
			 * the video device. 
			 *
			 * @param path The path to the image file to use.   
			 */
			LINPHONECXX_PUBLIC linphone::Status setStaticPicture(const std::string & path);
			
			/**
			 * @brief Get the frame rate for static picture. 
			 *
			 * @return The frame rate used for static picture. 
			 */
			LINPHONECXX_PUBLIC float getStaticPictureFps();
			
			/**
			 * @brief Set the frame rate for static picture. 
			 *
			 * @param fps The new frame rate to use for static picture. 
			 */
			LINPHONECXX_PUBLIC linphone::Status setStaticPictureFps(float fps);
			
			/**
			 * @brief Get the STUN server address being used. 
			 *
			 * @return The STUN server address being used.   
			 */
			LINPHONECXX_PUBLIC std::string getStunServer() const;
			
			/**
			 * @brief Set the STUN server address to use when the firewall policy is set to
			 * STUN. 
			 *
			 * @param server The STUN server address to use.   
			 */
			LINPHONECXX_PUBLIC void setStunServer(const std::string & server);
			
			/**
			 * @brief Returns a null terminated table of strings containing the file format
			 * extension supported for call recording. 
			 *
			 * @return The supported formats, typically 'wav' and 'mkv'.      
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getSupportedFileFormatsList();
			
			/**
			 * @brief Set the supported tags. 
			 *
			 */
			LINPHONECXX_PUBLIC void setSupportedTag(const std::string & tags);
			
			/**
			 * @brief Return the list of the available text payload types. 
			 *
			 * @return A freshly allocated list of the available payload types. The list must
			 * be destroyed with bctbx_list_free() after usage. The elements of the list
			 * haven't to be unref.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::PayloadType>> getTextPayloadTypes();
			
			/**
			 * @brief Redefine the list of the available payload types. 
			 *
			 * @param payloadTypes The new list of payload types. The core does not take
			 * ownership on it.    
			 */
			LINPHONECXX_PUBLIC void setTextPayloadTypes(const std::list<std::shared_ptr<linphone::PayloadType>> & payloadTypes);
			
			/**
			 * @brief Gets the UDP port used for text streaming. 
			 *
			 * @return The UDP port used for text streaming 
			 */
			LINPHONECXX_PUBLIC int getTextPort() const;
			
			/**
			 * @brief Sets the UDP port used for text streaming. 
			 *
			 * A value if -1 will request the system to allocate the local port randomly. This
			 * is recommended in order to avoid firewall warnings. 
			 * @param port The UDP port to use for text streaming 
			 */
			LINPHONECXX_PUBLIC void setTextPort(int port);
			
			/**
			 * @brief Get the text port range from which is randomly chosen the UDP port used
			 * for text streaming. 
			 *
			 * @return a @ref Range object     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Range> getTextPortsRange() const;
			
			/**
			 * @brief Gets the TLS certificate. 
			 *
			 * @return the TLS certificate or nullptr if not set yet.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsCert() const;
			
			/**
			 * @brief Sets a TLS certificate used for TLS authentication The certificate won't
			 * be stored, you have to set it after each @ref Core startup. 
			 *
			 * @param tlsCert the TLS certificate.   
			 */
			LINPHONECXX_PUBLIC void setTlsCert(const std::string & tlsCert);
			
			/**
			 * @brief Gets the path to the TLS certificate file. 
			 *
			 * @return the TLS certificate path or nullptr if not set yet.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsCertPath() const;
			
			/**
			 * @brief Sets a TLS certificate path used for TLS authentication The path will be
			 * stored in the rc file and automatically restored on startup. 
			 *
			 * @param tlsCertPath path to the TLS certificate.   
			 */
			LINPHONECXX_PUBLIC void setTlsCertPath(const std::string & tlsCertPath);
			
			/**
			 * @brief Gets the TLS key. 
			 *
			 * @return the TLS key or nullptr if not set yet.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsKey() const;
			
			/**
			 * @brief Sets a TLS key used for TLS authentication The key won't be stored, you
			 * have to set it after each @ref Core startup. 
			 *
			 * @param tlsKey the TLS key.   
			 */
			LINPHONECXX_PUBLIC void setTlsKey(const std::string & tlsKey);
			
			/**
			 * @brief Gets the path to the TLS key file. 
			 *
			 * @return the TLS key path or nullptr if not set yet.   
			 */
			LINPHONECXX_PUBLIC std::string getTlsKeyPath() const;
			
			/**
			 * @brief Sets a TLS key path used for TLS authentication The path will be stored
			 * in the rc file and automatically restored on startup. 
			 *
			 * @param tlsKeyPath path to the TLS key.   
			 */
			LINPHONECXX_PUBLIC void setTlsKeyPath(const std::string & tlsKeyPath);
			
			/**
			 * @brief Retrieves the port configuration used for each transport (udp, tcp,
			 * tls). 
			 *
			 * A zero value port for a given transport means the transport is not used. A
			 * value of LC_SIP_TRANSPORT_RANDOM (-1) means the port is to be chosen randomly
			 * by the system. 
			 * @return A @ref Transports structure with the configured ports     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Transports> getTransports();
			
			/**
			 * @brief Sets the ports to be used for each of transport (UDP or TCP) A zero
			 * value port for a given transport means the transport is not used. 
			 *
			 * A value of LC_SIP_TRANSPORT_RANDOM (-1) means the port is to be choosen
			 * randomly by the system. 
			 * @param transports A #LinphoneSipTransports structure giving the ports to use   
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status setTransports(const std::shared_ptr<const linphone::Transports> & transports);
			
			/**
			 * @brief Retrieves the real port number assigned for each sip transport (udp,
			 * tcp, tls). 
			 *
			 * A zero value means that the transport is not activated. If
			 * LC_SIP_TRANSPORT_RANDOM was passed to linphone_core_set_sip_transports, the
			 * random port choosed by the system is returned. 
			 * @return A @ref Transports structure with the ports being used     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Transports> getTransportsUsed();
			
			/**
			 * @brief get tunnel instance if available 
			 *
			 * @return @ref Tunnel or nullptr if not available.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Tunnel> getTunnel() const;
			
			/**
			 * @brief Return the global unread chat message count. 
			 *
			 * @return The global unread chat message count. 
			 */
			LINPHONECXX_PUBLIC int getUnreadChatMessageCount() const;
			
			/**
			 * @brief Return the unread chat message count for all active local address. 
			 *
			 * (Primary contact + proxy configs.) 
			 * @return The unread chat message count. 
			 */
			LINPHONECXX_PUBLIC int getUnreadChatMessageCountFromActiveLocals() const;
			
			/**
			 * @brief Retrieve the maximum available upload bandwidth. 
			 *
			 * This value was set by setUploadBandwidth(). 
			 * @return the upload bandiwdth in kbits/s, 0 for infinite 
			 */
			LINPHONECXX_PUBLIC int getUploadBandwidth() const;
			
			/**
			 * @brief Sets maximum available upload bandwidth This is IP bandwidth, in kbit/s. 
			 *
			 * This information is used by liblinphone together with remote side available
			 * bandwidth signaled in SDP messages to properly configure audio & video codec's
			 * output bitrate.
			 * @param bandwidth the bandwidth in kbits/s, 0 for infinite 
			 */
			LINPHONECXX_PUBLIC void setUploadBandwidth(int bandwidth);
			
			/**
			 * @brief Set audio packetization time linphone will send (in absence of
			 * requirement from peer) A value of 0 stands for the current codec default
			 * packetization time. 
			 *
			 * @return the upload packetization time set 
			 */
			LINPHONECXX_PUBLIC int getUploadPtime();
			
			/**
			 * @brief Set audio packetization time linphone will send (in absence of
			 * requirement from peer) A value of 0 stands for the current codec default
			 * packetization time. 
			 *
			 * @param ptime the upload packetization time to set 
			 */
			LINPHONECXX_PUBLIC void setUploadPtime(int ptime);
			
			/**
			 * @brief Return the external ip address of router. 
			 *
			 * In some cases the uPnP can have an external ip address but not a usable uPnP
			 * (state different of Ok).
			 * @return a null terminated string containing the external ip address. If the the
			 * external ip address is not available return null.   
			 */
			LINPHONECXX_PUBLIC std::string getUpnpExternalIpaddress() const;
			
			/**
			 * @brief Return the internal state of uPnP. 
			 *
			 * @return an LinphoneUpnpState. 
			 */
			LINPHONECXX_PUBLIC linphone::UpnpState getUpnpState() const;
			
			/**
			 * @brief Gets whether linphone is currently streaming audio from and to files,
			 * rather than using the soundcard. 
			 *
			 * @return A boolean value representing whether linphone is streaming audio from
			 * and to files or not. 
			 */
			LINPHONECXX_PUBLIC bool getUseFiles();
			
			/**
			 * @brief Ask the core to stream audio from and to files, instead of using the
			 * soundcard. 
			 *
			 * @param yesno A boolean value asking to stream audio from and to files or not. 
			 */
			LINPHONECXX_PUBLIC void setUseFiles(bool yesno);
			
			/**
			 * @brief Indicates whether SIP INFO is used to send digits. 
			 *
			 * @return A boolean value telling whether SIP INFO is used to send digits 
			 */
			LINPHONECXX_PUBLIC bool getUseInfoForDtmf();
			
			/**
			 * @brief Sets whether SIP INFO is to be used to send digits. 
			 *
			 * @param useInfo A boolean value telling whether to use SIP INFO to send digits 
			 */
			LINPHONECXX_PUBLIC void setUseInfoForDtmf(bool useInfo);
			
			/**
			 * @brief Indicates whether RFC2833 is used to send digits. 
			 *
			 * @return A boolean value telling whether RFC2833 is used to send digits 
			 */
			LINPHONECXX_PUBLIC bool getUseRfc2833ForDtmf();
			
			/**
			 * @brief Sets whether RFC2833 is to be used to send digits. 
			 *
			 * @param useRfc2833 A boolean value telling whether to use RFC2833 to send digits 
			 */
			LINPHONECXX_PUBLIC void setUseRfc2833ForDtmf(bool useRfc2833);
			
			/**
			 * @brief Gets the user-agent as a string. 
			 *
			 * @return liblinphone's user agent as a string.   
			 */
			LINPHONECXX_PUBLIC std::string getUserAgent();
			
			/**
			 * @brief Get the path to the directory storing the user's certificates. 
			 *
			 * @return The path to the directory storing the user's certificates.   
			 */
			LINPHONECXX_PUBLIC std::string getUserCertificatesPath();
			
			/**
			 * @brief Set the path to the directory storing the user's x509 certificates (used
			 * by dtls) 
			 *
			 * @param path The path to the directory to use to store the user's certificates. 
			 *  
			 */
			LINPHONECXX_PUBLIC void setUserCertificatesPath(const std::string & path);
			
			/**
			 * @brief Enable vibration will incoming call is ringing (Android only). 
			 *
			 * @return true if the device should vibrate while an incoming call is ringing,
			 * false otherwise 
			 */
			LINPHONECXX_PUBLIC void setVibrationOnIncomingCallEnabled(bool enable);
			
			/**
			 * @brief Get the default policy for video. 
			 *
			 * See setVideoActivationPolicy() for more details. 
			 * @return The video policy being used     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::VideoActivationPolicy> getVideoActivationPolicy() const;
			
			/**
			 * @brief Sets the default policy for video. 
			 *
			 * This policy defines whether:
			 * @param policy The @ref VideoActivationPolicy to use   
			 */
			LINPHONECXX_PUBLIC void setVideoActivationPolicy(const std::shared_ptr<const linphone::VideoActivationPolicy> & policy);
			
			/**
			 * @brief Tells whether the video adaptive jitter compensation is enabled. 
			 *
			 * @return true if the video adaptive jitter compensation is enabled, false
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC bool videoAdaptiveJittcompEnabled();
			
			/**
			 * @brief Enable or disable the video adaptive jitter compensation. 
			 *
			 * @param enable true to enable the video adaptive jitter compensation, false to
			 * disable it. 
			 */
			LINPHONECXX_PUBLIC void enableVideoAdaptiveJittcomp(bool enable);
			
			/**
			 * @brief Tells whether video capture is enabled. 
			 *
			 * @return true if video capture is enabled, false if disabled. 
			 */
			LINPHONECXX_PUBLIC bool videoCaptureEnabled();
			
			/**
			 * @brief Enable or disable video capture. 
			 *
			 * This function does not have any effect during calls. It just indicates the @ref
			 * Core to initiate future calls with video capture or not. 
			 * @param enable true to enable video capture, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableVideoCapture(bool enable);
			
			/**
			 * @brief Returns the name of the currently active video device. 
			 *
			 * @return The name of the currently active video device.   
			 */
			LINPHONECXX_PUBLIC std::string getVideoDevice() const;
			
			/**
			 * @brief Sets the active video device. 
			 *
			 * @param id The name of the video device to use as returned by
			 * linphone_core_get_video_devices   
			 */
			LINPHONECXX_PUBLIC linphone::Status setVideoDevice(const std::string & id);
			
			/**
			 * @brief Gets the list of the available video capture devices. 
			 *
			 * @return An unmodifiable array of strings contanining the names of the available
			 * video capture devices that is nullptr terminated.      
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getVideoDevicesList() const;
			
			/**
			 * @brief Tells whether video display is enabled. 
			 *
			 * @return true if video display is enabled, false if disabled. 
			 */
			LINPHONECXX_PUBLIC bool videoDisplayEnabled();
			
			/**
			 * @brief Enable or disable video display. 
			 *
			 * This function does not have any effect during calls. It just indicates the @ref
			 * Core to initiate future calls with video display or not. 
			 * @param enable true to enable video display, false to disable it. 
			 */
			LINPHONECXX_PUBLIC void enableVideoDisplay(bool enable);
			
			/**
			 * @brief Get the name of the mediastreamer2 filter used for rendering video. 
			 *
			 * @return The currently selected video display filter.   
			 */
			LINPHONECXX_PUBLIC std::string getVideoDisplayFilter();
			
			/**
			 * @brief Set the name of the mediastreamer2 filter to be used for rendering
			 * video. 
			 *
			 * This is for advanced users of the library, mainly to workaround hardware/driver
			 * bugs. 
			 * @param filterName the filter name to use or nullptr to use default.   
			 */
			LINPHONECXX_PUBLIC void setVideoDisplayFilter(const std::string & filterName);
			
			/**
			 * @brief Get the DSCP field for outgoing video streams. 
			 *
			 * The DSCP defines the quality of service in IP packets. 
			 * @return The current DSCP value 
			 */
			LINPHONECXX_PUBLIC int getVideoDscp() const;
			
			/**
			 * @brief Set the DSCP field for outgoing video streams. 
			 *
			 * The DSCP defines the quality of service in IP packets. 
			 * @param dscp The DSCP value to set 
			 */
			LINPHONECXX_PUBLIC void setVideoDscp(int dscp);
			
			/**
			 * @brief Returns true if either capture or display is enabled, false otherwise. 
			 *
			 * same as ( videoCaptureEnabled() | videoDisplayEnabled() )
			 * @return true if either capture or display is enabled, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool videoEnabled();
			
			/**
			 * @brief Returns the nominal video jitter buffer size in milliseconds. 
			 *
			 * @return The nominal video jitter buffer size in milliseconds 
			 */
			LINPHONECXX_PUBLIC int getVideoJittcomp();
			
			/**
			 * @brief Sets the nominal video jitter buffer size in milliseconds. 
			 *
			 * The value takes effect immediately for all running and pending calls, if any. A
			 * value of 0 disables the jitter buffer.
			 * @param milliseconds the jitter buffer size in milliseconds 
			 */
			LINPHONECXX_PUBLIC void setVideoJittcomp(int milliseconds);
			
			/**
			 * @brief Use to get multicast address to be used for video stream. 
			 *
			 * @return an ipv4/6 multicast address, or default value.   
			 */
			LINPHONECXX_PUBLIC std::string getVideoMulticastAddr() const;
			
			/**
			 * @brief Use to set multicast address to be used for video stream. 
			 *
			 * @param ip an ipv4/6 multicast address.   
			 * @return 0 in case of success 
			 */
			LINPHONECXX_PUBLIC linphone::Status setVideoMulticastAddr(const std::string & ip);
			
			/**
			 * @brief Use to get multicast state of video stream. 
			 *
			 * @return true if subsequent calls will propose multicast ip set by
			 * setVideoMulticastAddr() 
			 */
			LINPHONECXX_PUBLIC bool videoMulticastEnabled() const;
			
			/**
			 * @brief Use to enable multicast rtp for video stream. 
			 *
			 * If enabled, outgoing calls put a multicast address from getVideoMulticastAddr()
			 * into video cline. In case of outgoing call video stream is sent to this
			 * multicast address. For incoming calls behavior is unchanged. 
			 * @param yesno if yes, subsequent outgoing calls will propose multicast ip set by
			 * setVideoMulticastAddr() 
			 */
			LINPHONECXX_PUBLIC void enableVideoMulticast(bool yesno);
			
			/**
			 * @brief Use to get multicast ttl to be used for video stream. 
			 *
			 * @return a time to leave value 
			 */
			LINPHONECXX_PUBLIC int getVideoMulticastTtl() const;
			
			/**
			 * @brief Use to set multicast ttl to be used for video stream. 
			 *
			 * @param ttl value or -1 if not used. [0..255] default value is 1 
			 * @return 0 in case of success 
			 */
			LINPHONECXX_PUBLIC linphone::Status setVideoMulticastTtl(int ttl);
			
			/**
			 * @brief Return the list of the available video payload types. 
			 *
			 * @return A freshly allocated list of the available payload types. The list must
			 * be destroyed with bctbx_list_free() after usage. The elements of the list
			 * haven't to be unref.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::PayloadType>> getVideoPayloadTypes();
			
			/**
			 * @brief Redefine the list of the available video payload types. 
			 *
			 * @param payloadTypes The new list of codecs. The core does not take ownership on
			 * it.    
			 */
			LINPHONECXX_PUBLIC void setVideoPayloadTypes(const std::list<std::shared_ptr<linphone::PayloadType>> & payloadTypes);
			
			/**
			 * @brief Gets the UDP port used for video streaming. 
			 *
			 * @return The UDP port used for video streaming 
			 */
			LINPHONECXX_PUBLIC int getVideoPort() const;
			
			/**
			 * @brief Sets the UDP port used for video streaming. 
			 *
			 * A value of -1 will request the system to allocate the local port randomly. This
			 * is recommended in order to avoid firewall warnings. 
			 * @param port The UDP port to use for video streaming 
			 */
			LINPHONECXX_PUBLIC void setVideoPort(int port);
			
			/**
			 * @brief Get the video port range from which is randomly chosen the UDP port used
			 * for video streaming. 
			 *
			 * @return a @ref Range object     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Range> getVideoPortsRange() const;
			
			/**
			 * @brief Get the video preset used for video calls. 
			 *
			 * @return The name of the video preset used for video calls (can be nullptr if
			 * the default video preset is used).   
			 */
			LINPHONECXX_PUBLIC std::string getVideoPreset() const;
			
			/**
			 * @brief Set the video preset to be used for video calls. 
			 *
			 * @param preset The name of the video preset to be used (can be nullptr to use
			 * the default video preset).   
			 */
			LINPHONECXX_PUBLIC void setVideoPreset(const std::string & preset);
			
			/**
			 * @brief Tells whether video preview is enabled. 
			 *
			 * @return A boolean value telling whether video preview is enabled 
			 */
			LINPHONECXX_PUBLIC bool videoPreviewEnabled() const;
			
			/**
			 * @brief Controls video preview enablement. 
			 *
			 * @param enable A boolean value telling whether the video preview is to be shown
			 * Video preview refers to the action of displaying the local webcam image to the
			 * user while not in call. 
			 */
			LINPHONECXX_PUBLIC void enableVideoPreview(bool enable);
			
			/**
			 * @brief Enable or disable video source reuse when switching from preview to
			 * actual video call. 
			 *
			 * This source reuse is useful when you always display the preview, even before
			 * calls are initiated. By keeping the video source for the transition to a real
			 * video call, you will smooth out the source close/reopen cycle.
			 * This function does not have any effect durfing calls. It just indicates the
			 * @ref Core to initiate future calls with video source reuse or not. Also, at the
			 * end of a video call, the source will be closed whatsoever for now. 
			 * @param enable true to enable video source reuse. false to disable it for
			 * subsequent calls. 
			 */
			LINPHONECXX_PUBLIC void enableVideoSourceReuse(bool enable);
			
			/**
			 * @brief Tells whether Wifi only mode is enabled or not. 
			 *
			 * @warning Only works for Android platform. 
			 * @return A boolean value telling whether Wifi only mode is enabled or not 
			 */
			LINPHONECXX_PUBLIC bool wifiOnlyEnabled();
			
			/**
			 * @brief Turns Wifi only mode on or off. 
			 *
			 * If enabled, app won't register when active network isn't WiFi or Ethernet.
			 * @warning Only works for Android platform. 
			 * @param enable A boolean value telling whether to enable IPv6 support 
			 */
			LINPHONECXX_PUBLIC void enableWifiOnly(bool enable);
			
			/**
			 * @brief Get a pointer to the sqlite db holding zrtp/lime cache. 
			 *
			 * @return An sqlite3 pointer cast to a void one or nullptr if cache is not
			 * available(not enabled at compile or access failed)   
			 */
			LINPHONECXX_PUBLIC void * getZrtpCacheDb();
			
			/**
			 * @brief Get the path to the file storing the zrtp secrets cache. 
			 *
			 * @return The path to the file storing the zrtp secrets cache.   
			 */
			LINPHONECXX_PUBLIC std::string getZrtpSecretsFile();
			
			/**
			 * @brief Set the path to the file storing the zrtp secrets cache. 
			 *
			 * @param file The path to the file to use to store the zrtp secrets cache.   
			 */
			LINPHONECXX_PUBLIC void setZrtpSecretsFile(const std::string & file);
			
			/**
			 * @brief Special function to indicate if the audio session is activated. 
			 *
			 * Must be called when ProviderDelegate of the callkit notifies that the audio
			 * session is activated or deactivated. 
			 */
			LINPHONECXX_PUBLIC void activateAudioSession(bool actived);
			
			/**
			 * @brief Add all current calls into the conference. 
			 *
			 * If no conference is running a new internal conference context is created and
			 * all current calls are added to it. 
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status addAllToConference();
			
			/**
			 * @brief Adds authentication information to the @ref Core. 
			 *
			 * That piece of information will be used during all SIP transactions that require
			 * authentication. 
			 * @param info The @ref AuthInfo to add.   
			 */
			LINPHONECXX_PUBLIC void addAuthInfo(const std::shared_ptr<const linphone::AuthInfo> & info);
			
			/**
			 * @brief Add support for the specified content type. 
			 *
			 * It is the application responsibility to handle it correctly afterwards. 
			 * @param contentType The content type to add support for   
			 */
			LINPHONECXX_PUBLIC void addContentTypeSupport(const std::string & contentType);
			
			/**
			 * @brief Add a friend list. 
			 *
			 * @param list @ref FriendList object   
			 */
			LINPHONECXX_PUBLIC void addFriendList(const std::shared_ptr<linphone::FriendList> & list);
			
			/**
			 * @brief Add the given linphone specs to the list of functionalities the linphone
			 * client supports. 
			 *
			 * @param spec The spec to add   
			 */
			LINPHONECXX_PUBLIC void addLinphoneSpec(const std::string & spec);
			
			/**
			 * @brief Add a proxy configuration. 
			 *
			 * This will start registration on the proxy, if registration is enabled. 
			 * @param config the @ref ProxyConfig to add   
			 * @return 0 if successful, -1 otherwise 
			 */
			LINPHONECXX_PUBLIC linphone::Status addProxyConfig(const std::shared_ptr<linphone::ProxyConfig> & config);
			
			/**
			 * @brief This function controls signaling features supported by the core. 
			 *
			 * They are typically included in a SIP Supported header. 
			 * @param tag The feature tag name   
			 */
			LINPHONECXX_PUBLIC void addSupportedTag(const std::string & tag);
			
			/**
			 * @brief Add a participant to the conference. 
			 *
			 * If no conference is going on a new internal conference context is created and
			 * the participant is added to it. 
			 * @param call The current call with the participant to add   
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status addToConference(const std::shared_ptr<linphone::Call> & call);
			
			/**
			 * @brief Special function to indicate if the audio route is changed. 
			 *
			 * Must be called in the callback of AVAudioSessionRouteChangeNotification. 
			 * @deprecated 07/01/2020 now handled in the linphone SDK directly 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED void audioRouteChanged();
			
			/**
			 * @brief Checks if a new version of the application is available. 
			 *
			 * @param currentVersion The current version of the application   
			 */
			LINPHONECXX_PUBLIC void checkForUpdate(const std::string & currentVersion);
			
			/**
			 * @brief Clear all authentication information. 
			 *
			 */
			LINPHONECXX_PUBLIC void clearAllAuthInfo();
			
			/**
			 * @brief Erase the call log. 
			 *
			 */
			LINPHONECXX_PUBLIC void clearCallLogs();
			
			/**
			 * @brief Erase all proxies from config. 
			 *
			 */
			LINPHONECXX_PUBLIC void clearProxyConfig();
			
			/**
			 * @brief Special function to configure audio session with default settings. 
			 *
			 * Must be called in ProviderDelegate's callbacks when answer an incoming call and
			 * start an outgoing call. 
			 */
			LINPHONECXX_PUBLIC void configureAudioSession();
			
			/**
			 * @brief Create a @ref AccountCreator and set Linphone Request callbacks. 
			 *
			 * @param xmlrpcUrl The URL to the XML-RPC server.   
			 * @return The new @ref AccountCreator object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::AccountCreator> createAccountCreator(const std::string & xmlrpcUrl);
			
			/**
			 * @brief Create a @ref Address object by parsing the user supplied address, given
			 * as a string. 
			 *
			 * @param address String containing the user supplied address   
			 * @return The create @ref Address object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Address> createAddress(const std::string & address);
			
			/**
			 * @brief Creates a fake @ref CallLog. 
			 *
			 * @param quality call quality 
			 * @return a @ref CallLog object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallLog> createCallLog(const std::shared_ptr<linphone::Address> & from, const std::shared_ptr<linphone::Address> & to, linphone::Call::Dir dir, int duration, time_t startTime, time_t connectedTime, linphone::Call::Status status, bool videoEnabled, float quality);
			
			/**
			 * @brief Create a @ref CallParams suitable for inviteWithParams(),
			 * linphone_core_accept_call_with_params,
			 * linphone_core_accept_early_media_with_params or
			 * linphone_core_accept_call_update. 
			 *
			 * The parameters are initialized according to the current @ref Core configuration
			 * and the last used local @ref CallParams, the ones passed through
			 * Call::update(), Call::acceptWithParams() or
			 * linphone_call_accept_update_with_params(). 
			 * @param call @ref Call for which the parameters are to be build, or nullptr in
			 * the case where the parameters are to be used for a new outgoing call.   
			 * @return A new @ref CallParams object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallParams> createCallParams(const std::shared_ptr<linphone::Call> & call);
			
			/**
			 * @brief Create a chat room. 
			 *
			 * @param participants The initial list of participants of the chat room.    
			 * @return The newly created chat room (can be an existing one if backend is
			 * Basic) or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatRoom> createChatRoom(const std::shared_ptr<const linphone::ChatRoomParams> & params, const std::shared_ptr<const linphone::Address> & localAddr, const std::list<std::shared_ptr<linphone::Address>> & participants);
			
			/**
			 * @brief Create a chat room. 
			 *
			 * @param participants The initial list of participants of the chat room    
			 * @return The newly created chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createChatRoom(const std::shared_ptr<const linphone::ChatRoomParams> & params, const std::shared_ptr<const linphone::Address> & localAddr, const std::string & subject, const std::list<std::shared_ptr<linphone::Address>> & participants);
			
			/**
			 * @brief Create a chat room. 
			 *
			 * @param participants The initial list of participants of the chat room.    
			 * @return The newly created chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createChatRoom(const std::shared_ptr<const linphone::ChatRoomParams> & params, const std::string & subject, const std::list<std::shared_ptr<linphone::Address>> & participants);
			
			/**
			 *
			 * @param participants The initial list of participants of the chat room.    
			 * @return The newly created chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createChatRoom(const std::string & subject, const std::list<std::shared_ptr<linphone::Address>> & participants);
			
			/**
			 *
			 * @param participant @ref Address representing the initial participant to add to
			 * the chat room   
			 * @return The newly created chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createChatRoom(const std::shared_ptr<const linphone::ChatRoomParams> & params, const std::shared_ptr<const linphone::Address> & localAddr, const std::shared_ptr<const linphone::Address> & participant);
			
			/**
			 *
			 * @param participant @ref Address representing the initial participant to add to
			 * the chat room   
			 * @return The newly created chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createChatRoom(const std::shared_ptr<const linphone::Address> & participant);
			
			/**
			 * @brief Create a client-side group chat room. 
			 *
			 * When calling this function the chat room is only created at the client-side and
			 * is empty. You need to call ChatRoom::addParticipants() to create at the server
			 * side and add participants to it. Also, the created chat room will not be a
			 * one-to-one chat room even if ChatRoom::addParticipants() is called with only
			 * one participant.
			 * @param fallback Boolean value telling whether we should plan on being able to
			 * fallback to a basic chat room if the client-side group chat room creation fails 
			 * @return The newly created client-side group chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createClientGroupChatRoom(const std::string & subject, bool fallback);
			
			/**
			 * @brief Create a client-side group chat room. 
			 *
			 * When calling this function the chat room is only created at the client-side and
			 * is empty. You need to call ChatRoom::addParticipants() to create at the server
			 * side and add participants to it. Also, the created chat room will not be a
			 * one-to-one chat room even if ChatRoom::addParticipants() is called with only
			 * one participant.
			 * @param encrypted Boolean value telling whether we should apply encryption or
			 * not on chat messages sent and received on this room. 
			 * @return The newly created client-side group chat room.   
			 * @deprecated 02/07/2020, use createChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> createClientGroupChatRoom(const std::string & subject, bool fallback, bool encrypted);
			
			/**
			 * @brief Create some default conference parameters for instanciating a a
			 * conference with createConferenceWithParams(). 
			 *
			 * @return conference parameters.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ConferenceParams> createConferenceParams();
			
			/**
			 * @brief Create a conference. 
			 *
			 * @param params Parameters of the conference. See @ref ConferenceParams.   
			 * @return A pointer on the freshly created conference. That object will be
			 * automatically freed by the core after calling terminateConference().   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Conference> createConferenceWithParams(const std::shared_ptr<const linphone::ConferenceParams> & params);
			
			/**
			 * @brief Create a @ref Config object from a user config file. 
			 *
			 * @param filename The filename of the config file to read to fill the
			 * instantiated @ref Config   
			 * @return a @ref Config object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Config> createConfig(const std::string & filename);
			
			/**
			 * @brief Create a content with default values from Linphone core. 
			 *
			 * @return @ref Content object with default values set   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Content> createContent();
			
			/**
			 * @brief Creates and returns the default chat room parameters. 
			 *
			 * @return A @ref ChatRoomParams object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatRoomParams> createDefaultChatRoomParams();
			
			/**
			 * @brief Create a default LinphoneFriend. 
			 *
			 * @return The created @ref Friend object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> createFriend();
			
			/**
			 * @brief Create a new empty @ref FriendList object. 
			 *
			 * @return A new @ref FriendList object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::FriendList> createFriendList();
			
			/**
			 * @brief Create a @ref Friend from the given address. 
			 *
			 * @param address A string containing the address to create the @ref Friend from   
			 * @return The created @ref Friend object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> createFriendWithAddress(const std::string & address);
			
			/**
			 * @brief Creates an empty info message. 
			 *
			 * @return a new LinphoneInfoMessage.  
			 * The info message can later be filled with information using
			 * InfoMessage::addHeader() or InfoMessage::setContent(), and finally sent with
			 * linphone_core_send_info_message(). 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::InfoMessage> createInfoMessage();
			
			/**
			 * @brief Create an independent media file player. 
			 *
			 * This player support WAVE and MATROSKA formats. 
			 * @param windowId Id of the drawing window. Depend of video out   
			 * @return A pointer on the new instance. nullptr if failed.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Player> createLocalPlayer(const std::string & soundCardName, const std::string & videoDisplayName, void * windowId);
			
			/**
			 * @brief Create a @ref MagicSearch object. 
			 *
			 * @return The created @ref MagicSearch object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::MagicSearch> createMagicSearch();
			
			/**
			 * @brief Create a new @ref NatPolicy object with every policies being disabled. 
			 *
			 * @return A new @ref NatPolicy object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::NatPolicy> createNatPolicy();
			
			/**
			 * @brief Create a new @ref NatPolicy by reading the config of a @ref Core
			 * according to the passed ref. 
			 *
			 * @param ref The reference of a NAT policy in the config of the @ref Core   
			 * @return A new @ref NatPolicy object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::NatPolicy> createNatPolicyFromConfig(const std::string & ref);
			
			/**
			 * @brief Create an out-of-dialog notification, specifying the destination
			 * resource, the event name. 
			 *
			 * The notification can be send with Event::notify(). 
			 * @param event the event name   
			 * @return a @ref Event holding the context of the notification.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> createNotify(const std::shared_ptr<const linphone::Address> & resource, const std::string & event);
			
			/**
			 * @brief Create a publish context for a one-shot publish. 
			 *
			 * After being created, the publish must be sent using Event::sendPublish(). The
			 * @ref Event is automatically terminated when the publish transaction is
			 * finished, either with success or failure. The application must not call
			 * Event::terminate() for such one-shot publish. 
			 * @param event the event name   
			 * @return the @ref Event holding the context of the publish.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> createOneShotPublish(const std::shared_ptr<const linphone::Address> & resource, const std::string & event);
			
			/**
			 * @brief Create a @ref PresenceActivity with the given type and description. 
			 *
			 * @param description An additional description of the activity to set for the
			 * activity. Can be nullptr if no additional description is to be added.   
			 * @return The created @ref PresenceActivity object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceActivity> createPresenceActivity(linphone::PresenceActivity::Type acttype, const std::string & description);
			
			/**
			 * @brief Create a default LinphonePresenceModel. 
			 *
			 * @return The created @ref PresenceModel object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceModel> createPresenceModel();
			
			/**
			 * @brief Create a @ref PresenceModel with the given activity type and activity
			 * description. 
			 *
			 * @param description An additional description of the activity to set for the
			 * activity. Can be nullptr if no additional description is to be added.   
			 * @return The created @ref PresenceModel object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceModel> createPresenceModelWithActivity(linphone::PresenceActivity::Type acttype, const std::string & description);
			
			/**
			 * @brief Create a @ref PresenceModel with the given activity type, activity
			 * description, note content and note language. 
			 *
			 * @param lang The language of the note to be added to the created model.   
			 * @return The created @ref PresenceModel object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceModel> createPresenceModelWithActivityAndNote(linphone::PresenceActivity::Type acttype, const std::string & description, const std::string & note, const std::string & lang);
			
			/**
			 * @brief Create a @ref PresenceNote with the given content and language. 
			 *
			 * @param lang The language of the note to be created.   
			 * @return The created @ref PresenceNote object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceNote> createPresenceNote(const std::string & content, const std::string & lang);
			
			/**
			 * @brief Create a @ref PresencePerson with the given id. 
			 *
			 * @param id The id of the person to be created.   
			 * @return The created @ref PresencePerson object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresencePerson> createPresencePerson(const std::string & id);
			
			/**
			 * @brief Create a @ref PresenceService with the given id, basic status and
			 * contact. 
			 *
			 * @param contact A string containing a contact information corresponding to the
			 * service to be created.   
			 * @return The created @ref PresenceService object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PresenceService> createPresenceService(const std::string & id, linphone::PresenceBasicStatus basicStatus, const std::string & contact);
			
			/**
			 * @brief Same as getPrimaryContact() but the result is a @ref Address object
			 * instead of const char *. 
			 *
			 * @return a @ref Address object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Address> createPrimaryContactParsed();
			
			/**
			 * @brief Create a proxy config with default values from Linphone core. 
			 *
			 * @return @ref ProxyConfig with default values set   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ProxyConfig> createProxyConfig();
			
			/**
			 * @brief Create a publish context for an event state. 
			 *
			 * After being created, the publish must be sent using Event::sendPublish(). After
			 * expiry, the publication is refreshed unless it is terminated before. 
			 * @param expires the lifetime of event being published, -1 if no associated
			 * duration, in which case it will not be refreshed. 
			 * @return the @ref Event holding the context of the publish.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> createPublish(const std::shared_ptr<const linphone::Address> & resource, const std::string & event, int expires);
			
			/**
			 * @brief Create an outgoing subscription, specifying the destination resource,
			 * the event name, and an optional content body. 
			 *
			 * If accepted, the subscription runs for a finite period, but is automatically
			 * renewed if not terminated before. Unlike subscribe() the subscription isn't
			 * sent immediately. It will be send when calling Event::sendSubscribe(). 
			 * @param expires the whished duration of the subscription 
			 * @return a @ref Event holding the context of the created subcription.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> createSubscribe(const std::shared_ptr<const linphone::Address> & resource, const std::string & event, int expires);
			
			/**
			 * @brief Create an outgoing subscription, specifying the destination resource,
			 * the event name, and an optional content body. 
			 *
			 * If accepted, the subscription runs for a finite period, but is automatically
			 * renewed if not terminated before. Unlike subscribe() the subscription isn't
			 * sent immediately. It will be send when calling Event::sendSubscribe(). 
			 * @param expires the whished duration of the subscription 
			 * @return a @ref Event holding the context of the created subcription.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> createSubscribe(const std::shared_ptr<const linphone::Address> & resource, const std::shared_ptr<linphone::ProxyConfig> & proxy, const std::string & event, int expires);
			
			/**
			 * @brief Create a @ref XmlRpcSession for a given url. 
			 *
			 * @param url The URL to the XML-RPC server. Must be NON nullptr.   
			 * @return The new @ref XmlRpcSession object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::XmlRpcSession> createXmlRpcSession(const std::string & url);
			
			/**
			 * @brief Removes a chatroom including all message history from the LinphoneCore. 
			 *
			 * @param chatRoom A @ref ChatRoom object   
			 */
			LINPHONECXX_PUBLIC void deleteChatRoom(const std::shared_ptr<linphone::ChatRoom> & chatRoom);
			
			/**
			 * @brief Inconditionnaly disable incoming chat messages. 
			 *
			 * @param denyReason the deny reason (@ref Reason::None has no effect). 
			 */
			LINPHONECXX_PUBLIC void disableChat(linphone::Reason denyReason);
			
			/**
			 * @brief Enable reception of incoming chat messages. 
			 *
			 * By default it is enabled but it can be disabled with disableChat(). 
			 */
			LINPHONECXX_PUBLIC void enableChat();
			
			/**
			 * @brief Tells to @ref Core to use Linphone Instant Messaging encryption. 
			 *
			 * @param enable The new lime state
			 * @deprecated 04/02/2019 Use linphone_core_enable_lime_x3dh instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED void enableLime(linphone::LimeState enable);
			
			/**
			 * @brief Call this method when you receive a push notification (if you handle
			 * push notifications manually). 
			 *
			 * It will ensure the proxy configs are correctly registered to the proxy server,
			 * so the call or the message will be correctly delivered. 
			 */
			LINPHONECXX_PUBLIC void ensureRegistered();
			
			/**
			 * @brief This method is called by the application to notify the linphone core
			 * library when it enters background mode. 
			 *
			 */
			LINPHONECXX_PUBLIC void enterBackground();
			
			/**
			 * @brief Join the local participant to the running conference. 
			 *
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status enterConference();
			
			/**
			 * @brief This method is called by the application to notify the linphone core
			 * library when it enters foreground mode. 
			 *
			 */
			LINPHONECXX_PUBLIC void enterForeground();
			
			/**
			 * @brief Returns whether a specific file format is supported. 
			 *
			 * @see linphone_core_get_supported_file_formats 
			 * @param fmt The format extension (wav, mkv).   
			 * @return true if the file format is supported, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool fileFormatSupported(const std::string & fmt);
			
			/**
			 * @brief Find authentication info matching realm, username, domain criteria. 
			 *
			 * First of all, (realm,username) pair are searched. If multiple results (which
			 * should not happen because realm are supposed to be unique), then domain is
			 * added to the search. 
			 * @param sipDomain the SIP domain name (optional)   
			 * @return a @ref AuthInfo if found.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<const linphone::AuthInfo> findAuthInfo(const std::string & realm, const std::string & username, const std::string & sipDomain);
			
			/**
			 * @brief Search from the list of current calls if a remote address match uri. 
			 *
			 * @param uri which should match call remote uri   
			 * @return @ref Call or nullptr is no match is found.   
			 * @deprecated 27/10/2020. Use getCallByRemoteAddress2() instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::Call> findCallFromUri(const std::string & uri) const;
			
			/**
			 * @brief Get the call log matching the call id, or nullptr if can't be found. 
			 *
			 * @param callId Call id of the call log to find   
			 * @return A call log matching the call id if any.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::CallLog> findCallLogFromCallId(const std::string & callId);
			
			/**
			 * @brief Find a chat room. 
			 *
			 * No reference is transfered to the application. The @ref Core keeps a reference
			 * on the chat room. 
			 * @param localAddr a linphone address.   
			 * @return @ref ChatRoom where messaging can take place.   
			 * @deprecated 02/07/2020, use searchChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> findChatRoom(const std::shared_ptr<const linphone::Address> & peerAddr, const std::shared_ptr<const linphone::Address> & localAddr) const;
			
			/**
			 * @brief Retrieves a list of @ref Address sort and filter. 
			 *
			 * @param sipOnly Only sip address or not 
			 * @return A list of filtered @ref Address + the @ref Address created with the
			 * filter.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Address>> findContactsByChar(const std::string & filter, bool sipOnly);
			
			/**
			 * @brief Search a @ref Friend by its address. 
			 *
			 * @param address The @ref Address to use to search the friend.   
			 * @return The @ref Friend object corresponding to the given address or nullptr if
			 * not found.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> findFriend(const std::shared_ptr<const linphone::Address> & address) const;
			
			/**
			 * @brief Search a @ref Friend by its phone number. 
			 *
			 * @param phoneNumber The phone number to use to search the friend.   
			 * @return The @ref Friend object corresponding to the given phone number or
			 * nullptr if not found.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> findFriendByPhoneNumber(const std::string & phoneNumber) const;
			
			/**
			 * @brief Search all @ref Friend matching an address. 
			 *
			 * @param address The address to use to search the friends.   
			 * @return A list of @ref Friend corresponding to the given address.      
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::Friend>> findFriends(const std::shared_ptr<const linphone::Address> & address) const;
			
			/**
			 * @brief Find a one to one chat room. 
			 *
			 * No reference is transfered to the application. The @ref Core keeps a reference
			 * on the chat room. 
			 * @param participantAddr a linphone address.   
			 * @return @ref ChatRoom where messaging can take place.   
			 * @deprecated 12/12/2018, use linphone_core_find_one_to_one_chat_room_2 instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> findOneToOneChatRoom(const std::shared_ptr<const linphone::Address> & localAddr, const std::shared_ptr<const linphone::Address> & participantAddr) const;
			
			/**
			 * @brief Find a one to one chat room. 
			 *
			 * No reference is transfered to the application. The @ref Core keeps a reference
			 * on the chat room. 
			 * @param encrypted whether to look for an encrypted chat room or not 
			 * @return @ref ChatRoom where messaging can take place.   
			 * @deprecated 02/07/2020, use searchChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> findOneToOneChatRoom(const std::shared_ptr<const linphone::Address> & localAddr, const std::shared_ptr<const linphone::Address> & participantAddr, bool encrypted) const;
			
			/**
			 * @brief Get the call by callid. 
			 *
			 * @return call @ref Call, return null if there is no call find.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getCallByCallid(const std::string & callId) const;
			
			/**
			 * @brief Get the call with the remote_address specified. 
			 *
			 * @param remoteAddress The remote address of the call that we want to get   
			 * @return The call if it has been found, nullptr otherwise.  
			 * @deprecated 08/07/2020 use getCallByRemoteAddress2() instead 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::Call> getCallByRemoteAddress(const std::string & remoteAddress) const;
			
			/**
			 * @brief Get the call with the specified @ref Address. 
			 *
			 * @param remoteAddress the @ref Address for which the call remote address must
			 * match   
			 * @return the @ref Call of the call if found.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> getCallByRemoteAddress2(const std::shared_ptr<const linphone::Address> & remoteAddress) const;
			
			/**
			 * @brief Get the list of call logs (past calls). 
			 *
			 * At the contrary of linphone_core_get_call_logs, it is your responsibility to
			 * unref the logs and free this list once you are done using it. 
			 * @param localAddress The local @ref Address object   
			 * @return A list of @ref CallLog.      
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::CallLog>> getCallHistory(const std::shared_ptr<const linphone::Address> & peerAddress, const std::shared_ptr<const linphone::Address> & localAddress);
			
			/**
			 * @brief Get the list of call logs (past calls) that matches the given @ref
			 * Address. 
			 *
			 * At the contrary of linphone_core_get_call_logs, it is your responsibility to
			 * unref the logs and free this list once you are done using it. 
			 * @param address @ref Address object   
			 * @return A list of @ref CallLog.      
			 * @deprecated 29/10/2018 Use getCallHistory() instead.
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::list<std::shared_ptr<linphone::CallLog>> getCallHistoryForAddress(const std::shared_ptr<const linphone::Address> & address);
			
			/**
			 * @brief Get a chat room whose peer is the supplied address. 
			 *
			 * If it does not exist yet, it will be created as a basic chat room. No reference
			 * is transfered to the application. The @ref Core keeps a reference on the chat
			 * room. @warning This method is prone to errors, use searchChatRoom() instead 
			 * @param addr a linphone address.   
			 * @return @ref ChatRoom where messaging can take place.   
			 * @deprecated 02/07/2020, use searchChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> getChatRoom(const std::shared_ptr<const linphone::Address> & addr);
			
			/**
			 * @brief Get a chat room. 
			 *
			 * If it does not exist yet, it will be created as a basic chat room. No reference
			 * is transfered to the application. The @ref Core keeps a reference on the chat
			 * room. @warning This method is prone to errors, use searchChatRoom() instead 
			 * @param localAddr a linphone address.   
			 * @return @ref ChatRoom where messaging can take place.   
			 * @deprecated 02/07/2020, use searchChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> getChatRoom(const std::shared_ptr<const linphone::Address> & peerAddr, const std::shared_ptr<const linphone::Address> & localAddr);
			
			/**
			 * @brief Get a chat room for messaging from a sip uri like
			 * sip:joe@sip.linphone.org. 
			 *
			 * If it does not exist yet, it will be created as a basic chat room. No reference
			 * is transfered to the application. The @ref Core keeps a reference on the chat
			 * room. @warning This method is prone to errors, use searchChatRoom() instead 
			 * @param to The destination address for messages.   
			 * @return @ref ChatRoom where messaging can take place.   
			 * @deprecated 02/07/2020, use searchChatRoom() instead
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED std::shared_ptr<linphone::ChatRoom> getChatRoomFromUri(const std::string & to);
			
			/**
			 * @brief Search a @ref Friend by its reference key. 
			 *
			 * @param key The reference key to use to search the friend.   
			 * @return The @ref Friend object corresponding to the given reference key.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Friend> getFriendByRefKey(const std::string & key) const;
			
			/**
			 * @brief Retrieves the list of @ref Friend from the core that has the given
			 * display name. 
			 *
			 * @param name the name of the list   
			 * @return the first @ref FriendList object or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::FriendList> getFriendListByName(const std::string & name) const;
			
			/**
			 * @brief Get payload type from mime type and clock rate. 
			 *
			 * This function searches in audio and video codecs for the given payload type
			 * name and clockrate. 
			 * @param channels number of channels, can be
			 * LINPHONE_FIND_PAYLOAD_IGNORE_CHANNELS 
			 * @return Returns nullptr if not found. If a @ref PayloadType is returned, it
			 * must be released with linphone_payload_type_unref after using it.     
			 * @warning The returned payload type is allocated as a floating reference i.e.
			 * the reference counter is initialized to 0. 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::PayloadType> getPayloadType(const std::string & type, int rate, int channels);
			
			/**
			 * @brief Search for a @ref ProxyConfig by it's idkey. 
			 *
			 * @param idkey An arbitrary idkey string associated to a proxy configuration 
			 * @return the @ref ProxyConfig object for the given idkey value, or nullptr if
			 * none found   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ProxyConfig> getProxyConfigByIdkey(const std::string & idkey);
			
			/**
			 * @brief Return the unread chat message count for a given local address. 
			 *
			 * @param address @ref Address object.   
			 * @return The unread chat message count. 
			 */
			LINPHONECXX_PUBLIC int getUnreadChatMessageCountFromLocal(const std::shared_ptr<const linphone::Address> & address) const;
			
			/**
			 * @brief Get the zrtp sas validation status for a peer uri. 
			 *
			 * Once the SAS has been validated or rejected, the status will never return to
			 * Unknown (unless you delete your cache) 
			 * @param addr the peer uri  
			 * @return - LinphoneZrtpPeerStatusUnknown: this uri is not present in cache OR
			 * during calls with the active device, SAS never was validated or rejected
			 */
			LINPHONECXX_PUBLIC linphone::ZrtpPeerStatus getZrtpStatus(const std::string & addr);
			
			/**
			 * @brief Check whether the device has a hardware echo canceller. 
			 *
			 * @return true if it does, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool hasBuiltinEchoCanceller();
			
			/**
			 * @brief Check whether the device is flagged has crappy opengl. 
			 *
			 * @return true if crappy opengl flag is set, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool hasCrappyOpengl();
			
			/**
			 * @brief Tells whether there is a call running. 
			 *
			 * @return A boolean value telling whether a call is currently running or not 
			 */
			LINPHONECXX_PUBLIC bool inCall() const;
			
			/**
			 * @brief Constructs a @ref Address from the given string if possible. 
			 *
			 * In case of just a username, characters will be unescaped. If a phone number is
			 * detected, it will be flattened. sip: or sips: prefix will be added if not
			 * present. Finally,  will be added if not present using default proxy config.
			 * @see ProxyConfig::normalizeSipUri() for documentation. 
			 * @param url the url to parse   
			 * @return the @ref Address matching the url or nullptr in case of failure.     
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Address> interpretUrl(const std::string & url);
			
			/**
			 * @brief Initiates an outgoing call. 
			 *
			 * The application doesn't own a reference to the returned LinphoneCall object.
			 * Use linphone_call_ref to safely keep the LinphoneCall pointer valid within your
			 * application.
			 * @param url The destination of the call (sip address, or phone number).   
			 * @return A @ref Call object or nullptr in case of failure.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> invite(const std::string & url);
			
			/**
			 * @brief Initiates an outgoing call given a destination @ref Address The @ref
			 * Address can be constructed directly using linphone_address_new, or created by
			 * interpretUrl(). 
			 *
			 * The application doesn't own a reference to the returned @ref Call object. Use
			 * linphone_call_ref to safely keep the @ref Call pointer valid within your
			 * application. 
			 * @param addr The destination of the call (sip address).   
			 * @return A @ref Call object or nullptr in case of failure.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> inviteAddress(const std::shared_ptr<const linphone::Address> & addr);
			
			/**
			 * @brief Initiates an outgoing call given a destination @ref Address The @ref
			 * Address can be constructed directly using linphone_address_new, or created by
			 * interpretUrl(). 
			 *
			 * The application doesn't own a reference to the returned @ref Call object. Use
			 * linphone_call_ref to safely keep the @ref Call pointer valid within your
			 * application. If the proxy is not specified in parameters, the caller proxy will
			 * be automatically selected by finding what is the best to reach the destination
			 * of the call. 
			 * @param params Call parameters   
			 * @return A @ref Call object or nullptr in case of failure.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> inviteAddressWithParams(const std::shared_ptr<const linphone::Address> & addr, const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Initiates an outgoing call according to supplied call parameters The
			 * application doesn't own a reference to the returned @ref Call object. 
			 *
			 * Use linphone_call_ref to safely keep the @ref Call pointer valid within your
			 * application. 
			 * @param params the @ref CallParams call parameters   
			 * @return A @ref Call object or nullptr in case of failure.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Call> inviteWithParams(const std::string & url, const std::shared_ptr<const linphone::CallParams> & params);
			
			/**
			 * @brief Tells whether a content type is supported. 
			 *
			 * @param contentType The content type to check   
			 * @return A boolean value telling whether the specified content type is supported
			 * or not. 
			 */
			LINPHONECXX_PUBLIC bool isContentTypeSupported(const std::string & contentType) const;
			
			/**
			 * @brief Checks if the given media filter is loaded and usable. 
			 *
			 * This is for advanced users of the library, mainly to expose mediastreamer video
			 * filter status. 
			 * @param filtername the filter name   
			 * @return true if the filter is loaded and usable, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isMediaFilterSupported(const std::string & filtername);
			
			/**
			 * @brief Main loop function. 
			 *
			 * It is crucial that your application call it periodically.
			 * iterate() performs various backgrounds tasks:
			 */
			LINPHONECXX_PUBLIC void iterate();
			
			/**
			 * @brief Make the local participant leave the running conference. 
			 *
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status leaveConference();
			
			/**
			 * @brief Tells if lime is available. 
			 *
			 * @deprecated 04/02/2019 Use linphone_core_lime_x3dh_available instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED bool limeAvailable() const;
			
			/**
			 * @brief Returns the lime state. 
			 *
			 * @return The current lime state
			 * @deprecated 04/02/2019 Use linphone_core_lime_x3dh_enabled instead. 
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED linphone::LimeState limeEnabled() const;
			
			/**
			 * @brief Tells if LIME X3DH is available. 
			 *
			 */
			LINPHONECXX_PUBLIC bool limeX3DhAvailable() const;
			
			/**
			 * @brief Update current config with the content of a xml config file. 
			 *
			 * @param xmlUri the path to the xml file   
			 */
			LINPHONECXX_PUBLIC void loadConfigFromXml(const std::string & xmlUri);
			
			/**
			 * @brief Check if a media encryption type is supported. 
			 *
			 * @param menc @ref MediaEncryption 
			 * @return whether a media encryption scheme is supported by the @ref Core engine 
			 */
			LINPHONECXX_PUBLIC bool mediaEncryptionSupported(linphone::MediaEncryption menc) const;
			
			/**
			 * @brief Migrates the call logs from the linphonerc to the database if not done
			 * yet. 
			 *
			 */
			LINPHONECXX_PUBLIC void migrateLogsFromRcToDb();
			
			/**
			 * @brief Migrate configuration so that all SIP transports are enabled. 
			 *
			 * Versions of linphone < 3.7 did not support using multiple SIP transport
			 * simultaneously. This function helps application to migrate the configuration so
			 * that all transports are enabled. Existing proxy configuration are added a
			 * transport parameter so that they continue using the unique transport that was
			 * set previously. This function must be used just after creating the core, before
			 * any call to iterate() 
			 * @return 1 if migration was done, 0 if not done because unnecessary or already
			 * done, -1 in case of error. 
			 */
			LINPHONECXX_PUBLIC linphone::Status migrateToMultiTransport();
			
			/**
			 * @brief Notify all friends that have subscribed. 
			 *
			 * @param presence @ref PresenceModel to notify   
			 */
			LINPHONECXX_PUBLIC void notifyAllFriends(const std::shared_ptr<linphone::PresenceModel> & presence);
			
			/**
			 * @brief Notifies the upper layer that a presence status has been received by
			 * calling the appropriate callback if one has been set. 
			 *
			 * This method is for advanced usage, where customization of the liblinphone's
			 * internal behavior is required. 
			 * @param linphoneFriend the @ref Friend whose presence information has been
			 * received.   
			 */
			LINPHONECXX_PUBLIC void notifyNotifyPresenceReceived(const std::shared_ptr<linphone::Friend> & linphoneFriend);
			
			/**
			 * @brief Notifies the upper layer that a presence model change has been received
			 * for the uri or telephone number given as a parameter, by calling the
			 * appropriate callback if one has been set. 
			 *
			 * This method is for advanced usage, where customization of the liblinphone's
			 * internal behavior is required. 
			 * @param presenceModel the @ref PresenceModel that has been modified   
			 */
			LINPHONECXX_PUBLIC void notifyNotifyPresenceReceivedForUriOrTel(const std::shared_ptr<linphone::Friend> & linphoneFriend, const std::string & uriOrTel, const std::shared_ptr<const linphone::PresenceModel> & presenceModel);
			
			/**
			 * @brief Pause all currently running calls. 
			 *
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status pauseAllCalls();
			
			/**
			 * @brief Plays a dtmf sound to the local user. 
			 *
			 * @param durationMs Duration in ms, -1 means play until next further call to
			 * stopDtmf() 
			 */
			LINPHONECXX_PUBLIC void playDtmf(char dtmf, int durationMs);
			
			/**
			 * @brief Plays an audio file to the local user. 
			 *
			 * This function works at any time, during calls, or when no calls are running. It
			 * doesn't request the underlying audio system to support multiple playback
			 * streams. 
			 * @param audiofile The path to an audio file in wav PCM 16 bit format   
			 * @return 0 on success, -1 on error 
			 */
			LINPHONECXX_PUBLIC linphone::Status playLocal(const std::string & audiofile);
			
			/**
			 * @brief Empties sound resources to allow a new call to be accepted. 
			 *
			 * This function is autyomatically called by the core if the media resource mode
			 * is set to unique. 
			 * @return An integer returning the exit value. If it is 0, sound resources have
			 * been emptied. Otherwise, sound resources are busy and cannot be freed
			 * immediately. 
			 */
			LINPHONECXX_PUBLIC int preemptSoundResources();
			
			/**
			 * @brief Call generic OpenGL render for a given core. 
			 *
			 */
			LINPHONECXX_PUBLIC void previewOglRender() const;
			
			/**
			 * @brief Publish an event state. 
			 *
			 * This first create a @ref Event with createPublish() and calls
			 * Event::sendPublish() to actually send it. After expiry, the publication is
			 * refreshed unless it is terminated before. 
			 * @param body the actual published data   
			 * @return the @ref Event holding the context of the publish.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> publish(const std::shared_ptr<const linphone::Address> & resource, const std::string & event, int expires, const std::shared_ptr<const linphone::Content> & body);
			
			/**
			 * @brief Gets keep alive interval of real time text. 
			 *
			 * @return keep alive interval of real time text. 
			 */
			LINPHONECXX_PUBLIC unsigned int realtimeTextGetKeepaliveInterval() const;
			
			/**
			 * @brief Set keep alive interval for real time text. 
			 *
			 * @param interval The keep alive interval of real time text, 25000 by default. 
			 */
			LINPHONECXX_PUBLIC void realtimeTextSetKeepaliveInterval(unsigned int interval);
			
			/**
			 * @brief force registration refresh to be initiated upon next iterate 
			 *
			 */
			LINPHONECXX_PUBLIC void refreshRegisters();
			
			/**
			 * @brief Black list a friend. 
			 *
			 * same as Friend::setIncSubscribePolicy() with @ref SubscribePolicy::SPDeny
			 * policy; 
			 * @param linphoneFriend @ref Friend to reject   
			 */
			LINPHONECXX_PUBLIC void rejectSubscriber(const std::shared_ptr<linphone::Friend> & linphoneFriend);
			
			/**
			 * @brief Reload mediastreamer2 plugins from specified directory. 
			 *
			 * @param path the path from where plugins are to be loaded, pass nullptr to use
			 * default (compile-time determined) plugin directory.   
			 */
			LINPHONECXX_PUBLIC void reloadMsPlugins(const std::string & path);
			
			/**
			 * @brief Update detection of sound devices. 
			 *
			 * Use this function when the application is notified of USB plug events, so that
			 * list of available hardwares for sound playback and capture is updated. 
			 */
			LINPHONECXX_PUBLIC void reloadSoundDevices();
			
			/**
			 * @brief Update detection of camera devices. 
			 *
			 * Use this function when the application is notified of USB plug events, so that
			 * list of available hardwares for video capture is updated. 
			 */
			LINPHONECXX_PUBLIC void reloadVideoDevices();
			
			/**
			 * @brief Removes an authentication information object. 
			 *
			 * @param info The @ref AuthInfo to remove.   
			 */
			LINPHONECXX_PUBLIC void removeAuthInfo(const std::shared_ptr<const linphone::AuthInfo> & info);
			
			/**
			 * @brief Remove a specific call log from call history list. 
			 *
			 * This function destroys the call log object. It must not be accessed anymore by
			 * the application after calling this function. 
			 * @param callLog @ref CallLog object to remove.   
			 */
			LINPHONECXX_PUBLIC void removeCallLog(const std::shared_ptr<linphone::CallLog> & callLog);
			
			/**
			 * @brief Remove support for the specified content type. 
			 *
			 * It is the application responsibility to handle it correctly afterwards. 
			 * @param contentType The content type to remove support for   
			 */
			LINPHONECXX_PUBLIC void removeContentTypeSupport(const std::string & contentType);
			
			/**
			 * @brief Removes a friend list. 
			 *
			 * @param list @ref FriendList object   
			 */
			LINPHONECXX_PUBLIC void removeFriendList(const std::shared_ptr<linphone::FriendList> & list);
			
			/**
			 * @brief Remove a call from the conference. 
			 *
			 * @param call a call that has been previously merged into the conference.  
			 * After removing the remote participant belonging to the supplied call, the call
			 * becomes a normal call in paused state. If one single remote participant is left
			 * alone together with the local user in the conference after the removal, then
			 * the conference is automatically transformed into a simple call in
			 * StreamsRunning state. The conference's resources are then automatically
			 * destroyed.
			 * In other words, unless leaveConference() is explicitly called, the last remote
			 * participant of a conference is automatically put in a simple call in running
			 * state.
			 * @return 0 if successful, -1 otherwise. 
			 */
			LINPHONECXX_PUBLIC linphone::Status removeFromConference(const std::shared_ptr<linphone::Call> & call);
			
			/**
			 * @brief Remove the given linphone specs from the list of functionalities the
			 * linphone client supports. 
			 *
			 * @param spec The spec to remove   
			 */
			LINPHONECXX_PUBLIC void removeLinphoneSpec(const std::string & spec);
			
			/**
			 * @brief Removes a proxy configuration. 
			 *
			 * @ref Core will then automatically unregister and place the proxy configuration
			 * on a deleted list. For that reason, a removed proxy does NOT need to be freed. 
			 * @param config the @ref ProxyConfig to remove   
			 */
			LINPHONECXX_PUBLIC void removeProxyConfig(const std::shared_ptr<linphone::ProxyConfig> & config);
			
			/**
			 * @brief Remove a supported tag. 
			 *
			 * @param tag The tag to remove  
			 * @see addSupportedTag() 
			 */
			LINPHONECXX_PUBLIC void removeSupportedTag(const std::string & tag);
			
			/**
			 * @brief Reset the counter of missed calls. 
			 *
			 */
			LINPHONECXX_PUBLIC void resetMissedCallsCount();
			
			/**
			 * @brief Find a chat room. 
			 *
			 * @param participants The participants that must be present in the chat room to
			 * find.    
			 * @return A matching chat room or nullptr if none matches.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ChatRoom> searchChatRoom(const std::shared_ptr<const linphone::ChatRoomParams> & params, const std::shared_ptr<const linphone::Address> & localAddr, const std::shared_ptr<const linphone::Address> & remoteAddr, const std::list<std::shared_ptr<linphone::Address>> & participants) const;
			
			/**
			 * @brief Sets the UDP port range from which to randomly select the port used for
			 * audio streaming. 
			 *
			 * @param maxPort The upper bound of the audio port range to use 
			 */
			LINPHONECXX_PUBLIC void setAudioPortRange(int minPort, int maxPort);
			
			/**
			 * @brief Assign an audio file to be played locally upon call failure, for a given
			 * reason. 
			 *
			 * @param audiofile a wav file to be played when such call failure happens.   
			 */
			LINPHONECXX_PUBLIC void setCallErrorTone(linphone::Reason reason, const std::string & audiofile);
			
			/**
			 * @brief Set the rectangle where the decoder will search a QRCode. 
			 *
			 * @param h height 
			 */
			LINPHONECXX_PUBLIC void setQrcodeDecodeRect(const int x, const int y, const int w, const int h);
			
			/**
			 * @brief Sets the UDP port range from which to randomly select the port used for
			 * text streaming. 
			 *
			 * @param maxPort The upper bound of the text port range to use 
			 */
			LINPHONECXX_PUBLIC void setTextPortRange(int minPort, int maxPort);
			
			/**
			 * @brief Assign an audio file to be played as a specific tone id. 
			 *
			 * This function typically allows to customize telephony tones per country. 
			 * @param audiofile a wav file to be played.   
			 */
			LINPHONECXX_PUBLIC void setTone(linphone::ToneID toneId, const std::string & audiofile);
			
			/**
			 * @brief Set the user agent string used in SIP messages. 
			 *
			 * Set the user agent string used in SIP messages as "[ua_name]/[version]". No
			 * slash character will be printed if nullptr is given to "version". If nullptr is
			 * given to "ua_name" and "version" both, the User-agent header will be empty.
			 * This function should be called just after linphone_factory_create_core ideally. 
			 * @param version Version of the user agent.   
			 */
			LINPHONECXX_PUBLIC void setUserAgent(const std::string & name, const std::string & version);
			
			/**
			 * @brief Sets the UDP port range from which to randomly select the port used for
			 * video streaming. 
			 *
			 * @param maxPort The upper bound of the video port range to use 
			 */
			LINPHONECXX_PUBLIC void setVideoPortRange(int minPort, int maxPort);
			
			/**
			 * @brief Tells whether a specified sound device can capture sound. 
			 *
			 * @param device the device name as returned by linphone_core_get_sound_devices   
			 * @return A boolean value telling whether the specified sound device can capture
			 * sound 
			 * @deprecated 08/07/2020 use @ref AudioDevice API instead()
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED bool soundDeviceCanCapture(const std::string & device);
			
			/**
			 * @brief Tells whether a specified sound device can play sound. 
			 *
			 * @param device the device name as returned by linphone_core_get_sound_devices   
			 * @return A boolean value telling whether the specified sound device can play
			 * sound 
			 * @deprecated 08/07/2020 use @ref AudioDevice API instead()
			 */
			LINPHONECXX_PUBLIC LINPHONECXX_DEPRECATED bool soundDeviceCanPlayback(const std::string & device);
			
			/**
			 * @brief Check if a call will need the sound resources in near future (typically
			 * an outgoing call that is awaiting response). 
			 *
			 * In liblinphone, it is not possible to have two independant calls using sound
			 * device or camera at the same time. In order to prevent this situation, an
			 * application can use soundResourcesLocked() to know whether it is possible at a
			 * given time to start a new outgoing call. When the function returns true, an
			 * application should not allow the user to start an outgoing call. 
			 * @return A boolean value telling whether a call will need the sound resources in
			 * near future 
			 */
			LINPHONECXX_PUBLIC bool soundResourcesLocked();
			
			/**
			 * @brief Start a @ref Core object after it has been instantiated and not
			 * automatically started. 
			 *
			 * Also re-initialize a @ref Core object that has been stopped using stop(). Must
			 * be called only if @ref GlobalState is either Ready of Off. State will changed
			 * to Startup, Configuring and then On.
			 * @return 0: success, -1: global failure, -2: could not connect database 
			 */
			LINPHONECXX_PUBLIC linphone::Status start();
			
			/**
			 * @brief Start recording the running conference. 
			 *
			 * @param path Path to the file where the recording will be written   
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status startConferenceRecording(const std::string & path);
			
			/**
			 * @brief Starts an echo calibration of the sound devices, in order to find
			 * adequate settings for the echo canceler automatically. 
			 *
			 * @return LinphoneStatus whether calibration has started or not. 
			 */
			LINPHONECXX_PUBLIC linphone::Status startEchoCancellerCalibration();
			
			/**
			 * @brief Start the simulation of call to test the latency with an external
			 * device. 
			 *
			 * @param rate Sound sample rate. 
			 * @return -1 in case of failure, 1 otherwise. 
			 */
			LINPHONECXX_PUBLIC linphone::Status startEchoTester(unsigned int rate);
			
			/**
			 * @brief Stop a @ref Core object after it has been instantiated and started. 
			 *
			 * If stopped, it can be started again using start(). Must be called only if @ref
			 * GlobalState is either On. State will changed to Shutdown and then Off.
			 */
			LINPHONECXX_PUBLIC void stop();
			
			/**
			 * @brief Stop asynchronously a @ref Core object after it has been instantiated
			 * and started. 
			 *
			 * State changes to Shutdown then iterate() must be called to allow the Core to
			 * end asynchronous tasks (terminate call, etc.). When all tasks are finished,
			 * State will change to Off. Must be called only if @ref GlobalState is On. When
			 * @ref GlobalState is Off @ref Core can be started again using start().
			 */
			LINPHONECXX_PUBLIC void stopAsync();
			
			/**
			 * @brief Stop recording the running conference. 
			 *
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status stopConferenceRecording();
			
			/**
			 * @brief Stops playing a dtmf started by playDtmf(). 
			 *
			 */
			LINPHONECXX_PUBLIC void stopDtmf();
			
			/**
			 * @brief Stop the simulation of call. 
			 *
			 */
			LINPHONECXX_PUBLIC linphone::Status stopEchoTester();
			
			/**
			 * @brief Whenever the liblinphone is playing a ring to advertise an incoming call
			 * or ringback of an outgoing call, this function stops the ringing. 
			 *
			 * Typical use is to stop ringing when the user requests to ignore the call. 
			 */
			LINPHONECXX_PUBLIC void stopRinging();
			
			/**
			 * @brief Create an outgoing subscription, specifying the destination resource,
			 * the event name, and an optional content body. 
			 *
			 * If accepted, the subscription runs for a finite period, but is automatically
			 * renewed if not terminated before. 
			 * @param body an optional body, may be nullptr.   
			 * @return a @ref Event holding the context of the created subcription.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Event> subscribe(const std::shared_ptr<const linphone::Address> & resource, const std::string & event, int expires, const std::shared_ptr<const linphone::Content> & body);
			
			/**
			 * @brief Take a photo of currently from capture device and write it into a jpeg
			 * file. 
			 *
			 * Note that the snapshot is asynchronous, an application shall not assume that
			 * the file is created when the function returns.
			 * @param file a path where to write the jpeg content.   
			 * @return 0 if successfull, -1 otherwise (typically if jpeg format is not
			 * supported). 
			 */
			LINPHONECXX_PUBLIC linphone::Status takePreviewSnapshot(const std::string & file);
			
			/**
			 * @brief Terminates all the calls. 
			 *
			 * @return 0 
			 */
			LINPHONECXX_PUBLIC linphone::Status terminateAllCalls();
			
			/**
			 * @brief Terminate the running conference. 
			 *
			 * If it is a local conference, all calls inside it will become back separate
			 * calls and will be put in #LinphoneCallPaused state. If it is a conference
			 * involving a focus server, all calls inside the conference will be terminated. 
			 * @return 0 if succeeded. Negative number if failed 
			 */
			LINPHONECXX_PUBLIC linphone::Status terminateConference();
			
			/**
			 * @brief Upload the log collection to the configured server url. 
			 *
			 */
			LINPHONECXX_PUBLIC void uploadLogCollection();
			
			/**
			 * @brief Tells the core to use a separate window for local camera preview video,
			 * instead of inserting local view within the remote video window. 
			 *
			 * @param yesno true to use a separate window, false to insert the preview in the
			 * remote video window. 
			 */
			LINPHONECXX_PUBLIC void usePreviewWindow(bool yesno);
			
			/**
			 * @brief Specify whether the tls server certificate must be verified when
			 * connecting to a SIP/TLS server. 
			 *
			 * @param yesno A boolean value telling whether the tls server certificate must be
			 * verified 
			 */
			LINPHONECXX_PUBLIC void verifyServerCertificates(bool yesno);
			
			/**
			 * @brief Specify whether the tls server certificate common name must be verified
			 * when connecting to a SIP/TLS server. 
			 *
			 * @param yesno A boolean value telling whether the tls server certificate common
			 * name must be verified 
			 */
			LINPHONECXX_PUBLIC void verifyServerCn(bool yesno);
			
			/**
			 * @brief Test if video is supported. 
			 *
			 * @return true if the library was built with video support, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool videoSupported();
			
			;
			/**
			 * @brief Compress the log collection in a single file. 
			 *
			 * @return The path of the compressed log collection file (to be freed calling
			 * ms_free()).     
			 */
			LINPHONECXX_PUBLIC static std::string compressLogCollection();
			
;
			/**
			 * @brief Enable the linphone core log collection to upload logs on a server. 
			 *
			 * @param state @ref LogCollectionState value telling whether to enable log
			 * collection or not. 
			 */
			LINPHONECXX_PUBLIC static void enableLogCollection(linphone::LogCollectionState state);
			
;
			/**
			 * @brief Get the max file size in bytes of the files used for log collection. 
			 *
			 * @return The max file size in bytes of the files used for log collection.   
			 */
			LINPHONECXX_PUBLIC static size_t getLogCollectionMaxFileSize();
			
;
			/**
			 * @brief Get the path where the log files will be written for log collection. 
			 *
			 * @return The path where the log files will be written.   
			 */
			LINPHONECXX_PUBLIC static std::string getLogCollectionPath();
			
;
			/**
			 * @brief Get the prefix of the filenames that will be used for log collection. 
			 *
			 * @return The prefix of the filenames used for log collection.   
			 */
			LINPHONECXX_PUBLIC static std::string getLogCollectionPrefix();
			
;
			/**
			 * @brief Returns liblinphone's version as a string. 
			 *
			 * @return the current version of the @ref Core   
			 */
			LINPHONECXX_PUBLIC static std::string getVersion();
			
;
			/**
			 * @brief Tells whether the linphone core log collection is enabled. 
			 *
			 * @return The @ref LogCollectionState of the @ref Core log collection. 
			 */
			LINPHONECXX_PUBLIC static linphone::LogCollectionState logCollectionEnabled();
			
;
			/**
			 * @brief Reset the log collection by removing the log files. 
			 *
			 */
			LINPHONECXX_PUBLIC static void resetLogCollection();
			
;
			/**
			 * @brief Enable logs serialization (output logs from either the thread that
			 * creates the linphone core or the thread that calls iterate()). 
			 *
			 * Must be called before creating the linphone core. 
			 */
			LINPHONECXX_PUBLIC static void serializeLogs();
			
;
			/**
			 * @brief Set the max file size in bytes of the files used for log collection. 
			 *
			 * Warning: this function should only not be used to change size dynamically but
			 * instead only before calling enableLogCollection(). If you increase max size on
			 * runtime, logs chronological order COULD be broken. 
			 * @param size The max file size in bytes of the files used for log collection. 
			 */
			LINPHONECXX_PUBLIC static void setLogCollectionMaxFileSize(size_t size);
			
;
			/**
			 * @brief Set the path of a directory where the log files will be written for log
			 * collection. 
			 *
			 * @param path The path where the log files will be written.   
			 */
			LINPHONECXX_PUBLIC static void setLogCollectionPath(const std::string & path);
			
;
			/**
			 * @brief Set the prefix of the filenames that will be used for log collection. 
			 *
			 * @param prefix The prefix to use for the filenames for log collection.   
			 */
			LINPHONECXX_PUBLIC static void setLogCollectionPrefix(const std::string & prefix);
			
;
			/**
			 * @brief True if tunnel support was compiled. 
			 *
			 * @return true if library was built with tunnel, false otherwise 
			 */
			LINPHONECXX_PUBLIC static bool tunnelAvailable();
			
;
			/**
			 * @brief Return the availability of uPnP. 
			 *
			 * @return true if uPnP is available otherwise return false. 
			 */
			LINPHONECXX_PUBLIC static bool upnpAvailable();
			
;
			/**
			 * @brief Tells whether VCARD support is builtin. 
			 *
			 * @return true if VCARD is supported, false otherwise. 
			 */
			LINPHONECXX_PUBLIC static bool vcardSupported();
			

		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_CORE_HH
