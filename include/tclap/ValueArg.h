
/****************************************************************************** 
 * 
 *  file:  ValueArg.h
 * 
 *  Copyright (c) 2003, Michael E. Smoot .
 *  All rights reverved.
 * 
 *  See the file COPYING in the top directory of this distribution for
 *  more information.
 *  
 *  THE SOFTWARE IS PROVIDED _AS IS_, WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 *  DEALINGS IN THE SOFTWARE.  
 *  
 *****************************************************************************/ 


#ifndef __VALUE_ARGUMENT_HH__
#define __VALUE_ARGUMENT_HH__

#include <string>
#include <vector>
#include <sstream>
#include <tclap/Visitor.h>
#include <tclap/Arg.h>

using namespace std;

namespace TCLAP {

/**
 * The basic labeled argument that parses a value.
 * This is a template class, which means the type T defines the type
 * that a given object will attempt to parse when the flag/name is matched
 * on the command line.  While there is nothing stopping you from creating
 * an unflagged ValueArg, it is unwise and would cause significant problems.
 * Instead use an UnlabeledValueArg.
 */
template<class T>
class ValueArg : public Arg
{
	protected:

		/**
		 * The value parsed from the command line.
		 * Can be of any type, as long as the >> operator for the type
		 * is defined.
		 */
		T _value;

		/**
		 * A human readable description of the type to be parsed.
		 * This is a hack, plain and simple.  Ideally we would use RTTI to
		 * return the name of type T, but until there is some sort of
		 * consistent support for human readable names, we are left to our
		 * own devices.
		 */
		string _typeDesc;

		/**
		 * Extracts the value from the string.
		 * Attempts to parse string as type T, if this fails an exception
		 * is thrown.
		 * \param val - string value to be parsed. 
		 */
		void _extractValue( const string& val ); 
 
	public:

		/**
		 * Labeled ValueArg constructor.
		 * You could conceivably call this constructor with a blank flag, 
		 * but that would make you a bad person.  It would also cause
		 * an exception to be thrown.   If you want an unlabeled argument, 
		 * use the other constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param value - The default value assigned to this argument if it
		 * is not present on the command line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		ValueArg(const string& flag, 
				 const string& name, 
			     const string& desc, 
				 bool req, 
				 T value,
				 const string& typeDesc,
				 Visitor* v = NULL);

		/**
		 * Destructor.
		 */ 
		~ValueArg();

		/**
		 * Handles the processing of the argument.
		 * This re-implements the Arg version of this method to set the
		 * _value of the argument appropriately.  It knows the difference
		 * between labeled and unlabeled.
		 * \param i - Pointer the the current argument in the list.
		 * \param args - Mutable list of strings. Passed 
		 * in from main().
		 */
		virtual bool processArg(int* i, vector<string>& args); 

		/**
		 * Returns the value of the argument.
		 */
		T& getValue() ;

		/**
		 * Specialization of shortID.
		 */
		virtual string shortID(const string& val = "val") const;

		/**
		 * Specialization of longID.
		 */
		virtual string longID(const string& val = "val") const;

};



/**
 * Constructor implementation.
 */
template<class T>
ValueArg<T>::ValueArg(const string& flag, 
				      const string& name, 
					  const string& desc, 
					  bool req, 
					  T val,
					  const string& typeDesc,
					  Visitor* v)
: Arg(flag, name, desc, req, true, v),
  _value( val ),
  _typeDesc( typeDesc )
{ };

/**
 * Destructor implementation.
 */
template<class T>
ValueArg<T>::~ValueArg() { };

/**
 * Implementation of getValue().
 */
template<class T>
T& ValueArg<T>::getValue() { return _value; };

/**
 * Implementation of processArg().
 */
template<class T>
bool ValueArg<T>::processArg(int *i, vector<string>& args)
{
	if ( _ignoreable && Arg::ignoreRest() )
		return false;

	string flag = args[*i];

	string value = "";
	trimFlag( flag, value );

	if ( argMatches( flag ) )
	{
		if ( _alreadySet )
			throw( ArgException("Argument already set!", toString()) );

		if ( Arg::_delimiter != ' ' && value == "" )
			throw( ArgException( "Couldn't find delimiter for this argument!",
									 toString() ) );

		if ( value == "" )
		{
			(*i)++;
			if ( (unsigned int)*i < args.size() ) 
				_extractValue( args[*i] );
			else
				throw( ArgException("Missing a value for this argument!",
							 toString() ) );
		}
		else
			_extractValue( value );
				
		_alreadySet = true;
		_checkWithVisitor();
		return true;
	}	
	else
		return false;
}

/**
 * Implementation of _extractValue.
 */
template<class T>
void ValueArg<T>::_extractValue( const string& val ) 
{
	istringstream is(val);
	is >> _value;
	if ( is.fail() ) 
		throw( ArgException("Couldn't read argument value!", toString() ) );
}

/**
 * Implementation of shortID.
 */
template<class T>
string ValueArg<T>::shortID(const string& val) const
{
	return Arg::shortID( _typeDesc );	
}

/**
 * Implementation of longID.
 */
template<class T>
string ValueArg<T>::longID(const string& val) const
{
	return Arg::longID( _typeDesc );	
}

}
#endif