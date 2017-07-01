#include <tclap/CmdLine.h>

namespace TCLAP {

    template <typename... Args>
    class ValueArg<std::tuple<Args...>>: public Arg {
    public:

        using value_type = std::tuple<Args...>;

        static constexpr size_t nargs = sizeof...(Args);

    protected:

        /**
         * The value parsed from the command line.
         * Can be of any type, as long as the >> operator for the type
         * is defined.
         */
        value_type _value;

        /**
         * Used to support the reset() method so that ValueArg can be
         * reset to their constructed value.
         */
        value_type _default;

        /**
         * A human readable description of the type to be parsed.
         * This is a hack, plain and simple.  Ideally we would use RTTI to
         * return the name of type T, but until there is some sort of
         * consistent support for human readable names, we are left to our
         * own devices.
         */
        std::string _typeDesc;

        /**
         * A Constraint this Arg must conform to.
         */
        Constraint<value_type>* _constraint;


        template <typename T>
        T _extractSingleValue(const std::string& strVal);

        template<size_t... I>
        value_type _extractAllValues(const std::array<std::string, nargs>& val,
                                     std::index_sequence<I...>);

        /**
         * Extracts the value from the string.
         * Attempts to parse string as type T, if this fails an exception
         * is thrown.
         * \param val - value to be parsed.
         */
        void _extractValue(const std::array<std::string, nargs>& val);

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
        ValueArg(const std::string& flag,
                 const std::string& name,
                 const std::string& desc,
                 bool req,
                 value_type value,
                 const std::string& typeDesc,
                 Visitor* v = NULL);

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
         * \param parser - A CmdLine parser object to add this Arg to
         * \param v - An optional visitor.  You probably should not
         * use this unless you have a very good reason.
         */
        ValueArg(const std::string& flag,
                 const std::string& name,
                 const std::string& desc,
                 bool req,
                 value_type value,
                 const std::string& typeDesc,
                 CmdLineInterface& parser,
                 Visitor* v = NULL);

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
         * \param constraint - A pointer to a Constraint object used
         * to constrain this Arg.
         * \param parser - A CmdLine parser object to add this Arg to.
         * \param v - An optional visitor.  You probably should not
         * use this unless you have a very good reason.
         */
        ValueArg(const std::string& flag,
                 const std::string& name,
                 const std::string& desc,
                 bool req,
                 value_type value,
                 Constraint<value_type>* constraint,
                 CmdLineInterface& parser,
                 Visitor* v = NULL);

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
         * \param constraint - A pointer to a Constraint object used
         * to constrain this Arg.
         * \param v - An optional visitor.  You probably should not
         * use this unless you have a very good reason.
         */
        ValueArg(const std::string& flag,
                 const std::string& name,
                 const std::string& desc,
                 bool req,
                 value_type value,
                 Constraint<value_type>* constraint,
                 Visitor* v = NULL);

        /**
         * Handles the processing of the argument.
         * This re-implements the Arg version of this method to set the
         * _value of the argument appropriately.  It knows the difference
         * between labeled and unlabeled.
         * \param i - Pointer the the current argument in the list.
         * \param args - Mutable list of strings. Passed
         * in from main().
         */
        virtual bool processArg(int* i, std::vector<std::string>& args);

        /**
         * Returns the value of the argument.
         */
        const value_type& getValue() const { return _value; }

        /**
         * A ValueArg can be used as as its value type (T) This is the
         * same as calling getValue()
         */
        operator const value_type&() const { return getValue(); }

        /**
         * Specialization of shortID.
         * \param val - value to be used.
         */
        virtual std::string shortID(const std::string& val = "val") const;

        /**
         * Specialization of longID.
         * \param val - value to be used.
         */
        virtual std::string longID(const std::string& val = "val") const;

        virtual void reset();

        ValueArg(const ValueArg&) = delete;
        ValueArg& operator=(const ValueArg&) = delete;

    };

    template<class... Args>
    ValueArg<std::tuple<Args...>>::ValueArg(const std::string& flag,
                                            const std::string& name,
                                            const std::string& desc,
                                            bool req,
                                            value_type val,
                                            const std::string& typeDesc,
                                            Visitor* v)
        : Arg(flag, name, desc, req, true, v),
          _value( val ),
          _default( val ),
          _typeDesc( typeDesc ),
          _constraint( NULL )
    { }

    template<class... Args>
    ValueArg<std::tuple<Args...>>::ValueArg(const std::string& flag,
                                            const std::string& name,
                                            const std::string& desc,
                                            bool req,
                                            value_type val,
                                            const std::string& typeDesc,
                                            CmdLineInterface& parser,
                                            Visitor* v)
        : Arg(flag, name, desc, req, true, v),
          _value( val ),
          _default( val ),
          _typeDesc( typeDesc ),
          _constraint( NULL )
    {
        parser.add( this );
    }

    template<class... Args>
    ValueArg<std::tuple<Args...>>::ValueArg(const std::string& flag,
                                            const std::string& name,
                                            const std::string& desc,
                                            bool req,
                                            value_type val,
                                            Constraint<value_type>* constraint,
                                            Visitor* v)
        : Arg(flag, name, desc, req, true, v),
          _value( val ),
          _default( val ),
          _typeDesc( Constraint<value_type>::shortID(constraint) ),
          _constraint( constraint )
    { }

    template<class... Args>
    ValueArg<std::tuple<Args...>>::ValueArg(const std::string& flag,
                                            const std::string& name,
                                            const std::string& desc,
                                            bool req,
                                            value_type val,
                                            Constraint<value_type>* constraint,
                                            CmdLineInterface& parser,
                                            Visitor* v)
        : Arg(flag, name, desc, req, true, v),
          _value( val ),
          _default( val ),
          _typeDesc( Constraint<value_type>::shortID(constraint) ),
          // TODO(macbishop): Will crash
          // if constraint is NULL
          _constraint( constraint )
    {
        parser.add( this );
    }

    template<class... Args>
    bool ValueArg<std::tuple<Args...>>::processArg(int *i, std::vector<std::string> &args) {

        if (this->_ignoreable && this->ignoreRest())
            return false;

        if (this->_hasBlanks(args[*i])) {
            return false;
        }

        std::string flag = args[*i];
        std::string value = "";
        this->trimFlag(flag, value);

        if (this->argMatches(flag)) {
            if (this->_alreadySet) {
                throw TCLAP::CmdLineParseException("Argument already set!", this->toString());
            }
            if (this->delimiter() != ' ' && value == "") {
                throw TCLAP::ArgParseException("Couldn't find delimiter for this argument!",
                                               this->toString());
            }

            if (value == "") {

                std::array<std::string, nargs> values;

                for (auto &value: values) {
                    (*i)++;
                    if (static_cast<size_t>(*i) >= args.size()) {
                        throw TCLAP::ArgParseException("Missing a value for this argument!",
                                                       this->toString());
                    }
                    value = args[*i];
                }

                _extractValue(values);

            }
            else {
                // TODO: What is here?
                throw TCLAP::ArgParseException("Missing a value for this argument!",
                                               this->toString());
            }

            return true;
        }

        return false;
    }

/**
 * Implementation of shortID.
 */
    template<class... Args>
    std::string ValueArg<std::tuple<Args...>>::shortID(const std::string&) const
    {
        return Arg::shortID( _typeDesc );
    }

/**
 * Implementation of longID.
 */
    template<class... Args>
    std::string ValueArg<std::tuple<Args...>>::longID(const std::string&) const
    {
        return Arg::longID( _typeDesc );
    }

    template <typename... Args>
    template <typename T>
    T ValueArg<std::tuple<Args...>>::_extractSingleValue(const std::string& strVal) {
        try {
            T value;
            ExtractValue(value, strVal, typename ArgTraits<T>::ValueCategory());
            return value;
        } catch( ArgParseException &e) {
            throw ArgParseException(e.error(), toString());
        }
    }

    template<class... Args>
    template<size_t... I>
    typename ValueArg<std::tuple<Args...>>::value_type
    ValueArg<std::tuple<Args...>>::_extractAllValues(
        const std::array<std::string, nargs>& values,
        std::index_sequence<I...>) {
        return std::make_tuple(
            this->_extractSingleValue<std::tuple_element_t<I, value_type>>(values[I])...);
    }


    template<class... Args>
    void ValueArg<std::tuple<Args...>>::_extractValue(
        std::array<std::string, nargs> const& values) {

        _value = _extractAllValues(values, std::index_sequence_for<Args...>{});

        if (this->_constraint != NULL) {
            if (!this->_constraint->check(this->_value)) {
                std::stringstream err;
                err << "Value ( ";
                for (auto &val: values) {
                    err << val << ' ';
                }
                err << ") does not meet constraint: "
                    << this->_constraint->description();
                throw TCLAP::CmdLineParseException(err.str(), this->toString());
            }
        }

    }


    template<class... Args>
    void ValueArg<std::tuple<Args...>>::reset()
    {
        Arg::reset();
        _value = _default;
    }

} 
