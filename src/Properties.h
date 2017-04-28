/*******************************************************************************
 * Copyright (c) 2016 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Hari Prasada Reddy - initial API and implementation and/or initial documentation
 *    Lokesh K Haralakatta - Added required members amd methods for Secure Connetion
 *******************************************************************************/

#ifndef SRC_PROPERTIES_H_
#define SRC_PROPERTIES_H_


class Properties {
private:
	std::string orgId;
	std::string domain;
	std::string deviceType;
	std::string deviceId;
	std::string authMethod;
	std::string authToken;
	std::string trustStore;
	std::string keyStore;
	std::string privateKey;
	std::string keyPassPhrase;
	bool useCerts;

public:
	Properties(): orgId(""), domain("internetofthings.ibmcloud.com"), deviceType(""), deviceId(""),
	authMethod(""), authToken(""), useCerts(false), trustStore(""),keyStore(""),privateKey(""),
	keyPassPhrase("") {}

	std::string getorgId(){ return orgId;}
	std::string getdomain(){ return domain;}
	std::string getdeviceType(){ return deviceType;}
	std::string getdeviceId(){ return deviceId;}
	std::string getauthMethod(){ return authMethod;}
	std::string getauthToken(){ return authToken;}
	bool getuseCerts(){ return useCerts;}
	std::string gettrustStore(){ return trustStore;}
	std::string getkeyStore(){ return keyStore;}
	std::string getprivateKey(){ return privateKey;}
	std::string getkeyPassPhrase(){ return keyPassPhrase;}

	void setorgId(const std::string& org){ orgId = org;}
	void setdomain(const std::string& domainName){ domain = domainName;}
	void setdeviceType(const std::string& devicetype){ deviceType = devicetype;}
	void setdeviceId(const std::string& deviceid){ deviceId = deviceid;}
	void setauthMethod(const std::string& authmethod){ authMethod = authmethod;}
	void setauthToken(const std::string& authtoken){ authToken = authtoken;}
	void setuseCerts(const bool& certs){ useCerts = certs;}
	void settrustStore(const std::string& truststore){ trustStore = truststore;}
	void setkeyStore(const std::string& keystore){ keyStore = keystore;}
	void setprivateKey(const std::string& privatekey){ privateKey = privatekey;}
	void setkeyPassPhrase(const std::string& passphrase){ keyPassPhrase = passphrase;}

};


#endif /* SRC_PROPERTIES_H_ */
