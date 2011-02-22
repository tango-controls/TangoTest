//=============================================================================
//
// file :        Starter.h
//
// description : Include for the Starter class.
//
// project :	Starter for Tango Administration
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
//
// $Log$
// Revision 3.27  2010/09/21 12:18:58  pascal_verdier
// GPL Licence added to header.
//
// Revision 3.26  2010/02/09 15:09:49  pascal_verdier
// Define  _TG_WINDOWS_  replace WIN32.
// LogFileHome property added.
//
// Revision 3.25  2009/04/06 13:57:57  pascal_verdier
// IDL-4 (Tango-7) release.
//
// Revision 3.24  2008/09/23 14:19:40  pascal_verdier
// Log files history added.
//
// Revision 3.23  2008/04/09 14:46:11  pascal_verdier
// Bug in init of NotifdState attribute fixed.
//
// Revision 3.22  2008/04/07 08:54:55  pascal_verdier
// Check if this starter instance is able to run on this host.
//
// Revision 3.21  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
// Revision 3.20  2007/03/23 09:23:44  pascal_verdier
// Bug in windows batch file startup fixed.
//
// Revision 3.19  2007/02/12 13:06:51  pascal_verdier
// Spectrum String attribute management changed.
//
// Revision 3.18  2006/11/20 06:58:37  pascal_verdier
// Mutex on start process data added.
//
// Revision 3.17  2006/06/13 19:39:35  pascal_verdier
// Minor changes.
//
// Revision 3.16  2006/06/05 07:20:36  pascal_verdier
// Server startup is now delayed (with timeout) in a startup level.
// New state MOVING added.
// At startup, starter loop until properties have been read.
//
// Revision 3.15  2006/04/24 07:06:27  pascal_verdier
// A thread is started for each level when at servers startup.
//
// Revision 3.14  2006/02/09 11:59:18  pascal_verdier
// A ping thread is now started for each server.
//
// Revision 3.13  2006/02/08 07:13:55  pascal_verdier
// Minor changes.
//
// Revision 3.12  2005/09/30 12:01:50  pascal_verdier
// RunningServers and StoppedServers spectrum attribute max_x set to 200.
//
// Revision 3.11  2005/09/16 09:24:23  pascal_verdier
// For backward compatibility, set UseEvents property to boolean was not a good idea.
//
// Revision 3.10  2005/09/13 14:36:10  pascal_verdier
// Pogo-4.4.0 compatibility.
//
// Revision 3.9  2005/08/26 07:18:16  pascal_verdier
// FQDN management added.
// Time between startup level added.
//
// Revision 3.8  2005/05/27 13:16:39  pascal_verdier
// InterStartupLevelWait property has been added.
//
// Revision 3.7  2005/04/21 07:18:48  pascal_verdier
// Add a little timeout for ping.
// Fix a bug for windows (SIGCHLD).
//
// Revision 3.6  2004/12/10 08:57:19  pascal_verdier
// Tango 5 compatibility (attribute management).
//
// Revision 3.5  2004/07/05 11:01:20  pascal_verdier
// PB on service fixed.
//
// Revision 3.4  2004/07/05 08:04:30  pascal_verdier
// WIN32 project prolems fixed.
//
// Revision 3.3  2004/06/29 04:24:26  pascal_verdier
// First revision using events.
//
// Revision 3.2  2004/05/19 08:56:32  pascal_verdier
// Bug fixed on service mode.
// CreateProcess used under Windows.
//
// Revision 3.0.1.3  2004/02/27 09:53:02  pascal_verdier
// - The starter device is now warned by Database server when something change on a server.
// It replace the DbGetHostServersInfo polling.
// - HostState, ControlledRunningServers, ontrolledStoppedServers attributes added.
//
// Revision 3.0.1.2  2003/12/08 08:53:52  pascal_verdier
// Cluster (multi-host) control implemented.
// Control of notify daemon implemented but not tested.
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
// Revision 1.1  2000/07/31 13:48:42  verdier
// Initial revision
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//=============================================================================
#ifndef _STARTER_H
#define _STARTER_H

#include <tango.h>
#include <StarterUtil.h>
#include <sstream>

/**
 * @author	$Author$
 * @version	$Revision$
 */

 //	Add your own constants definitions here.
 //-----------------------------------------------
#define PING_TIMEOUT			3	//	seconds
#define	TIME_BETWEEN_STARTUPS	500	//	Milli seconds
#define NOTIFY_DAEMON_SCRIPT	"notify_daemon"

//	Used onlyduring the Cpp Api bug fixing on 
//	specAtt->get_max_x() method.
#define NB_SERVERS_MAX			200
#define SERVER_TIMEOUT		30	//	Timeout Minimum at server startup


#ifdef _TG_WINDOWS_
#	define	LOG_HOME	"c:\\temp"
#else
#	define	LOG_HOME	"/var/tmp"
#endif

//	Structure to difine a new process to start
typedef struct {
	bool	could_start;
	char	*adminname;
	char	*servname;
	char	*instancename;
	char	*logfile;
}
NewProcess;

namespace Starter_ns
{
class StartProcessShared;

/**
 * Class Description:
 * This device server is able to control <b>Tango</b> components (database, device servers, clients...).
 *	It is able to start or stop and to report the status of these components.
 */

/*
 *	Device States Description:
*  Tango::UNKNOWN :  This device server has NOT been correctly initialised.
*  Tango::ON :       All controlled servers are running.
*  Tango::ALARM :    At least one controled device server is not running.
*  Tango::MOVING :   The Sarter is starting servers.
 */


class Starter: public Tango::Device_4Impl
{
public :
	//	Add your own data members here
	//-----------------------------------------
	/**
	 *	Manage the PollingState object startup
	 */
	void manage_PollingState_startup();
	/**
	 *	Start process thread Shared data
	 */
	StartProcessShared	*start_proc_data;
	/**
	 *	Boolean used for init.
	 */
	bool	starting;
	/**
	 *	StarterUtil instance to be used everywhere.
	 */
	StarterUtil	*util;
	/**
	 *	Structures defining servers to be controlled
	 */
	vector<ControledServer>		servers;
	/**
	 *	Exception must be or not thrown.
	 */
	Tango::DevBoolean	throwable;
	/**
	 *	Static string to be returned.
	 */
	string	returned_str;
	/**
	 *	Database device (as DeviceProxy) for not implemented API commands.
	 */
	Tango::DeviceProxy	*dbase;
	/**
	 *	Notify daemon State.
	 */
	Tango::DevState	notifyd_state;
	/**
	 *	Is server list modification fired from DB ds.
	 */
	Tango::DevShort	fireFromDbase;
	bool	do_update_from_db;

	Tango::DevVarStringArray	stringArrayRunning;
	Tango::DevVarStringArray	stringArrayStopped;
	Tango::DevVarStringArray	stringArrayServers;


	//	Here is the Start of the automatic code generation part
	//-------------------------------------------------------------	
/**
 *	@name attributes
 *	Attributs member data.
 */
//@{
		Tango::DevState	*attr_NotifdState_read;
		Tango::DevShort	*attr_HostState_read;
		Tango::DevString	*attr_RunningServers_read;
		Tango::DevString	*attr_StoppedServers_read;
		Tango::DevString	*attr_Servers_read;
//@}

/**
 *	@name Device properties
 *	Device properties member data.
 */
//@{
/**
 *	Path to find executable files
 *	to start device servers
 */
	vector<string>	startDsPath;
/**
 *	The Starter will wait a bit before starting servers, to be sure than the drivers
 *	are started.This time is in seconds.
 */
	Tango::DevShort	waitForDriverStartup;
/**
 *	Use events if not null.
 */
	Tango::DevShort	useEvents;
/**
 *	Skip starting servers at startup if false.
 */
	Tango::DevBoolean	startServersAtStartup;
/**
 *	Time to wait before two startup levels in seconds.
 */
	Tango::DevLong	interStartupLevelWait;
/**
 *	Timeout on device server startup in seconds.
 */
	Tango::DevLong	serverStartupTimeout;
/**
 *	Number of log file kept.
 */
	Tango::DevLong	keepLogFiles;
/**
 *	The home directory to log servers traces.
 *	For Linux the default value is /var/tmp
 *	For Win32 it is c:\temp
 */
	string	logFileHome;
//@}

/**@name Constructors
 * Miscellaneous constructors */
//@{
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	Starter(Tango::DeviceClass *cl,string &s);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device Name
 */
	Starter(Tango::DeviceClass *cl,const char *s);
/**
 * Constructs a newly allocated Command object.
 *
 *	@param cl	Class.
 *	@param s 	Device name
 *	@param d	Device description.
 */
	Starter(Tango::DeviceClass *cl,const char *s,const char *d);
//@}

/**@name Destructor
 * Only one desctructor is defined for this class */
//@{
/**
 * The object desctructor.
 */	
	~Starter();
//@}

	
/**@name Miscellaneous methods */
//@{
/**
 *	Initialize the device
 */
	virtual void init_device();
/**
 *	Always executed method befor execution command method.
 */
	virtual void always_executed_hook();

//@}

/**
 * @name Starter methods prototypes
 */

//@{
/**
 *	Hardware acquisition for attributes.
 */
	virtual void read_attr_hardware(vector<long> &attr_list);
/**
 *	Extract real attribute values for NotifdState acquisition result.
 */
	virtual void read_NotifdState(Tango::Attribute &attr);
/**
 *	Extract real attribute values for HostState acquisition result.
 */
	virtual void read_HostState(Tango::Attribute &attr);
/**
 *	Extract real attribute values for RunningServers acquisition result.
 */
	virtual void read_RunningServers(Tango::Attribute &attr);
/**
 *	Extract real attribute values for StoppedServers acquisition result.
 */
	virtual void read_StoppedServers(Tango::Attribute &attr);
/**
 *	Extract real attribute values for Servers acquisition result.
 */
	virtual void read_Servers(Tango::Attribute &attr);
/**
 *	Read/Write allowed for NotifdState attribute.
 */
	virtual bool is_NotifdState_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for HostState attribute.
 */
	virtual bool is_HostState_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for RunningServers attribute.
 */
	virtual bool is_RunningServers_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for StoppedServers attribute.
 */
	virtual bool is_StoppedServers_allowed(Tango::AttReqType type);
/**
 *	Read/Write allowed for Servers attribute.
 */
	virtual bool is_Servers_allowed(Tango::AttReqType type);
/**
 *	Execution allowed for DevStartAll command.
 */
	virtual bool is_DevStartAll_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DevStopAll command.
 */
	virtual bool is_DevStopAll_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DevGetRunningServers command.
 */
	virtual bool is_DevGetRunningServers_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DevGetStopServers command.
 */
	virtual bool is_DevGetStopServers_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DevStart command.
 */
	virtual bool is_DevStart_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DevStop command.
 */
	virtual bool is_DevStop_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for DevReadLog command.
 */
	virtual bool is_DevReadLog_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for HardKillServer command.
 */
	virtual bool is_HardKillServer_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for NotifyDaemonState command.
 */
	virtual bool is_NotifyDaemonState_allowed(const CORBA::Any &any);
/**
 *	Execution allowed for UpdateServersInfo command.
 */
	virtual bool is_UpdateServersInfo_allowed(const CORBA::Any &any);
/**
 * This command gets the device state (stored in its <i>device_state</i> data member) and returns it to the caller.
 *	@return	State Code
 *	@exception DevFailed
 */
	virtual Tango::DevState	dev_state();
/**
 * Start all device servers controled on the host for the argin level.
 *	@param	argin	Startup level.
 *	@exception DevFailed
 */
	void	dev_start_all(Tango::DevShort);
/**
 * Stop all device servers controled on the host for the argin level.
 *	@param	argin	Startup Level.
 *	@exception DevFailed
 */
	void	dev_stop_all(Tango::DevShort);
/**
 * Control the running process from property list.
 *	And return the list of the processes which are really running.
 *	@param	argin	True for all servers. False for controled servers only.
 *	@return	List of the processes which are running.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*dev_get_running_servers(Tango::DevBoolean);
/**
 * Control the running process from property list.
 *	And return the list of the processes which are not running.
 *	@param	argin	True for all servers. False for controled servers only.
 *	@return	List of the processes which are not running.
 *	@exception DevFailed
 */
	Tango::DevVarStringArray	*dev_get_stop_servers(Tango::DevBoolean);
/**
 * Start the specified server.
 *	@param	argin	Server to be started.
 *	@exception DevFailed
 */
	void	dev_start(Tango::DevString);
/**
 * Stop the specified server.
 *	@param	argin	Servero be stopped.
 *	@exception DevFailed
 */
	void	dev_stop(Tango::DevString);
/**
 * At server startup, its standard error is redirected to a log file.
 *	This command will read this file and return the read string from the file.
 *	@param	argin	server name and domain
 *	@return	ig Starter/corvus)
 *	@exception DevFailed
 */
	Tango::ConstDevString	dev_read_log(Tango::DevString);
/**
 * Hard kill a server (kill -9)
 *	@param	argin	Server name
 *	@exception DevFailed
 */
	void	hard_kill_server(Tango::DevString);
/**
 * Returns the Notify Daemon state.
 *	@return	Tango::ON if Notify daemon is running else Tango::FAULT.
 *	@exception DevFailed
 */
	Tango::DevState	notify_daemon_state();
/**
 * Indicate to the device server than the information about servers to be controlled has been modified.
 *	The device server must read the database to update the servers info list.
 *	If the default case, this command is sent by Database server itself.
 *	@exception DevFailed
 */
	void	update_servers_info();

/**
 *	Read the device properties from database
 */
	 void get_device_property();
//@}

	//	Here is the end of the automatic code generation part
	//-------------------------------------------------------------	



protected :	
	//	Add your own data members here
	//-----------------------------------------
/**
 * @name private methods prototypes
 */
//@{
/**
 *	Allocate and fill the servers controled object
 */
void build_server_ctrl_object(void);
/**
 *	Return how many servers to start for specified level.
 */
int	nb_servers_to_start(int level);
/**
 *	Check if a process could be started (file exists, is not running, ...)
 */
NewProcess *processCouldStart(char *);
/**
 *	Start a thread to start processes
 */
void startProcesses(vector<NewProcess *>, int level);

/**
 *	check if instance and host name are coherent
 */
void check_host();
//@}
};





//=========================================================
/**
 *	Shared data between DS and StartProcess thread.
 */
//=========================================================
class StartProcessShared: public Tango::TangoMonitor
{
private:
	/**
	 *	Manage levels thread
	 *	To be shure that (level) threads are not concurent
	 */
	vector<int>	start_process_thread_levels;
	/**
	 *	Starter is MOVING (starting servers) when > 0;
	 */
	int	starting_processes;

public:
	StartProcessShared() {starting_processes=0;};

	void push_back_level(int level);
	void remove_current_level();
	int  get_current_level();
	int  get_starting_processes();

};


#ifdef _TG_WINDOWS_
class StartWinThread: public omni_thread
{
	NewProcess	*process;
	Starter		*starter;
public:
	StartWinThread(NewProcess* proc, Starter *st) 
		{ process = proc; starter=st;};

	// Set the path between cotes for windows.
	string get_server_name_with_cotes(string servname);
	
	void *run_undetached(void *);
	void start() {start_undetached();}
};
#endif

//=========================================================
/**
 *	Create a thread to fork a sub-process (new device server)
 */
//=========================================================
class StartProcessThread: public omni_thread
{
	vector<NewProcess *>	processes;
	Starter					*starter;
	int						thread_level;
public:
/**
 *	Initialize the sub process parameters (name, domain, log_file).
 */
	StartProcessThread(vector<NewProcess *> v_np, int level, Starter *st);
/**
 * Execute the fork of the sub process in a thread.
 */
	void *run_undetached(void *);
	void start() {start_undetached();}
/**
 *	Start one process
 */
 	void start_process(NewProcess *);
};

}	//	namespace

#endif	// _STARTER_H
