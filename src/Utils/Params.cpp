#include "Utils/Params.hpp"

namespace GP
{
    
    Params* Params::m_singleton = new Params();

    Params::~Params(void)
    {
	m_values.clear();
    }
    
    const char* Params::GetValue(const char id[], const char notFound[]) 
    {
	auto iter = m_values.find(id);
	if(iter != m_values.end())
	    return iter->second.c_str();
	return notFound;
    }
    
    void Params::SetValue(const char id[], const char val[])
    {
	m_values[id] = val;
    }
    
    
    void Params::Print(FILE *out) const
    {
	for(auto &iter : m_values)
	    fprintf(out, "<%-30s> <%s>\n", iter.first.c_str(), iter.second.c_str());
    }
    
    void Params::Read(FILE *in)
    {
	char id[300];
	char val[300];
	while(fscanf(in, "%s %s", id, val) == 2)
	{
	    // printf("reading <%s> <%s>\n", id, val);
	    if(strcmp(id, "ParamsExtraFile") == 0)
	    {
		printf("...using ParamsExtraFile <%s>\n", val);
		
		FILE *fp = fopen(val, "r");
		Read(fp);
		fclose(fp);
	    }
	    else
		SetValue(id, val);
	}
    }
}



