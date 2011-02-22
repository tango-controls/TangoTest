/*PROTECTED REGION ID(Starter::main.cpp) ENABLED START*/
static const char *RcsId = "$Id$";
//=============================================================================
//
// file :        main.cpp
//
// description :  C++ source for a TANGO device server main.
//               The main rule is to initialise (and create) the Tango
//               system and to create the DServerClass singleton.
//               The main should be the same for every Tango device server.
//
// project :     Starter for Tango Administration.
//
// $Author$
//
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
// $Revision$
// $Date$
//
// SVN only:
// $HeadURL:  $
//
// CVS only:
// $Source$
// $Log$
// Revision 3.10  2010/10/18 12:58:52  pascal_verdier
// Pogo-7 compatibility
//
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <tango.h>
#ifdef _TG_WINDOWS_
//#define USE_SERVICE
#endif

#ifdef sun
#	include <signal.h>
#endif


#include <string.h>
#include <Starter.h>


#ifdef USE_SERVICE
#include <StarterService.h>
using namespace std;
#endif

#ifdef _TG_WINDOWS__DBG
#include	"Stackwalker.h"
#endif

int main(int argc,char *argv[])
{
	//	First time set SIGINT to default to do not block
	//	signal handling for child proceeses
	//---------------------------------------------------
#ifndef _TG_WINDOWS_
	sigset(SIGINT, SIG_DFL);
#endif
	//	Check for hostname
	//---------------------------------
	if (argc>1)
	{
		char *p;
		for (p=argv[1] ; *p ; p++)
			if (*p=='.')
				*p = '\0';	//	Take off extention (eg:.esrf.fr)
	}

	//	Set an automatic retry on database connection
	//---------------------------------------------------
	Tango::Util::_daemon = true;
	Tango::Util::_sleep_between_connect = 5;

#ifdef USE_SERVICE

	StarterService	service(argv[0]);
	int	ret;

	if ((ret=service.options(argc, argv))<=0)
		return ret;
	service.run(argc, argv);
	return 0;

#else
	Tango::Util *tg;
	try
	{
		// Initialise the device server
		//----------------------------------------
		tg = Tango::Util::init(argc,argv);

		// Create the device server singleton 
		//	which will create everything
		//----------------------------------------
		tg->server_init();
		cout << "Ready to accept request" << endl;

		// Run the endless loop
		//----------------------------------------
#ifdef WIN32_DBG
  InitAllocCheck(ACOutput_XML);
#endif
		tg->get_orb()->run();
	}
	catch (bad_alloc)
	{
		cerr << "Can't allocate memory to store device object !!!" << endl;
		cerr << "Exiting" << endl;
	}
	catch (CORBA::Exception &e)
	{
		Tango::Except::print_exception(e);
		
		cerr << "Received a CORBA_Exception" << endl;
		cerr << "Exiting" << endl;
	}
	tg->server_cleanup();
#ifdef WIN32_DBG
  DeInitAllocCheck();
#endif
	return(0);
#endif
}


/*PROTECTED REGION END*/
//========================================================
