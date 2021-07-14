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

#ifndef _LINPHONE_CONFIG_HH
#define _LINPHONE_CONFIG_HH

#include <list>
#include <string>
#include <cstdint>
#include "object.hh"




struct _LinphoneConfig;


namespace linphone {

	class Config;

	/**
	 * @brief This object is used to manipulate a configuration file. 
	 *
	 * The format of the configuration file is a .ini like format:
	 * Various types can be used: strings and lists of strings, integers, floats,
	 * booleans (written as 0 or 1) and range of integers.
	 * Usually a @ref Core is initialized using two @ref Config, one default (where
	 * configuration changes through API calls will be saved) and one named 'factory'
	 * which is read-only and overwrites any setting that may exists in the default
	 * one.
	 * It is also possible to use only one (either default or factory) or even none. 
	 */
	class Config: public Object {
	
		public:

			Config(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneConfig *cPtr() {return (_LinphoneConfig *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Returns the list of sections' names in the LinphoneConfig. 
			 *
			 * @return A list of strings.    
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getSectionsNamesList();
			
			/**
			 * @brief Removes entries for key,value in a section. 
			 *
			 * @param key the key to clean   
			 */
			LINPHONECXX_PUBLIC void cleanEntry(const std::string & section, const std::string & key);
			
			/**
			 * @brief Removes every pair of key,value in a section and remove the section. 
			 *
			 * @param section the section to clean   
			 */
			LINPHONECXX_PUBLIC void cleanSection(const std::string & section);
			
			/**
			 * @brief Dumps the @ref Config as INI into a buffer. 
			 *
			 * @return The buffer that contains the config dump     
			 */
			LINPHONECXX_PUBLIC std::string dump() const;
			
			/**
			 * @brief Dumps the @ref Config as XML into a buffer. 
			 *
			 * @return The buffer that contains the XML dump     
			 */
			LINPHONECXX_PUBLIC std::string dumpAsXml() const;
			
			/**
			 * @brief Retrieves a configuration item as a boolean, given its section, key, and
			 * default value. 
			 *
			 * The default boolean value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC bool getBool(const std::string & section, const std::string & key, bool defaultValue) const;
			
			/**
			 * @brief Retrieves a default configuration item as a float, given its section,
			 * key, and default value. 
			 *
			 * The default float value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found default value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC float getDefaultFloat(const std::string & section, const std::string & key, float defaultValue) const;
			
			/**
			 * @brief Retrieves a default configuration item as an integer, given its section,
			 * key, and default value. 
			 *
			 * The default integer value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found default value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC int getDefaultInt(const std::string & section, const std::string & key, int defaultValue) const;
			
			/**
			 * @brief Retrieves a default configuration item as a 64 bit integer, given its
			 * section, key, and default value. 
			 *
			 * The default integer value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found default value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC int64_t getDefaultInt64(const std::string & section, const std::string & key, int64_t defaultValue) const;
			
			/**
			 * @brief Retrieves a default configuration item as a string, given its section,
			 * key, and default value. 
			 *
			 * The default value string is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found default value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC std::string getDefaultString(const std::string & section, const std::string & key, const std::string & defaultValue) const;
			
			/**
			 * @brief Retrieves a configuration item as a float, given its section, key, and
			 * default value. 
			 *
			 * The default float value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC float getFloat(const std::string & section, const std::string & key, float defaultValue) const;
			
			/**
			 * @brief Retrieves a configuration item as an integer, given its section, key,
			 * and default value. 
			 *
			 * The default integer value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC int getInt(const std::string & section, const std::string & key, int defaultValue) const;
			
			/**
			 * @brief Retrieves a configuration item as a 64 bit integer, given its section,
			 * key, and default value. 
			 *
			 * The default integer value is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found 
			 * @return the found value or default_value if not found. 
			 */
			LINPHONECXX_PUBLIC int64_t getInt64(const std::string & section, const std::string & key, int64_t defaultValue) const;
			
			/**
			 * @brief Returns the list of keys' names for a section in the LinphoneConfig. 
			 *
			 * @param section The section name   
			 * @return A list of strings.    
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getKeysNamesList(const std::string & section);
			
			/**
			 * @brief Retrieves the overwrite flag for a config item. 
			 *
			 * @param key The name of the configuration item to retrieve the overwrite flag
			 * from.   
			 * @return true if overwrite flag is set, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getOverwriteFlagForEntry(const std::string & section, const std::string & key) const;
			
			/**
			 * @brief Retrieves the overwrite flag for a config section. 
			 *
			 * @param section The section from which to retrieve the overwrite flag   
			 * @return true if overwrite flag is set, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getOverwriteFlagForSection(const std::string & section) const;
			
			/**
			 * @brief Retrieves a configuration item as a range, given its section, key, and
			 * default min and max values. 
			 *
			 * @param defaultMax the default max value to return if not found. 
			 * @return true if the value is successfully parsed as a range, false otherwise.
			 * If false is returned, min and max are filled respectively with default_min and
			 * default_max values. 
			 */
			LINPHONECXX_PUBLIC bool getRange(const std::string & section, const std::string & key, int * min, int * max, int defaultMin, int defaultMax) const;
			
			/**
			 * @brief Retrieves a section parameter item as a string, given its section and
			 * key. 
			 *
			 * The default value string is returned if the config item isn't found. 
			 * @param defaultValue The default value to return if not found.   
			 * @return the found default value or default_value if not found.   
			 */
			LINPHONECXX_PUBLIC std::string getSectionParamString(const std::string & section, const std::string & key, const std::string & defaultValue) const;
			
			/**
			 * @brief Retrieves the skip flag for a config item. 
			 *
			 * @param key The name of the configuration item to retrieve the skip flag from 
			 * @return true if skip flag is set, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getSkipFlagForEntry(const std::string & section, const std::string & key) const;
			
			/**
			 * @brief Retrieves the skip flag for a config section. 
			 *
			 * @param section The section from which to retrieve the skip flag   
			 * @return true if skip flag is set, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getSkipFlagForSection(const std::string & section) const;
			
			/**
			 * @brief Retrieves a configuration item as a string, given its section, key, and
			 * default value. 
			 *
			 * The default value string is returned if the config item isn't found. 
			 * @param defaultString The default value to return if not found.   
			 * @return the found value or the default one if not found.   
			 */
			LINPHONECXX_PUBLIC std::string getString(const std::string & section, const std::string & key, const std::string & defaultString) const;
			
			/**
			 * @brief Retrieves a configuration item as a list of strings, given its section,
			 * key, and default value. 
			 *
			 * The default value is returned if the config item is not found. 
			 * @param defaultList The list to return when the key doesn't exist.    
			 * @return A list of strings.    
			 */
			LINPHONECXX_PUBLIC std::list<std::string> getStringList(const std::string & section, const std::string & key, const std::list<std::string> & defaultList) const;
			
			/**
			 * @brief Returns if a given section with a given key is present in the
			 * configuration. 
			 *
			 * @param key to check if it exists   
			 * @return 1 if it exists, 0 otherwise 
			 */
			LINPHONECXX_PUBLIC int hasEntry(const std::string & section, const std::string & key) const;
			
			/**
			 * @brief Returns if a given section is present in the configuration. 
			 *
			 * @param section the section to check if exists   
			 * @return 1 if it exists, 0 otherwise 
			 */
			LINPHONECXX_PUBLIC int hasSection(const std::string & section) const;
			
			/**
			 * @brief Reads a xml config file and fill the @ref Config with the read config
			 * dynamic values. 
			 *
			 * @param filename The filename of the config file to read to fill the @ref Config
			 *   
			 */
			LINPHONECXX_PUBLIC std::string loadFromXmlFile(const std::string & filename);
			
			/**
			 * @brief Reads a xml config string and fill the @ref Config with the read config
			 * dynamic values. 
			 *
			 * @param buffer The string of the config file to fill the @ref Config   
			 * @return 0 in case of success 
			 */
			LINPHONECXX_PUBLIC linphone::Status loadFromXmlString(const std::string & buffer);
			
			/**
			 * @brief Reads a user config file and fill the @ref Config with the read config
			 * values. 
			 *
			 * @param filename The filename of the config file to read to fill the @ref Config
			 *   
			 */
			LINPHONECXX_PUBLIC linphone::Status readFile(const std::string & filename);
			
			/**
			 * @brief Check if given file name exists relatively to the current location. 
			 *
			 * @param filename The file name to check if exists   
			 * @return true if file exists relative to the to the current location 
			 */
			LINPHONECXX_PUBLIC bool relativeFileExists(const std::string & filename) const;
			
			/**
			 * @brief Reload the config from the file. 
			 *
			 */
			LINPHONECXX_PUBLIC void reload();
			
			/**
			 * @brief Sets a boolean config item. 
			 *
			 * @param value the value to set 
			 */
			LINPHONECXX_PUBLIC void setBool(const std::string & section, const std::string & key, bool value);
			
			/**
			 * @brief Sets a float config item. 
			 *
			 * @param value the value to set 
			 */
			LINPHONECXX_PUBLIC void setFloat(const std::string & section, const std::string & key, float value);
			
			/**
			 * @brief Sets an integer config item. 
			 *
			 * @param value the value to set 
			 */
			LINPHONECXX_PUBLIC void setInt(const std::string & section, const std::string & key, int value);
			
			/**
			 * @brief Sets a 64 bits integer config item. 
			 *
			 * @param value the value to set 
			 */
			LINPHONECXX_PUBLIC void setInt64(const std::string & section, const std::string & key, int64_t value);
			
			/**
			 * @brief Sets an integer config item, but store it as hexadecimal. 
			 *
			 * @param value the value to set 
			 */
			LINPHONECXX_PUBLIC void setIntHex(const std::string & section, const std::string & key, int value);
			
			/**
			 * @brief Sets the overwrite flag for a config item (used when dumping config as
			 * xml) 
			 *
			 * @param value The overwrite flag value to set 
			 */
			LINPHONECXX_PUBLIC void setOverwriteFlagForEntry(const std::string & section, const std::string & key, bool value);
			
			/**
			 * @brief Sets the overwrite flag for a config section (used when dumping config
			 * as xml) 
			 *
			 * @param value The overwrite flag value to set 
			 */
			LINPHONECXX_PUBLIC void setOverwriteFlagForSection(const std::string & section, bool value);
			
			/**
			 * @brief Sets a range config item. 
			 *
			 * @param maxValue the max value to set 
			 */
			LINPHONECXX_PUBLIC void setRange(const std::string & section, const std::string & key, int minValue, int maxValue);
			
			/**
			 * @brief Sets the skip flag for a config item (used when dumping config as xml) 
			 *
			 * @param value The skip flag value to set 
			 */
			LINPHONECXX_PUBLIC void setSkipFlagForEntry(const std::string & section, const std::string & key, bool value);
			
			/**
			 * @brief Sets the skip flag for a config section (used when dumping config as
			 * xml) 
			 *
			 * @param value The skip flag value to set 
			 */
			LINPHONECXX_PUBLIC void setSkipFlagForSection(const std::string & section, bool value);
			
			/**
			 * @brief Sets a string config item. 
			 *
			 * @param value The value to set   
			 */
			LINPHONECXX_PUBLIC void setString(const std::string & section, const std::string & key, const std::string & value);
			
			/**
			 * @brief Sets a string list config item. 
			 *
			 * @param value The value to set.    
			 */
			LINPHONECXX_PUBLIC void setStringList(const std::string & section, const std::string & key, const std::list<std::string> & value);
			
			/**
			 * @brief Writes the config file to disk. 
			 *
			 * @return 0 if successful, -1 otherwise 
			 */
			LINPHONECXX_PUBLIC linphone::Status sync();
			
			/**
			 * @brief Write a string in a file placed relatively with the Linphone
			 * configuration file. 
			 *
			 * @param data String to write   
			 */
			LINPHONECXX_PUBLIC void writeRelativeFile(const std::string & filename, const std::string & data) const;
			
			;
			/**
			 * @brief Instantiates a @ref Config object from a user provided buffer. 
			 *
			 * The caller of this constructor owns a reference. linphone_config_unref must be
			 * called when this object is no longer needed.
			 * @param buffer the buffer from which the @ref Config will be retrieved. We
			 * expect the buffer to be null-terminated.   
			 * @see newWithFactory() 
			 * @see linphone_config_new 
			 * @return a @ref Config object   
			 */
			LINPHONECXX_PUBLIC static std::shared_ptr<linphone::Config> newFromBuffer(const std::string & buffer);
			
;
			/**
			 * @brief Instantiates a @ref Config object from a user config file and a factory
			 * config file. 
			 *
			 * The caller of this constructor owns a reference. linphone_config_unref must be
			 * called when this object is no longer needed.
			 * @param factoryConfigFilename the filename of the factory config file to read to
			 * fill the instantiated @ref Config   
			 * @see linphone_config_new 
			 * @return a @ref Config object  
			 * The user config file is read first to fill the @ref Config and then the factory
			 * config file is read. Therefore the configuration parameters defined in the user
			 * config file will be overwritten by the parameters defined in the factory config
			 * file. 
			 */
			LINPHONECXX_PUBLIC static std::shared_ptr<linphone::Config> newWithFactory(const std::string & configFilename, const std::string & factoryConfigFilename);
			

	};

};

#endif // _LINPHONE_CONFIG_HH
