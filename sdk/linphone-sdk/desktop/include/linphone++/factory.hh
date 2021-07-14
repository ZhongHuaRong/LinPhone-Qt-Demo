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

#ifndef _LINPHONE_FACTORY_HH
#define _LINPHONE_FACTORY_HH

#include <string>
#include <list>
#include <cstdint>
#include "enums.hh"
#include "object.hh"




struct _LinphoneFactory;


namespace linphone {

	class DialPlan;
	class VideoDefinition;
	class Factory;
	class Address;
	class AuthInfo;
	class Buffer;
	class Config;
	class Content;
	class Core;
	class ErrorInfo;
	class ParticipantDeviceIdentity;
	class Range;
	class Transports;
	class TunnelConfig;
	class Vcard;
	class VideoActivationPolicy;

	/**
	 * @brief The factory is a singleton object devoted to the creation of all the
	 * objects of Liblinphone that cannot be created by @ref Core itself. 
	 *
	 * It is also used to configure a few behaviors before creating the @ref Core,
	 * like the logs verbosity or collection. 
	 */
	class Factory: public Object {
	
		public:

			Factory(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneFactory *cPtr() {return (_LinphoneFactory *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Get the directory where the data resources are located. 
			 *
			 * @return The path to the directory where the data resources are located   
			 */
			LINPHONECXX_PUBLIC std::string getDataResourcesDir();
			
			/**
			 * @brief Set the directory where the data resources are located. 
			 *
			 * @param path The path where the data resources are located   
			 */
			LINPHONECXX_PUBLIC void setDataResourcesDir(const std::string & path);
			
			/**
			 * @brief Returns a bctbx_list_t of all DialPlans. 
			 *
			 * @return A list of @ref DialPlan    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::DialPlan>> getDialPlans() const;
			
			/**
			 * @brief Get the directory where the image resources are located. 
			 *
			 * @return The path to the directory where the image resources are located   
			 */
			LINPHONECXX_PUBLIC std::string getImageResourcesDir();
			
			/**
			 * @brief Set the directory where the image resources are located. 
			 *
			 * @param path The path where the image resources are located   
			 */
			LINPHONECXX_PUBLIC void setImageResourcesDir(const std::string & path);
			
			/**
			 * @brief Indicates if the storage in database is available. 
			 *
			 * @return true if the database storage is available, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isDatabaseStorageAvailable();
			
			/**
			 * @brief Indicates if IMDN are available. 
			 *
			 * @return true if IDMN are available 
			 */
			LINPHONECXX_PUBLIC bool isImdnAvailable();
			
			/**
			 * @brief Sets the log collection path. 
			 *
			 * @param path the path of the logs   
			 */
			LINPHONECXX_PUBLIC void setLogCollectionPath(const std::string & path);
			
			/**
			 * @brief Get the directory where the mediastreamer2 plugins are located. 
			 *
			 * @return The path to the directory where the mediastreamer2 plugins are located,
			 * or nullptr if it has not been set.   
			 */
			LINPHONECXX_PUBLIC std::string getMspluginsDir();
			
			/**
			 * @brief Set the directory where the mediastreamer2 plugins are located. 
			 *
			 * @param path The path to the directory where the mediastreamer2 plugins are
			 * located   
			 */
			LINPHONECXX_PUBLIC void setMspluginsDir(const std::string & path);
			
			/**
			 * @brief Get the directory where the ring resources are located. 
			 *
			 * @return The path to the directory where the ring resources are located   
			 */
			LINPHONECXX_PUBLIC std::string getRingResourcesDir();
			
			/**
			 * @brief Set the directory where the ring resources are located. 
			 *
			 * @param path The path where the ring resources are located   
			 */
			LINPHONECXX_PUBLIC void setRingResourcesDir(const std::string & path);
			
			/**
			 * @brief Get the directory where the sound resources are located. 
			 *
			 * @return The path to the directory where the sound resources are located   
			 */
			LINPHONECXX_PUBLIC std::string getSoundResourcesDir();
			
			/**
			 * @brief Set the directory where the sound resources are located. 
			 *
			 * @param path The path where the sound resources are located   
			 */
			LINPHONECXX_PUBLIC void setSoundResourcesDir(const std::string & path);
			
			/**
			 * @brief Get the list of standard video definitions supported by Linphone. 
			 *
			 * @return A list of video definitions.    
			 */
			LINPHONECXX_PUBLIC std::list<std::shared_ptr<linphone::VideoDefinition>> getSupportedVideoDefinitions() const;
			
			/**
			 * @brief Get the top directory where the resources are located. 
			 *
			 * @return The path to the top directory where the resources are located   
			 */
			LINPHONECXX_PUBLIC std::string getTopResourcesDir() const;
			
			/**
			 * @brief Set the top directory where the resources are located. 
			 *
			 * If you only define this top directory, the other resources directory will
			 * automatically be derived form this one. 
			 * @param path The path to the top directory where the resources are located   
			 */
			LINPHONECXX_PUBLIC void setTopResourcesDir(const std::string & path);
			
			/**
			 * @brief Parse a string holding a SIP URI and create the according @ref Address
			 * object. 
			 *
			 * @param addr A string holding the SIP URI to parse.   
			 * @return A new @ref Address.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Address> createAddress(const std::string & addr) const;
			
			/**
			 * @brief Creates a @ref AuthInfo object. 
			 *
			 * The object can be created empty, that is with all arguments set to nullptr.
			 * Username, userid, password, realm and domain can be set later using specific
			 * methods. At the end, username and passwd (or ha1) are required. 
			 * @param domain The SIP domain for which this authentication information is
			 * valid, if it has to be restricted for a single SIP domain.   
			 * @return A @ref AuthInfo object. linphone_auth_info_destroy must be used to
			 * destroy it when no longer needed. The @ref Core makes a copy of @ref AuthInfo
			 * passed through Core::addAuthInfo().   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::AuthInfo> createAuthInfo(const std::string & username, const std::string & userid, const std::string & passwd, const std::string & ha1, const std::string & realm, const std::string & domain) const;
			
			/**
			 * @brief Creates a @ref AuthInfo object. 
			 *
			 * The object can be created empty, that is with all arguments set to nullptr.
			 * Username, userid, password, realm and domain can be set later using specific
			 * methods. At the end, username and passwd (or ha1) are required. 
			 * @param algorithm The algorithm for encrypting password.   
			 * @return A @ref AuthInfo object. linphone_auth_info_destroy must be used to
			 * destroy it when no longer needed. The @ref Core makes a copy of @ref AuthInfo
			 * passed through Core::addAuthInfo().   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::AuthInfo> createAuthInfo(const std::string & username, const std::string & userid, const std::string & passwd, const std::string & ha1, const std::string & realm, const std::string & domain, const std::string & algorithm) const;
			
			/**
			 * @brief Creates an object @ref Buffer. 
			 *
			 * @return a @ref Buffer   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Buffer> createBuffer();
			
			/**
			 * @brief Creates an object @ref Buffer. 
			 *
			 * @param size the size of the data 
			 * @return a @ref Buffer   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Buffer> createBufferFromData(const uint8_t * data, size_t size);
			
			/**
			 * @brief Creates an object @ref Buffer. 
			 *
			 * @param data the data to set in the buffer   
			 * @return a @ref Buffer   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Buffer> createBufferFromString(const std::string & data);
			
			/**
			 * @brief Creates an object @ref Config. 
			 *
			 * @param path the path of the config   
			 * @return a @ref Config   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Config> createConfig(const std::string & path);
			
			/**
			 * @brief Creates an object @ref Config. 
			 *
			 * @param data the config data   
			 * @return a @ref Config   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Config> createConfigFromString(const std::string & data);
			
			/**
			 * @brief Creates an object @ref Config. 
			 *
			 * @param factoryPath the path of the factory   
			 * @return a @ref Config   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Config> createConfigWithFactory(const std::string & path, const std::string & factoryPath);
			
			/**
			 * @brief Creates an object @ref Content. 
			 *
			 * @return a @ref Content   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Content> createContent();
			
			/**
			 * @brief Instantiate a @ref Core object. 
			 *
			 * The @ref Core object is the primary handle for doing all phone actions. It
			 * should be unique within your application. The @ref Core object is not started
			 * automatically, you need to call Core::start() to that effect. The returned @ref
			 * Core will be in @ref GlobalState Ready. Core ressources can be released using
			 * Core::stop() which is strongly encouraged on garbage collected languages. 
			 * @param systemContext A pointer to a system object required by the core to
			 * operate. Currently it is required to pass an android Context on android, pass
			 * nullptr on other platforms.   
			 * @return a @ref Core object   
			 * @see linphone_core_new_with_config_3() 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> createCore(const std::string & configPath, const std::string & factoryConfigPath, void * systemContext) const;
			
			/**
			 * @brief Instantiate a @ref Core object with a given LinphoneConfig. 
			 *
			 * The @ref Core object is the primary handle for doing all phone actions. It
			 * should be unique within your application. The @ref Core object is not started
			 * automatically, you need to call Core::start() to that effect. The returned @ref
			 * Core will be in @ref GlobalState Ready. Core ressources can be released using
			 * Core::stop() which is strongly encouraged on garbage collected languages. 
			 * @param systemContext A pointer to a system object required by the core to
			 * operate. Currently it is required to pass an android Context on android, pass
			 * nullptr on other platforms.   
			 * @return a @ref Core object   
			 * @see createCore() 
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Core> createCoreWithConfig(const std::shared_ptr<linphone::Config> & config, void * systemContext) const;
			
			/**
			 * @brief Creates an object LinphoneErrorInfo. 
			 *
			 * @return a @ref ErrorInfo object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ErrorInfo> createErrorInfo();
			
			/**
			 * @brief Create a @ref ParticipantDeviceIdentity object. 
			 *
			 * @param name the name given to the device.   
			 * @return A new @ref ParticipantDeviceIdentity.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ParticipantDeviceIdentity> createParticipantDeviceIdentity(const std::shared_ptr<const linphone::Address> & address, const std::string & name) const;
			
			/**
			 * @brief Creates an object LinphoneRange. 
			 *
			 * @return a @ref Range object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Range> createRange();
			
			/**
			 * @brief Creates an object LinphoneTransports. 
			 *
			 * @return a @ref Transports object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Transports> createTransports();
			
			/**
			 * @brief Creates an object @ref TunnelConfig. 
			 *
			 * @return a @ref TunnelConfig   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::TunnelConfig> createTunnelConfig();
			
			/**
			 * @brief Create an empty @ref Vcard. 
			 *
			 * @return a new @ref Vcard.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Vcard> createVcard();
			
			/**
			 * @brief Creates an object LinphoneVideoActivationPolicy. 
			 *
			 * @return @ref VideoActivationPolicy object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::VideoActivationPolicy> createVideoActivationPolicy();
			
			/**
			 * @brief Create a @ref VideoDefinition from a given width and height. 
			 *
			 * @param height The height of the created video definition 
			 * @return A new @ref VideoDefinition object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::VideoDefinition> createVideoDefinition(unsigned int width, unsigned int height) const;
			
			/**
			 * @brief Create a @ref VideoDefinition from a given standard definition name. 
			 *
			 * @param name The standard definition name of the video definition to create   
			 * @return A new @ref VideoDefinition object   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::VideoDefinition> createVideoDefinitionFromName(const std::string & name) const;
			
			/**
			 * @brief Enables or disables log collection. 
			 *
			 * @param state the @ref LogCollectionState for log collection 
			 */
			LINPHONECXX_PUBLIC void enableLogCollection(linphone::LogCollectionState state);
			
			/**
			 * @brief Get the config path. 
			 *
			 * @param context used to compute path. Can be nullptr. JavaPlatformHelper on
			 * Android and char *appGroupId on iOS with shared core.   
			 * @return The config path   
			 */
			LINPHONECXX_PUBLIC std::string getConfigDir(void * context);
			
			/**
			 * @brief Get the data path. 
			 *
			 * @param context used to compute path. Can be nullptr. JavaPlatformHelper on
			 * Android and char *appGroupId on iOS with shared core.   
			 * @return The data path   
			 */
			LINPHONECXX_PUBLIC std::string getDataDir(void * context);
			
			/**
			 * @brief Get the download path. 
			 *
			 * @param context used to compute path. Can be nullptr. JavaPlatformHelper on
			 * Android and char *appGroupId on iOS with shared core.   
			 * @return The download path   
			 */
			LINPHONECXX_PUBLIC std::string getDownloadDir(void * context);
			
			/**
			 * @brief Indicates if the given LinphoneChatRoomBackend is available. 
			 *
			 * @param chatroomBackend the @ref ChatRoomBackend 
			 * @return true if the chatroom backend is available, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isChatroomBackendAvailable(linphone::ChatRoomBackend chatroomBackend);
			
			/**
			 * @brief Select encryption module and set secret material to encrypt the files. 
			 *
			 * @param secretSize size of the secret 
			 */
			LINPHONECXX_PUBLIC void setVfsEncryption(const uint16_t encryptionModule, const uint8_t * secret, const size_t secretSize);
			
			;
			/**
			 * @brief Clean the factory. 
			 *
			 * This function is generally useless as the factory is unique per process,
			 * however calling this function at the end avoid getting reports from belle-sip
			 * leak detector about memory leaked in get(). 
			 */
			LINPHONECXX_PUBLIC static void clean();
			
;
			/**
			 * @brief Create the @ref Factory if that has not been done and return a pointer
			 * on it. 
			 *
			 * @return A pointer on the @ref Factory   
			 */
			LINPHONECXX_PUBLIC static std::shared_ptr<linphone::Factory> get();
			

	};

};

#endif // _LINPHONE_FACTORY_HH
