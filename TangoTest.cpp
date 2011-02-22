static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         TangoTest.cpp
//
// description :  C++ source for the TangoTest and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                TangoTest are implemented in this file.
//
// project :      $TANGO Device Server for testing generic clients
//
// $Author$     $N.Leclercq - SOLEIL
//
// $Revision$ 
// 
// $Log$
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
// 
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//         (c) - Software Engineering Group - ESRF
//=============================================================================


//===================================================================
//
//	The folowing table gives the correspondance
//	between commands and method's name.
//
//	Command's name	|	Method's name
//	----------------------------------------
//	State	|	dev_state()
//	Status	|	dev_status()
//	DevVoid	|	dev_void()
//	DevBoolean	|	dev_boolean()
//	DevShort	|	dev_short()
//	DevUShort	|	dev_ushort()
//	DevLong	|	dev_long()
//	DevULong	|	dev_ulong()
//	DevFloat	|	dev_float()
//	DevDouble	|	dev_double()
//	DevString	|	dev_string()
//	DevVarCharArray	|	dev_var_char_array()
//	DevVarShortArray	|	dev_var_short_array()
//	DevVarUShortArray	|	dev_var_ushort_array()
//	DevVarLongArray	|	dev_var_long_array()
//	DevVarULongArray	|	dev_var_ulong_array()
//	DevVarFloatArray	|	dev_var_float_array()
//	DevVarDoubleArray	|	dev_var_double_array()
//	DevVarStringArray	|	dev_var_string_array()
//	DevVarLongStringArray	|	dev_var_long_string_array()
//	DevVarDoubleStringArray	|	dev_var_double_string_array()
//
//===================================================================
#include <math.h>
#include <tango.h>
#include <TangoTest.h>

static const long kSpecLen = 256;
static const long kImagLen = 251;

namespace TangoTest
{

//=============================================================================
// Standalone template function: generates a random <T> in the range [0, max]
//=============================================================================
template<typename T> inline T randomize (T max)
{
  if (max > 0) {
    int r = ((r = ::rand()) < 0) ? -r : r;
    return (T)(r % (int)max);
  }
  return 0;
}

//=============================================================================
// Standalone template function: generates a dummy beam image
//=============================================================================
template <typename T>
void generate_image (T* buffer, long max_xy)
{
	long bimg_center_x = ::rand() % (int)(max_xy * 0.05);
  if (::rand() % 2) {
    bimg_center_x *= -1;
  }
	long bimg_center_y = ::rand() % (int)(max_xy * 0.05);
  if (::rand() % 2) {
    bimg_center_y *= -1;
  }
	long bimg_offset_to_zero = (max_xy - 1) / 2;
	long bimg_x_offset_to_zero = bimg_offset_to_zero + bimg_center_x;
	long bimg_y_offset_to_zero = bimg_offset_to_zero + bimg_center_y;  
  long limit = max_xy / 8;
  long noise = ::rand() % (int)(limit * 0.2);
  if (::rand() % 2) {
    noise *= -1; 
  }
  limit += noise;
  // Reset image content
  ::memset(buffer, 0, max_xy * max_xy * sizeof(T));
  // Fill image buffer 
  long i, j, x, y, value;
  for (i = -limit; i < limit; i++) {
    y = i + bimg_y_offset_to_zero;
    if (y >= 0 && y < max_xy) {
      for (j = -limit; j < limit; j++) {
	      x = j + bimg_x_offset_to_zero;
        if (x >= 0 && x < max_xy) { 
          value = (long)::sqrt((double)(i * i + j * j));
          buffer[x * max_xy + y] =  (value < limit) ? limit - value : 0;	 		
        }	
      }
    }
  }
}

//=============================================================================
// Class: DataGenerator (thread)  
//=============================================================================
class DataGenerator : public omni_thread, public Tango::LogAdapter
{ 
public:
  DataGenerator::DataGenerator (TangoTest& dev, long sleep_time)
    : omni_thread(), 
      Tango::LogAdapter(&dev), 
      dev_(dev), 
      sleep_time_(sleep_time), 
      go_on_(1) 
  {
    // noop ctor
  }

  void go (void) 
  {
    DEBUG_STREAM << "DataGenerator::go" << endl;
    start_undetached();
  }  

  virtual void* run_undetached (void *) 
  {
    DEBUG_STREAM << "DataGenerator::run_undetached" << endl;
    do 
    {
      { //- enter critical section
        omni_mutex_lock guard(dev_.lock);
        if (!go_on_) break;
        DEBUG_STREAM << "DataGenerator::generating data" << endl;
        dev_.gen_data();
      } //- leave critical section
      sleep(0, sleep_time_ * 1000000);
    }
    while (go_on_);
    return 0; 
  }

  void abort (void) 
  {
    DEBUG_STREAM << "DataGenerator::abort" << endl;
    go_on_ = 0;
  }  

protected:
  virtual DataGenerator::~DataGenerator (void)
  {
    // noop dtor
  }

private:  
  int go_on_;
  TangoTest& dev_;
  long sleep_time_;  
};

//=============================================================================
// TangoTest::gen_data
//=============================================================================
void TangoTest::gen_data ()
{
  // Generate data for all attributes
 	static long k = 0;
  static const double kpi_deg = 3.14159 / 180.0;

  *attr_short_scalar_rww_read = 
			randomize(attr_short_scalar_w_write); 

  *attr_long_scalar_rww_read =
     	randomize(attr_long_scalar_w_write);

  *attr_double_scalar_rww_read = 
       attr_double_scalar_w_write * ::sin(kpi_deg * (k%360));

  *attr_short_scalar_ro_read =     
     randomize(attr_short_scalar_write);

  *attr_short_scalar_read =     
     randomize(attr_short_scalar_write);

 	*attr_long_scalar_read =
     randomize(attr_long_scalar_write);

 	*attr_double_scalar_read =
     attr_double_scalar_write * ::sin(kpi_deg * (k%360));

 	long i;

	for (i = 0; i < kSpecLen; i++)
		attr_short_spectrum_read[i] = randomize(256);

	for (i = 0; i < kSpecLen; i++)
		attr_long_spectrum_read[i] = randomize(256);

	for (i = 0; i < kSpecLen; i++)
		attr_double_spectrum_read[i] = randomize(256);

  for (i = 0; i < kSpecLen; i++)
     attr_wave_read[i] = attr_ampli_write * ::sin(kpi_deg * (i + (k%360)));

  generate_image(attr_short_image_read,  kImagLen);

  generate_image(attr_long_image_read,  kImagLen);

  generate_image(attr_double_image_read,  kImagLen);

  k++;
}

//=============================================================================
// TangoTest::TangoTest
//=============================================================================
TangoTest::TangoTest(Tango::DeviceClass *cl,string &s)
  : Tango::Device_2Impl (cl, s.c_str())
{
	init_device();
} 

//=============================================================================
// TangoTest::TangoTest
//=============================================================================
TangoTest::TangoTest(Tango::DeviceClass *cl,const char *s)
  : Tango::Device_2Impl (cl, s)
{
	init_device();
}

//=============================================================================
// TangoTest::TangoTest
//=============================================================================
TangoTest::TangoTest(Tango::DeviceClass *cl,const char *s,const char *d)
  : Tango::Device_2Impl (cl,s,d)
{
	init_device();
}

//=============================================================================
// TangoTest::~TangoTest
//=============================================================================
TangoTest::~TangoTest() 
{
  DEBUG_STREAM << "TangoTest::~TangoTest::destroy device " << get_name() << std::endl;
  delete_device();
}

//=============================================================================
// TangoTest::init_device
//=============================================================================
void TangoTest::init_device()
{
  LOG_INFO(("TangoTest::init_device::init device %s", device_name.c_str()));

  omni_mutex_lock guard(lock);

	// Initialise variables to default values
	//-------------------------------------------------------

  // Allocate/initialize read part for each readable attribute
  // Initialize write part for each writable attribute

  attr_short_scalar_ro_read = new Tango::DevShort;
	*attr_short_scalar_ro_read = 0;

	attr_short_scalar_read = new Tango::DevShort;
	*attr_short_scalar_read = 0;
  attr_short_scalar_write = 256;   

	attr_long_scalar_read = new Tango::DevLong;
	*attr_long_scalar_read = 0; 
  attr_long_scalar_write = 256; 

	attr_double_scalar_read = new Tango::DevDouble;
	*attr_double_scalar_read = 0;
  attr_double_scalar_write = 256;

  attr_short_scalar_w_write = 256;
  attr_short_scalar_rww_read = new Tango::DevShort;
  *attr_short_scalar_rww_read = 0;

  attr_long_scalar_w_write = 256;
  attr_long_scalar_rww_read = new Tango::DevLong;
  *attr_long_scalar_rww_read = 0;

  attr_double_scalar_w_write = 256;
  attr_double_scalar_rww_read = new Tango::DevDouble;
  *attr_double_scalar_rww_read = 0;

	attr_string_scalar_read = new char*;
	*attr_string_scalar_read = new char[256];
  ::strcpy(*attr_string_scalar_read, "Default string");
  attr_string_scalar_write = 0;

	attr_short_spectrum_read = new Tango::DevShort[kSpecLen];
	::memset(attr_short_spectrum_read, 0, kSpecLen * sizeof(Tango::DevShort));

	attr_long_spectrum_read = new Tango::DevLong[kSpecLen];
	::memset(attr_long_spectrum_read, 0, kSpecLen * sizeof(Tango::DevLong));

	attr_double_spectrum_read = new Tango::DevDouble[kSpecLen];
	::memset(attr_double_spectrum_read, 0, kSpecLen * sizeof(Tango::DevDouble));

	attr_short_image_read = new Tango::DevShort[kImagLen * kImagLen];
	::memset(attr_short_image_read, 0, kImagLen * kImagLen * sizeof(Tango::DevShort));

	attr_long_image_read = new Tango::DevLong[kImagLen * kImagLen];
	::memset(attr_long_image_read, 0, kImagLen * kImagLen * sizeof(Tango::DevLong));

	attr_double_image_read = new Tango::DevDouble[kImagLen * kImagLen];
	::memset(attr_double_image_read, 0, kImagLen * kImagLen * sizeof(Tango::DevDouble));

  attr_wave_read = new Tango::DevDouble[kSpecLen];
	::memset(attr_wave_read, 0, kSpecLen * sizeof(Tango::DevDouble));
  attr_ampli_write= 1;

  // Get device property from database
  get_device_property();

  // Multithreaded implementation stuffs
  if (mthreaded_impl == 0) {
   data_gen = 0;
   return; 
  }
  data_gen = new DataGenerator(*this, sleep_period);
  data_gen->go();
}

//=============================================================================
// TangoTest::delete_device
//=============================================================================
void TangoTest::delete_device()
{
  DEBUG_STREAM << "In TangoTest::delete_device for " << this->get_name() << endl;

  omni_mutex_lock guard(lock);

  if (mthreaded_impl && data_gen) {
    data_gen->abort();
    data_gen->join(0);
    data_gen = 0;
  } 

  if (attr_short_scalar_ro_read) {
    delete attr_short_scalar_ro_read;
    attr_short_scalar_ro_read = 0;
  }

  if (attr_short_scalar_read) {
    delete attr_short_scalar_read;
    attr_short_scalar_read = 0;
  }

  if (attr_long_scalar_read) {
    delete attr_long_scalar_read;
    attr_long_scalar_read = 0;
  }

  if (attr_double_scalar_read) {
	  delete attr_double_scalar_read;
    attr_double_scalar_read = 0;
  }

  if (attr_short_scalar_rww_read) {
	  delete attr_short_scalar_rww_read;
    attr_short_scalar_rww_read = 0;
  }

  if (attr_long_scalar_rww_read) {
	  delete attr_long_scalar_rww_read;
    attr_long_scalar_rww_read = 0;
  }

  if (attr_double_scalar_rww_read) {
	  delete attr_double_scalar_rww_read;
    attr_double_scalar_rww_read = 0;
  }

  if (*attr_string_scalar_read) {
    delete *attr_string_scalar_read;
  }

  if (attr_string_scalar_read) {
	  delete attr_string_scalar_read;
    attr_string_scalar_read = 0;
  }

  if (attr_short_spectrum_read) {
    delete[] attr_short_spectrum_read;
    attr_short_spectrum_read = 0;
  }

  if (attr_long_spectrum_read) {
    delete[] attr_long_spectrum_read;
    attr_long_spectrum_read = 0;
  }

  if (attr_double_spectrum_read) {
    delete[] attr_double_spectrum_read;
    attr_double_spectrum_read = 0;
  }

  if (attr_short_image_read) {
    delete[] attr_short_image_read;
    attr_short_image_read = 0;
  }

  if (attr_long_image_read) {
    delete[] attr_long_image_read;
    attr_long_image_read = 0;
  }

  if (attr_double_image_read) {
    delete[] attr_double_image_read;
    attr_double_image_read = 0;
  }

  if (attr_wave_read) {
    delete[] attr_wave_read;
    attr_wave_read = 0;
  }
}

//=============================================================================
// TangoTest::get_device_property
//=============================================================================
void TangoTest::get_device_property (void)
{
  //	Initialize your default values here.
	//------------------------------------------
	mthreaded_impl = 1;
	sleep_period = 250;

	//	Read class properties from database.(Automatic code generation)
	//-------------------------------------------------------------
	Tango::DbData	data;
	data.push_back(Tango::DbDatum("mthreaded_impl"));
	data.push_back(Tango::DbDatum("sleep_period"));

  try {
	  //	Call database and extract values
	  //--------------------------------------------
	  get_db_device()->get_property(data);
	  if (data[0].is_empty()==false) data[0] >> mthreaded_impl;
	  if (data[1].is_empty()==false) data[1] >> sleep_period;

	  //	End of Automatic code generation
	  //-------------------------------------------------------------
	  DEBUG_STREAM << "mthreaded_impl = " << mthreaded_impl << endl;
    DEBUG_STREAM << "sleep_period = " << sleep_period << endl;
  }
  catch (const Tango::DevFailed&) 
  {

  }
}

//=============================================================================
// TangoTest::always_executed_hook
//=============================================================================
void TangoTest::always_executed_hook ()
{
	DEBUG_STREAM << "In always_executed_hook method" << endl;
}

//=============================================================================
// TangoTest::write_attr_hardware
//=============================================================================
void TangoTest::write_attr_hardware(vector<long> &attr_list)
{
	DEBUG_STREAM << "In write_attr_hardware for " << attr_list.size() << " attribute(s)" << endl;

	for (long i=0 ; i < attr_list.size() ; i++)
	{
		Tango::WAttribute &att = dev_attr->get_w_attr_by_ind(attr_list[i]);
		string attr_name = att.get_name();

		DEBUG_STREAM << "Attribute name = " << attr_name << endl;		

		//	Switch on attribute name
		//---------------------------------
		if (attr_name == "string_scalar")
		{
      att.get_write_value(attr_string_scalar_write);
      DEBUG_STREAM << "attr_string_scalar_write = " << attr_string_scalar_write << endl;
      if (*attr_string_scalar_read) {
        delete[] *attr_string_scalar_read;
        size_t len = ::strlen(attr_string_scalar_write) + 1;
        *attr_string_scalar_read = new char[len];
        if (*attr_string_scalar_read) {
          ::memset(*attr_string_scalar_read, 0, len * sizeof(char));
          ::strcpy(*attr_string_scalar_read, attr_string_scalar_write);
        }
      }
		}
		if (attr_name == "double_scalar")
		{
			att.get_write_value(attr_double_scalar_write);
      DEBUG_STREAM << "attr_double_scalar_write = " << attr_double_scalar_write << endl;
		}
		if (attr_name == "long_scalar")
		{
	    att.get_write_value(attr_long_scalar_write);
      DEBUG_STREAM << "attr_long_scalar_write = " << attr_long_scalar_write << endl;
		}
		if (attr_name == "short_scalar")
		{
			att.get_write_value(attr_short_scalar_write);
      DEBUG_STREAM << "attr_short_scalar_write = " << attr_short_scalar_write << endl;
		}
    if (attr_name == "short_scalar_w")
		{
			att.get_write_value(attr_short_scalar_w_write);
      DEBUG_STREAM << "short_scalar_w = " << attr_short_scalar_w_write << endl;
		}
    if (attr_name == "long_scalar_w")
		{
			att.get_write_value(attr_long_scalar_w_write);
      DEBUG_STREAM << "long_scalar_w = " << attr_long_scalar_w_write << endl;
    }
    if (attr_name == "double_scalar_w")
		{
			att.get_write_value(attr_double_scalar_w_write);
      DEBUG_STREAM << "double_scalar_w = " << attr_double_scalar_w_write << endl;
    }
    if (attr_name == "ampli")
		{
			att.get_write_value(attr_ampli_write);
      DEBUG_STREAM << "ampli = " << attr_ampli_write << endl;
		}
	}
}

//=============================================================================
// TangoTest::read_attr_hardware
//=============================================================================
void TangoTest::read_attr_hardware(vector<long> &attr_list)
{
  LOG_DEBUG(("In read_attr_hardware for %d attributes",attr_list.size()));

  // Generate data for all attributes
  if (mthreaded_impl == 0) { 
 	  gen_data();
  }
} 

//=============================================================================
// TangoTest::read_attr
//=============================================================================
void TangoTest::read_attr(Tango::Attribute &attr)
{
	string &attr_name = attr.get_name();
  
  LOG_DEBUG(("Reading attribute %s", attr_name.c_str()));

	//	Switch on attribute name
	//---------------------------------
	if (attr_name == "double_image")
	{
		attr.set_value(attr_double_image_read, kImagLen, kImagLen);
	}
	if (attr_name == "long_image")
	{
		attr.set_value(attr_long_image_read, kImagLen, kImagLen);
	}
	if (attr_name == "short_image")
	{
		attr.set_value(attr_short_image_read, kImagLen, kImagLen);
	}
	if (attr_name == "double_spectrum")
	{
		attr.set_value(attr_double_spectrum_read, kSpecLen);
	}
	if (attr_name == "long_spectrum")
	{
		attr.set_value(attr_long_spectrum_read, kSpecLen);
	}
	if (attr_name == "short_spectrum")
	{
		attr.set_value(attr_short_spectrum_read, kSpecLen);
	}
	if (attr_name == "string_scalar")
	{
		attr.set_value(attr_string_scalar_read);
	}
	if (attr_name == "double_scalar")
	{
		attr.set_value(attr_double_scalar_read);
	}
	if (attr_name == "long_scalar")
	{
		attr.set_value(attr_long_scalar_read);
	}
	if (attr_name == "short_scalar_ro")
	{
		attr.set_value(attr_short_scalar_ro_read);
	}
  if (attr_name == "short_scalar")
	{
		attr.set_value(attr_short_scalar_read);
	}
  if (attr_name == "short_scalar_rww")
	{
		attr.set_value(attr_short_scalar_rww_read);
	}
  if (attr_name == "long_scalar_rww")
	{
		attr.set_value(attr_long_scalar_rww_read);
	}
  if (attr_name == "double_scalar_rww")
	{
		attr.set_value(attr_double_scalar_rww_read);
	}
  if (attr_name == "wave") 
	{
		attr.set_value(attr_wave_read, kSpecLen);
	}
} 

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_void
 *
 *	description:	method to execute "DevVoid"
 *	A DevVoid comand example
 *
 *
 */
//+------------------------------------------------------------------
void TangoTest::dev_void ()
{
	DEBUG_STREAM << "TangoTest::dev_void(): entering... !" << endl;
	//	Add your own code to control device here
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_boolean
 *
 *	description:	method to execute "DevBoolean"
 *	A DevBoolean comand example
 *
 * @param	argin	Any boolean value
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevBoolean TangoTest::dev_boolean(Tango::DevBoolean argin)
{
	DEBUG_STREAM << "TangoTest::dev_boolean(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_short
 *
 *	description:	method to execute "DevShort"
 *	A DevShort command example
 *
 * @param	argin	Any DevShort value
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevShort TangoTest::dev_short(Tango::DevShort argin)
{
	DEBUG_STREAM << "TangoTest::dev_short(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_ushort
 *
 *	description:	method to execute "DevUShort"
 *	A DevUShort command example
 *
 * @param	argin	Any DevUShort value
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevUShort TangoTest::dev_ushort (Tango::DevUShort argin)
{
	DEBUG_STREAM << "TangoTest::dev_ushort(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_long
 *
 *	description:	method to execute "DevLong"
 *	A DevLong command example
 *
 * @param	argin	Any DevLong value
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevLong TangoTest::dev_long (Tango::DevLong argin)
{
	DEBUG_STREAM << "TangoTest::dev_long(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_ulong
 *
 *	description:	method to execute "DevULong"
 *	A DevULong command example
 *
 * @param	argin	Any DevULong
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevULong TangoTest::dev_ulong (Tango::DevULong argin)
{
	DEBUG_STREAM << "TangoTest::dev_ulong(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_float
 *
 *	description:	method to execute "DevFloat"
 *	A DevFloat command example
 *
 * @param	argin	Any DevFloat value
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevFloat TangoTest::dev_float (Tango::DevFloat argin)
{
	DEBUG_STREAM << "TangoTest::dev_float(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_double
 *
 *	description:	method to execute "DevDouble"
 *	A DevDouble command example
 *
 * @param	argin	Any DevDouble value
 * @return	Echo of the argin value
 *
 */
//+------------------------------------------------------------------
Tango::DevDouble TangoTest::dev_double (Tango::DevDouble argin)
{
	DEBUG_STREAM << "TangoTest::dev_double(): entering... !" << endl;
  
  // Tangotest : just send back input argument
	return argin;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_string
 *
 *	description:	method to execute "DevString"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevString TangoTest::dev_string (Tango::DevString argin)
{
  DEBUG_STREAM << "TangoTest::dev_string(): entering... !" << endl;

  // Allocate argout
  Tango::DevString argout = new char [::strlen(argin) + 1];
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevString" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_string");
  }

  // Tangotest : just send back input argument
	::strcpy(argout, argin);

	return argout;
} 

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_char_array
 *
 *	description:	method to execute "DevVarCharArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarCharArray *TangoTest::dev_var_char_array (const Tango::DevVarCharArray *argin)
{
  DEBUG_STREAM << "TangoTest::dev_var_char_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarCharArray *argout = new Tango::DevVarCharArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarCharArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_char_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_short_array
 *
 *	description:	method to execute "DevVarShortArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarShortArray *TangoTest::dev_var_short_array (const Tango::DevVarShortArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_short_array(): entering... !" << endl;

  // Allocate argout
  Tango::DevVarShortArray	*argout = new Tango::DevVarShortArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarShortArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_short_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_ushort_array
 *
 *	description:	method to execute "DevVarUShortArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarUShortArray *TangoTest::dev_var_ushort_array (const Tango::DevVarUShortArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_ushort_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarUShortArray	*argout = new Tango::DevVarUShortArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarUShortArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_ushort_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_long_array
 *
 *	description:	method to execute "DevVarLongArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongArray *TangoTest::dev_var_long_array (const Tango::DevVarLongArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_long_array(): entering... !" << endl;

  // Allocate argout
  Tango::DevVarLongArray	*argout = new Tango::DevVarLongArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarLongArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_long_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_ulong_array
 *
 *	description:	method to execute "DevVarULongArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarULongArray *TangoTest::dev_var_ulong_array (const Tango::DevVarULongArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_ulong_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarULongArray	*argout = new Tango::DevVarULongArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarULongArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_ulong_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_float_array
 *
 *	description:	method to execute "DevVarFloatArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarFloatArray *TangoTest::dev_var_float_array (const Tango::DevVarFloatArray *argin)
{
  DEBUG_STREAM << "TangoTest::dev_var_float_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarFloatArray	*argout = new Tango::DevVarFloatArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarFloatArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_float_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_double_array
 *
 *	description:	method to execute "DevVarDoubleArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarDoubleArray *TangoTest::dev_var_double_array (const Tango::DevVarDoubleArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_double_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarDoubleArray	*argout = new Tango::DevVarDoubleArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarDoubleArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_double_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = (*argin)[i];
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_string_array
 *
 *	description:	method to execute "DevVarStringArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *TangoTest::dev_var_string_array (const Tango::DevVarStringArray *argin)
{
  DEBUG_STREAM << "TangoTest::dev_var_string_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarStringArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_string_array");
  }

  // Tangotest : just send back input argument
  argout->length(argin->length());
  for (int i = 0; i < argin->length(); i++) {
    (*argout)[i] = CORBA::string_dup((*argin)[i]);
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_long_string_array
 *
 *	description:	method to execute "DevVarLongStringArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongStringArray *TangoTest::dev_var_long_string_array (const Tango::DevVarLongStringArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_long_string_array(): entering... !" << endl;

  // Allocate argout
	Tango::DevVarLongStringArray	*argout = new Tango::DevVarLongStringArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarLongStringArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_long_string_array");
  }

  // Tangotest : just send back input argument
  argout->lvalue = argin->lvalue;
  argout->svalue.length(argin->svalue.length());
  for (int i = 0; i < argin->svalue.length(); i++) {
    argout->svalue[i] = CORBA::string_dup(argin->svalue[i]);
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	TangoTest::dev_var_double_string_array
 *
 *	description:	method to execute "DevVarDoubleStringArray"
 *	none
 *
 * @param	argin	-
 * @return	-
 *
 */
//+------------------------------------------------------------------
Tango::DevVarDoubleStringArray *TangoTest::dev_var_double_string_array (const Tango::DevVarDoubleStringArray *argin)
{
	DEBUG_STREAM << "TangoTest::dev_var_double_string_array(): entering... !" << endl;

  // Allocate argout
  Tango::DevVarDoubleStringArray *argout = new Tango::DevVarDoubleStringArray();
  if (argout == 0) {
    TangoSys_OMemStream o;
    o << "Failed to allocate Tango::DevVarDoubleStringArray" << ends;
    LOG_ERROR((o.str()));
    Tango::Except::throw_exception((const char *)"Out of memory error",
						                       o.str(),
						                       (const char *)"TangoTest::dev_var_double_string_array");
  }

  // Tangotest : just send back input argument
  argout->dvalue = argin->dvalue;
  argout->svalue.length(argin->svalue.length());
  for (int i = 0; i < argin->svalue.length(); i++) {
    argout->svalue[i] = CORBA::string_dup(argin->svalue[i]);
  }

  // DO NOT DELETE <argin>
  // See the TANGO programmer's manual section 7.2.3
  // for more info about C++ memory management.

	return argout;
}

}	//	namespace
