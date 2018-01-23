#ifndef GP__PARAMS_HPP_
#define GP__PARAMS_HPP_

#include "Utils/Map.hpp"
#include "Utils/Misc.hpp"
#include <string>
#include <cstdio>
#include <climits>

namespace GP
{
    class Params
    {
    public:
	Params(void) 
	{
	}
	
	virtual ~Params(void);
	

	virtual const char* GetValue(const char id[], const char notFound[] = NULL);

	virtual double GetValueAsDouble(const char id[], const double notFound = HUGE_VAL)
	{
	    const char *str = GetValue(id);
	    if(str == NULL)
		return notFound;
	    return StrToDouble(str);
	}

	virtual int GetValueAsInt(const char id[], const int notFound = INT_MAX)
	{
	    const char *str = GetValue(id);
	    if(str == NULL)
		return notFound;
	    return StrToInt(str);
	}
		
	virtual void SetValue(const char id[], const char val[]);
	
	
	virtual void Read(FILE * in);
	virtual void Print(FILE *out) const;
	
	static Params* GetSingleton(void)
	{
	    return m_singleton;
	}
	
    protected:
	static Params *m_singleton;
	
	UseMap(const std::string, const std::string) m_values;
    };
    
}

#endif



