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

public:
	Properties(): orgId(""), domain("internetofthings.ibmcloud.com"), deviceType(""), deviceId(""), authMethod(""), authToken(""){}
	std::string getorgId(){ return orgId;}
	std::string getdomain(){ return domain;}
	std::string getdeviceType(){ return deviceType;}
	std::string getdeviceId(){ return deviceId;}
	std::string getauthMethod(){ return authMethod;}
	std::string getauthToken(){ return authToken;}

	void setorgId(const std::string& org){ orgId = org;}
	void setdomain(const std::string& domainName){ domain = domainName;}
	void setdeviceType(const std::string& devicetype){ deviceType = devicetype;}
	void setdeviceId(const std::string& deviceid){ deviceId = deviceid;}
	void setauthMethod(const std::string& authmethod){ authMethod = authmethod;}
	void setauthToken(const std::string& authtoken){ authToken = authtoken;}

};


#endif /* SRC_PROPERTIES_H_ */
