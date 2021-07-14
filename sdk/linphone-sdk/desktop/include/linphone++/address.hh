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

#ifndef _LINPHONE_ADDRESS_HH
#define _LINPHONE_ADDRESS_HH

#include <string>
#include "enums.hh"
#include "object.hh"




struct _LinphoneAddress;


namespace linphone {

	class Address;

	/**
	 * @brief Object that represents a parsed SIP address. 
	 *
	 * A SIP address is made of display name, username, domain name, port, and various
	 * uri headers (such as tags). It looks like 'Alice <sip:alice@example.net>'.
	 * You can create an address using Factory::createAddress() or
	 * Core::interpretUrl() and both will return a nullptr object if it doesn't match
	 * the grammar defined by the standard.
	 * This object is used in almost every other major objects to identity people
	 * (including yourself) & servers.
	 * The @ref Address has methods to extract and manipulate all parts of the
	 * address. 
	 */
	class Address: public Object {
	
		public:

			Address(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneAddress *cPtr() {return (_LinphoneAddress *)mPrivPtr;}

	
		public:
			
		
			/**
			 * @brief Returns the display name. 
			 *
			 * @return the display name if any, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::string getDisplayName() const;
			
			/**
			 * @brief Sets the display name. 
			 *
			 * @param displayName the display name to set.   
			 */
			LINPHONECXX_PUBLIC linphone::Status setDisplayName(const std::string & displayName);
			
			/**
			 * @brief Returns the domain name. 
			 *
			 * @return the domain name if any, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::string getDomain() const;
			
			/**
			 * @brief Sets the domain. 
			 *
			 * @param domain the domain to set.   
			 */
			LINPHONECXX_PUBLIC linphone::Status setDomain(const std::string & domain);
			
			/**
			 * @brief returns whether the address is a routable SIP address or not 
			 *
			 * @return true if it is a routable SIP address, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool isSip() const;
			
			/**
			 * @brief Get the value of the method parameter. 
			 *
			 * @return the value of the parameter or nullptr.   
			 */
			LINPHONECXX_PUBLIC std::string getMethodParam() const;
			
			/**
			 * @brief Set the value of the method parameter. 
			 *
			 * @param methodParam the value to set to the method parameter.   
			 */
			LINPHONECXX_PUBLIC void setMethodParam(const std::string & methodParam);
			
			/**
			 * @brief Get the password encoded in the address. 
			 *
			 * It is used for basic authentication (not recommended). 
			 * @return the password if any, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::string getPassword() const;
			
			/**
			 * @brief Set the password encoded in the address. 
			 *
			 * It is used for basic authentication (not recommended). 
			 * @param password the password to set.   
			 */
			LINPHONECXX_PUBLIC void setPassword(const std::string & password);
			
			/**
			 * @brief Get port number as an integer value, 0 if not present. 
			 *
			 * @return the port set in the address or 0 if not present. 
			 */
			LINPHONECXX_PUBLIC int getPort() const;
			
			/**
			 * @brief Sets the port number. 
			 *
			 * @param port the port to set in the address 
			 */
			LINPHONECXX_PUBLIC linphone::Status setPort(int port);
			
			/**
			 * @brief Returns the address scheme, normally "sip". 
			 *
			 * @return the scheme if any, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::string getScheme() const;
			
			/**
			 * @brief Returns whether the address refers to a secure location (sips) or not. 
			 *
			 * @return true if address refers to a secure location, false otherwise 
			 */
			LINPHONECXX_PUBLIC bool getSecure() const;
			
			/**
			 * @brief Make the address refer to a secure location (sips scheme) 
			 *
			 * @param enabled true if address is requested to be secure. 
			 */
			LINPHONECXX_PUBLIC void setSecure(bool enabled);
			
			/**
			 * @brief Get the transport. 
			 *
			 * @return a @ref TransportType, default value if not set is UDP. 
			 */
			LINPHONECXX_PUBLIC linphone::TransportType getTransport() const;
			
			/**
			 * @brief Set a transport. 
			 *
			 * @param transport a @ref TransportType 
			 */
			LINPHONECXX_PUBLIC linphone::Status setTransport(linphone::TransportType transport);
			
			/**
			 * @brief Set the value of the parameters of the URI of the address. 
			 *
			 * @param params The parameters string 
			 */
			LINPHONECXX_PUBLIC void setUriParams(const std::string & params);
			
			/**
			 * @brief Returns the username. 
			 *
			 * @return the username name if any, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::string getUsername() const;
			
			/**
			 * @brief Sets the username. 
			 *
			 * @param username the username to set.   
			 */
			LINPHONECXX_PUBLIC linphone::Status setUsername(const std::string & username);
			
			/**
			 * @brief Returns the address as a string. 
			 *
			 * The returned char * must be freed by the application. Use ms_free(). 
			 * @return a string representation of the address.     
			 */
			LINPHONECXX_PUBLIC std::string asString() const;
			
			/**
			 * @brief Returns the SIP uri only as a string, that is display name is removed. 
			 *
			 * The returned char * must be freed by the application. Use ms_free(). 
			 * @return a string representation of the address.     
			 */
			LINPHONECXX_PUBLIC std::string asStringUriOnly() const;
			
			/**
			 * @brief Removes address's tags and uri headers so that it is displayable to the
			 * user. 
			 *
			 */
			LINPHONECXX_PUBLIC void clean();
			
			/**
			 * @brief Clones a @ref Address object. 
			 *
			 * @return a new @ref Address object.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::Address> clone() const;
			
			/**
			 * @brief Compare two @ref Address taking the tags and headers into account. 
			 *
			 * @param address2 @ref Address object.   
			 * @return Boolean value telling if the @ref Address objects are equal. 
			 * @see weakEqual() 
			 */
			LINPHONECXX_PUBLIC bool equal(const std::shared_ptr<const linphone::Address> & address2) const;
			
			/**
			 * @brief Get the header encoded in the address. 
			 *
			 * @param headerName the header name.   
			 * @return the header value or nullptr if it doesn't exists.   
			 */
			LINPHONECXX_PUBLIC std::string getHeader(const std::string & headerName) const;
			
			/**
			 * @brief Get the value of a parameter of the address. 
			 *
			 * @param paramName The name of the parameter.   
			 * @return The value of the parameter or nullptr if it doesn't exists.   
			 */
			LINPHONECXX_PUBLIC std::string getParam(const std::string & paramName) const;
			
			/**
			 * @brief Get the value of a parameter of the URI of the address. 
			 *
			 * @param uriParamName The name of the parameter.   
			 * @return The value of the parameter or nullptr if it doesn't exists.   
			 */
			LINPHONECXX_PUBLIC std::string getUriParam(const std::string & uriParamName) const;
			
			/**
			 * @brief Tell whether a parameter is present in the address. 
			 *
			 * @param paramName The name of the parameter.   
			 * @return A boolean value telling whether the parameter is present in the address 
			 */
			LINPHONECXX_PUBLIC bool hasParam(const std::string & paramName) const;
			
			/**
			 * @brief Tell whether a parameter is present in the URI of the address. 
			 *
			 * @param uriParamName The name of the parameter.   
			 * @return A boolean value telling whether the parameter is present in the URI of
			 * the address 
			 */
			LINPHONECXX_PUBLIC bool hasUriParam(const std::string & uriParamName) const;
			
			/**
			 * @brief Removes the value of a parameter of the URI of the address. 
			 *
			 * @param uriParamName The name of the parameter.   
			 */
			LINPHONECXX_PUBLIC void removeUriParam(const std::string & uriParamName);
			
			/**
			 * @brief Set a header into the address. 
			 *
			 * Headers appear in the URI with '?', such as
			 * <sip:test@linphone.org?SomeHeader=SomeValue>. 
			 * @param headerValue the header value.   
			 */
			LINPHONECXX_PUBLIC void setHeader(const std::string & headerName, const std::string & headerValue);
			
			/**
			 * @brief Set the value of a parameter of the address. 
			 *
			 * @param paramValue The new value of the parameter.   
			 */
			LINPHONECXX_PUBLIC void setParam(const std::string & paramName, const std::string & paramValue);
			
			/**
			 * @brief Set the value of a parameter of the URI of the address. 
			 *
			 * @param uriParamValue The new value of the parameter.   
			 */
			LINPHONECXX_PUBLIC void setUriParam(const std::string & uriParamName, const std::string & uriParamValue);
			
			/**
			 * @brief Compare two @ref Address ignoring tags and headers, basically just
			 * domain, username, and port. 
			 *
			 * @param address2 @ref Address object.   
			 * @return Boolean value telling if the @ref Address objects are equal. 
			 * @see equal() 
			 */
			LINPHONECXX_PUBLIC bool weakEqual(const std::shared_ptr<const linphone::Address> & address2) const;
			
			
	};

};

#endif // _LINPHONE_ADDRESS_HH
