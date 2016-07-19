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

#ifndef SRC_COMMANDCALLBACK_H_
#define SRC_COMMANDCALLBACK_H_

#include "Command.h"
class CommandCallback{
public:
	virtual void processCommand(Command& cmd) = 0;
};



#endif /* SRC_COMMANDCALLBACK_H_ */
