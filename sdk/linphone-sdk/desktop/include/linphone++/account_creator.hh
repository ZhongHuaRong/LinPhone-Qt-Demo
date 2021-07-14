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

#ifndef _LINPHONE_ACCOUNT_CREATOR_HH
#define _LINPHONE_ACCOUNT_CREATOR_HH

#include <string>
#include "account_creator.hh"
#include "enums.hh"
#include "object.hh"




struct _LinphoneAccountCreator;


namespace linphone {

	class ProxyConfig;
	class AccountCreatorListener;

	/**
	 * @brief The object used to configure an account on a server via XML-RPC, see
	 * https://wiki.linphone.org/xwiki/wiki/public/view/Lib/Features/Override%20account%20creator%20request/.
	 *
	 */
	class AccountCreator: public MultiListenableObject {
	
		public:
			/**
			* Enum describing the status of server request, used by the @ref AccountCreator. 
			*
			*/
			enum class Status {
				/**
				* Request status. 
				*/
				RequestOk,
				/**
				* Request failed. 
				*/
				RequestFailed,
				/**
				* Request failed due to missing argument(s) 
				*/
				MissingArguments,
				/**
				* Request failed due to missing callback(s) 
				*/
				MissingCallbacks,
				/**
				* Account status. 
				*/
				AccountCreated,
				/**
				* Account not created. 
				*/
				AccountNotCreated,
				/**
				* Account exist. 
				*/
				AccountExist,
				/**
				* Account exist with alias. 
				*/
				AccountExistWithAlias,
				/**
				* Account not exist. 
				*/
				AccountNotExist,
				/**
				* Account was created with Alias. 
				*/
				AliasIsAccount,
				/**
				* Alias exist. 
				*/
				AliasExist,
				/**
				* Alias not exist. 
				*/
				AliasNotExist,
				/**
				* Account activated. 
				*/
				AccountActivated,
				/**
				* Account already activated. 
				*/
				AccountAlreadyActivated,
				/**
				* Account not activated. 
				*/
				AccountNotActivated,
				/**
				* Account linked. 
				*/
				AccountLinked,
				/**
				* Account not linked. 
				*/
				AccountNotLinked,
				/**
				* Server. 
				*/
				ServerError,
				/**
				* Error cannot send SMS. 
				*/
				PhoneNumberInvalid,
				/**
				* Error key doesn't match. 
				*/
				WrongActivationCode,
				/**
				* Error too many SMS sent. 
				*/
				PhoneNumberOverused,
				/**
				* Error algo isn't MD5 or SHA-256. 
				*/
				AlgoNotSupported,
				/**
				* Generic error. 
				*/
				UnexpectedError
			};

			/**
			* Enum describing transport checking, used by the @ref AccountCreator. 
			*
			*/
			enum class TransportStatus {
				/**
				* Transport ok. 
				*/
				Ok,
				/**
				* Transport invalid. 
				*/
				Unsupported
			};

			/**
			* Enum describing domain checking, used by the @ref AccountCreator. 
			*
			*/
			enum class DomainStatus {
				/**
				* Domain ok. 
				*/
				Ok,
				/**
				* Domain invalid. 
				*/
				Invalid
			};

			/**
			* Enum describing activation code checking, used by the @ref AccountCreator. 
			*
			*/
			enum class ActivationCodeStatus {
				/**
				* Activation code ok. 
				*/
				Ok,
				/**
				* Activation code too short. 
				*/
				TooShort,
				/**
				* Activation code too long. 
				*/
				TooLong,
				/**
				* Contain invalid characters. 
				*/
				InvalidCharacters
			};

			/**
			* Enum describing language checking, used by the @ref AccountCreator. 
			*
			*/
			enum class LanguageStatus {
				/**
				* Language ok. 
				*/
				Ok
			};

			/**
			* Enum describing password checking, used by the @ref AccountCreator. 
			*
			*/
			enum class PasswordStatus {
				/**
				* Password ok. 
				*/
				Ok,
				/**
				* Password too short. 
				*/
				TooShort,
				/**
				* Password too long. 
				*/
				TooLong,
				/**
				* Contain invalid characters. 
				*/
				InvalidCharacters,
				/**
				* Missing specific characters. 
				*/
				MissingCharacters
			};

			/**
			* Enum describing email checking, used by the @ref AccountCreator. 
			*
			*/
			enum class EmailStatus {
				/**
				* Email ok. 
				*/
				Ok,
				/**
				* Email malformed. 
				*/
				Malformed,
				/**
				* Contain invalid characters. 
				*/
				InvalidCharacters
			};

			/**
			* Enum describing username checking, used by the @ref AccountCreator. 
			*
			*/
			enum class UsernameStatus {
				/**
				* Username ok. 
				*/
				Ok,
				/**
				* Username too short. 
				*/
				TooShort,
				/**
				* Username too long. 
				*/
				TooLong,
				/**
				* Contain invalid characters. 
				*/
				InvalidCharacters,
				/**
				* Invalid username. 
				*/
				Invalid
			};

			/**
			* Enum describing phone number checking, used by the @ref AccountCreator. 
			*
			*/
			enum class PhoneNumberStatus {
				/**
				* Phone number ok. 
				*/
				Ok = 1,
				/**
				* Phone number too short. 
				*/
				TooShort = 2,
				/**
				* Phone number too long. 
				*/
				TooLong = 4,
				/**
				* Country code invalid. 
				*/
				InvalidCountryCode = 8,
				/**
				* Phone number invalid. 
				*/
				Invalid = 16
			};


			AccountCreator(void *ptr, bool takeRef=true);
			LINPHONECXX_PUBLIC _LinphoneAccountCreator *cPtr() {return (_LinphoneAccountCreator *)mPrivPtr;}

			LINPHONECXX_PUBLIC void addListener(const std::shared_ptr<AccountCreatorListener> &listener);
			LINPHONECXX_PUBLIC void removeListener(const std::shared_ptr<AccountCreatorListener> &listener);
	
		public:
			
		
			/**
			 * @brief Get the activation code. 
			 *
			 * @return The activation code of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getActivationCode() const;
			
			/**
			 * @brief Set the activation code. 
			 *
			 * @param activationCode The activation code to set   
			 * @return @ref ActivationCodeStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC ActivationCodeStatus setActivationCode(const std::string & activationCode);
			
			/**
			 * @brief Get the algorithm configured in the account creator. 
			 *
			 * @return The algorithm of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getAlgorithm() const;
			
			/**
			 * @brief Set the supported algorithm. 
			 *
			 * @param algorithm The algorithm to use   
			 * @return LinphoneAccountCreatorAlgoStatusOk if everything is OK, or a specific
			 * error otherwise. 
			 */
			LINPHONECXX_PUBLIC linphone::AccountCreatorAlgoStatus setAlgorithm(const std::string & algorithm);
			
			/**
			 * @brief Set the set_as_default property. 
			 *
			 * @param setAsDefault true for the created proxy config to be set as default in
			 * @ref Core, false otherwise 
			 * @return @ref Status::RequestOk if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC Status setAsDefault(bool setAsDefault);
			
			/**
			 * @brief Get the display name. 
			 *
			 * @return The display name of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getDisplayName() const;
			
			/**
			 * @brief Set the display name. 
			 *
			 * @param displayName The display name to set   
			 * @return @ref UsernameStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC UsernameStatus setDisplayName(const std::string & displayName);
			
			/**
			 * @brief Get the domain. 
			 *
			 * @return The domain of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getDomain() const;
			
			/**
			 * @brief Set the domain. 
			 *
			 * @param domain The domain to set   
			 * @return @ref DomainStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC DomainStatus setDomain(const std::string & domain);
			
			/**
			 * @brief Get the email. 
			 *
			 * @return The email of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getEmail() const;
			
			/**
			 * @brief Set the email. 
			 *
			 * @param email The email to set   
			 * @return @ref EmailStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC EmailStatus setEmail(const std::string & email);
			
			/**
			 * @brief Get the ha1. 
			 *
			 * @return The ha1 of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getHa1() const;
			
			/**
			 * @brief Set the ha1. 
			 *
			 * @param ha1 The ha1 to set   
			 * @return @ref PasswordStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC PasswordStatus setHa1(const std::string & ha1);
			
			/**
			 * @brief Get the language use in email of SMS. 
			 *
			 * @return The language of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getLanguage() const;
			
			/**
			 * @brief Set the language to use in email or SMS if supported. 
			 *
			 * @param lang The language to use   
			 * @return @ref LanguageStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC LanguageStatus setLanguage(const std::string & lang);
			
			/**
			 * @brief Get the password. 
			 *
			 * @return The password of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getPassword() const;
			
			/**
			 * @brief Set the password. 
			 *
			 * @param password The password to set   
			 * @return @ref PasswordStatus::Ok if everything is OK, or specific(s) error(s)
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC PasswordStatus setPassword(const std::string & password);
			
			/**
			 * @brief Get the RFC 3966 normalized phone number. 
			 *
			 * @return The phone number of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getPhoneNumber() const;
			
			/**
			 * @brief Assign a proxy config pointer to the LinphoneAccountCreator. 
			 *
			 * @param cfg The LinphoneProxyConfig to associate with the
			 * LinphoneAccountCreator.   
			 */
			LINPHONECXX_PUBLIC void setProxyConfig(const std::shared_ptr<linphone::ProxyConfig> & cfg);
			
			/**
			 * @brief Get the route. 
			 *
			 * @return The route of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getRoute() const;
			
			/**
			 * @brief Set the route. 
			 *
			 * @param route The route to set   
			 * @return @ref Status::RequestOk if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC Status setRoute(const std::string & route);
			
			/**
			 * @brief Get the set_as_default property. 
			 *
			 * @return true if account will be set as default, false otherwise. 
			 */
			LINPHONECXX_PUBLIC bool getSetAsDefault() const;
			
			/**
			 * @brief Get Transport. 
			 *
			 * @return The @ref TransportType of the creator. 
			 */
			LINPHONECXX_PUBLIC linphone::TransportType getTransport() const;
			
			/**
			 * @brief Set Transport. 
			 *
			 * @param transport The @ref TransportType to set 
			 * @return @ref TransportStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC TransportStatus setTransport(linphone::TransportType transport);
			
			/**
			 * @brief Get the username. 
			 *
			 * @return The username of the @ref AccountCreator.   
			 */
			LINPHONECXX_PUBLIC std::string getUsername() const;
			
			/**
			 * @brief Set the username. 
			 *
			 * @param username The username to set   
			 * @return @ref UsernameStatus::Ok if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC UsernameStatus setUsername(const std::string & username);
			
			/**
			 * @brief Send a request to activate an account on server. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status activateAccount();
			
			/**
			 * @brief Send a request to activate an alias. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status activateAlias();
			
			/**
			 * @brief Send a request to create an account on server. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status createAccount();
			
			/**
			 * @brief Create and configure a proxy config and a authentication info for an
			 * account creator. 
			 *
			 * @return A @ref ProxyConfig object if successful, nullptr otherwise.   
			 */
			LINPHONECXX_PUBLIC std::shared_ptr<linphone::ProxyConfig> createProxyConfig() const;
			
			/**
			 * @brief Send a request to know if an account is activated on server. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status isAccountActivated();
			
			/**
			 * @brief Send a request to know the existence of account on server. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status isAccountExist();
			
			/**
			 * @brief Send a request to know if an account is linked. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status isAccountLinked();
			
			/**
			 * @brief Send a request to know if an alias is used. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status isAliasUsed();
			
			/**
			 * @brief Send a request to link an account to an alias. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status linkAccount();
			
			/**
			 * @brief Send a request to get the password & algorithm of an account using the
			 * confirmation key. 
			 *
			 * @return @ref Status::RequestOk if everything is OK, or a specific error
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC Status loginLinphoneAccount();
			
			/**
			 * @brief Send a request to recover an account. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status recoverAccount();
			
			/**
			 * @brief Reset the account creator entries like username, password, phone
			 * number... 
			 *
			 */
			LINPHONECXX_PUBLIC void reset();
			
			/**
			 * @brief Set the phone number normalized. 
			 *
			 * @param countryCode Country code to associate phone number with   
			 * @return @ref PhoneNumberStatus::Ok if everything is OK, or specific(s) error(s)
			 * otherwise. 
			 */
			LINPHONECXX_PUBLIC unsigned int setPhoneNumber(const std::string & phoneNumber, const std::string & countryCode);
			
			/**
			 * @brief Send a request to update an account. 
			 *
			 * @return @ref Status::RequestOk if the request has been sent, @ref
			 * Status::RequestFailed otherwise 
			 */
			LINPHONECXX_PUBLIC Status updateAccount();
			
			
		private:
			void *createCallbacks() override;
	};

};

#endif // _LINPHONE_ACCOUNT_CREATOR_HH
