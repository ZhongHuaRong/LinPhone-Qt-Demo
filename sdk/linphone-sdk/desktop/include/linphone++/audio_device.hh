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

#ifndef _LINPHONE_AUDIO_DEVICE_HH
#define _LINPHONE_AUDIO_DEVICE_HH

#include <string>
#include "audio_device.hh"
#include "object.hh"




struct _LinphoneAudioDevice;


namespace linphone {


	/**
	 * @brief Object holding audio device information. 
	 *
	 * It contains the name of the device, it's type if available (Earpiece, Speaker,
	 * Bluetooth, etc..) and capabilities (input, output or both) the name of the
	 * driver that created it (filter in mediastreamer).
	 * You can use the @ref AudioDevice objects to configure default input/output
	 * devices or do it dynamically during a call.
	 * To get the list of available devices, use Core::getAudioDevices(). This list
	 * will be limited to one device of each type. Use Core::getExtendedAudioDevices()
	 * for a complete list. 
	 */
	class AudioDevice: public Object {
	
		public:
			/**
			* @ref Capabilities enum represents whether a device can record audio, play audio
			* or both 
			*
			*/
			enum class Capabilities {
				/**
				*/
				CapabilityRecord = 1<<0,
				/**
				* Can record audio. 
				*/
				CapabilityPlay = 1<<1
			};

			/**
			* @ref Type enum represents the different types of an audio device. 
			*
			*/
			enum class Type {
				/**
				*/
				Unknown,
				/**
				* Unknown. 
				*/
				Microphone,
				/**
				* Microphone. 
				*/
				Earpiece,
				/**
				* Earpiece. 
				*/
				Speaker,
				/**
				* Speaker. 
				*/
				Bluetooth,
				/**
				* Bluetooth. 
				*/
				BluetoothA2DP,
				/**
				* Bluetooth A2DP. 
				*/
				Telephony,
				/**
				* Telephony. 
				*/
				AuxLine,
				/**
				* AuxLine. 
				*/
				GenericUsb,
				/**
				* GenericUsb. 
				*/
				Headset,
				/**
				* Headset. 
				*/
				Headphones
			};


			AudioDevice(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneAudioDevice *cPtr() {return (_LinphoneAudioDevice *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Returns the capabilities of the device. 
			 *
			 * @return the @ref Capabilities of the audio device (RECORD, PLAY or both) as a
			 * bit mask 
			 */
			LINPHONECXX_PUBLIC Capabilities getCapabilities() const;
			
			/**
			 * @brief Returns the name of the audio device. 
			 *
			 * @return the name of the audio device.   
			 */
			LINPHONECXX_PUBLIC std::string getDeviceName() const;
			
			/**
			 * @brief Returns the driver name used by the device. 
			 *
			 * @return the name of the driver used by this audio device.   
			 */
			LINPHONECXX_PUBLIC std::string getDriverName() const;
			
			/**
			 * @brief Returns the id of the audio device. 
			 *
			 * @return the id of the audio device.   
			 */
			LINPHONECXX_PUBLIC std::string getId() const;
			
			/**
			 * @brief Returns the type of the device. 
			 *
			 * @return the @ref Type of the audio device (microphone, speaker, earpiece,
			 * bluetooth, etc...) 
			 */
			LINPHONECXX_PUBLIC Type getType() const;
			
			/**
			 * @brief Returns whether or not the audio device has the given capability. 
			 *
			 * @param capability the @ref Capabilities to check 
			 * @return true if the audio device has the capability, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool hasCapability(Capabilities capability) const;
			
			
	};

};

#endif // _LINPHONE_AUDIO_DEVICE_HH
