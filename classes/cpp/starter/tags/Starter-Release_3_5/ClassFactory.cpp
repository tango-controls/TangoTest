static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :        ClassFactory.cpp
//
// description : C++ source for the class_factory method of the DServer
//               device class. This method is responsible to create
//               all class singletin for a device server. It is called
//               at device server startup
//
// project :     TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.2  2004/05/19 08:56:32  pascal_verdier
// Bug fixed on service mode.
// CreateProcess used under Windows.
//
// Revision 3.0.1.1  2003/09/18 12:02:49  pascal_verdier
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


#include <tango.h>
#include <StarterClass.h>

/**
 *	Create StarterClass singleton and store it in DServer object.
 *
 * @author	$Author$
 * @version	$Revision$
 */

void Tango::DServer::class_factory()
{

	add_class(Starter_ns::StarterClass::init("Starter"));

}
