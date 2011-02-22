static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :        main.cpp
//
// description : C++ source for a TANGO device server main.
//               The main rule is to initialise (and create) the Tango
//               system and to create the DServerClass singleton.
//               The main should be the same for every Tango device server.
//
// project :     TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.3  2005/03/24 15:35:30  pascal_verdier
// Add sigset for signal problem on solaris.
//
// Revision 3.2  2004/05/19 08:56:32  pascal_verdier
// Bug fixed on service mode.
// CreateProcess used under Windows.
//
// Revision 3.0.1.3  2003/12/08 08:53:52  pascal_verdier
// Cluster (multi-host) control implemented.
// Control of notify daemon implemented but not tested.
//
// Revision 3.0.1.2  2003/10/15 10:37:08  pascal_verdier
// *** empty log message ***
//
// Revision 3.0.1.1  2003/09/18 12:02:50  pascal_verdier
// Problem on Windows service startup fixed.
//
// Revision 3.0  2003/06/17 12:06:36  pascal_verdier
// TANGO 3.x server.
// polling bugs fixed.
//
// Revision 2.0  2003/01/09 13:35:50  verdier
// TANGO 2.2
//
// Revision 1.22  2002/12/18 08:09:19  verdier
// omniORB compatibility
//
// Revision 1.6  2002/10/15 18:55:21  verdier
// The host state is now calculated during the State polled command.
//
// Revision 1.5  2002/02/27 15:19:32  verdier
// WinNt services implemented.
//
// Revision 1.4  2001/04/03 09:40:50  verdier
// WInNt version is running.
//
// Revision 1.3  2001/02/12 09:34:21  verdier
// SunOS and Linux OK.
//
// Revision 1.2  2000/10/12 08:54:13  verdier
// Compatible with tango 2 done
//  Update from database info added.
//
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//             (c) - Pascal Verdier - ESRF
//=============================================================================

#ifdef WIN32
//#define USE_SERVICE
#endif

#ifdef sun
#	include <signal.h>
#endif


#include <tango.h>
#include <string.h>
#include <Starter.h>


#ifdef USE_SERVICE
#include <StarterService.h>
using namespace std;
#endif


int main(int argc,char *argv[])
{
	//	First time set SIGINT to default to do not block
	//	signal handling for child proceeses
	//---------------------------------------------------
#ifdef sun
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
	return(0);
#endif
}
