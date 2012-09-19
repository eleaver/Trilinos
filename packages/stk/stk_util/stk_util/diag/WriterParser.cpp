
#include <sstream>
#include <iomanip>
#include <map>

#include <stk_util/diag/WriterParser.hpp>
#include <stk_util/diag/Trace.hpp>
#include <stk_util/diag/Writer_fwd.hpp>
#include <iostream>

namespace stk {
namespace diag {

WriterParser::WriterParser()
  : OptionMaskParser()
{
  mask("coverage", 0, "Collect and display traceable function usage coverage");
  mask("members", LOG_MEMBERS, "Display data structure members messages");
  mask("trace", LOG_TRACE, "Display execution trace");
  mask("trace-stats", LOG_TRACE_STATS, "Display execution time and memory usage during trace");
  mask("trace-down", LOG_TRACE_SUB_CALLS, "Display subsequent calls after tracing is enabled");
}


OptionMaskParser::Mask
WriterParser::parse(
  const char *          mask_string) const
{
  m_optionMask = LOG_MEMBERS;
  return OptionMaskParser::parse(mask_string);
}


void
WriterParser::parseArg(
  const std::string &  name,
  const std::string &  arg) const
{
  if (name == "trace") {
    m_optionMask |= LOG_TRACE;
    if (!arg.empty()) {
      std::string::const_iterator it0 = arg.begin();
      std::string::const_iterator it1;
      std::string::const_iterator it2;
      do {
        // Trim preceeding spaces
        while (it0 != arg.end() && *it0 == ' ')
          it0++;

        if (it0 == arg.end())
          break;

        int paren_count = 0;
        for (it1 = it0; it1 != arg.end(); ++it1) {
          if (*it1 == '(')
            ++paren_count;
          else if (*it1 == ')')
            --paren_count;
          else if (*it1 == ',' && paren_count == 0)
            break;
        }


        // Trim trailing spaces
        it2 = it1;
        while (it2 != it0 && *(it2 - 1) == ' ')
          --it2;

        std::string function(it0, it2);

        Trace::addTraceFunction(function);

        it0 = it1 + 1;
      } while (it1 != arg.end());
    }
    else
      m_optionMask |= LOG_TRACE_SUB_CALLS;
  }

  else {
    OptionMaskNameMap::const_iterator mask_entry = m_optionMaskNameMap.find(name.c_str());

    if (mask_entry != m_optionMaskNameMap.end())
      m_optionMask |= (*mask_entry).second.m_mask;
    else {
      Mask  mask_hex = 0;
      std::istringstream mask_hex_stream(name.c_str());
      if (mask_hex_stream >> std::resetiosflags(std::ios::basefield) >> mask_hex)
        m_optionMask |= mask_hex;
      else
        m_status = false;
    }
  }
}


OptionMaskParser::Mask
OptionMaskParser::parse(
  const char *          mask) const
{
  if (mask) {
    const std::string mask_string(mask);

    m_status = true;

    std::string::const_iterator it0 = mask_string.begin();
    std::string::const_iterator it1;
    std::string::const_iterator it2;
    std::string::const_iterator it3;
    do {
      // Trim preceeding spaces
      while (it0 != mask_string.end() && *it0 == ' ')
        it0++;

      if (it0 == mask_string.end())
        break;

      for (it1 = it0; it1 != mask_string.end(); ++it1) {
        if (*it1 == '(' || *it1 == ':' || *it1 == ',')
          break;
      }

      // Trim trailing spaces
      it2 = it1;
      while (it2 != it0 && *(it2 - 1) == ' ')
        --it2;

      std::string name(it0, it2);

      // Get argument list
      if (*it1 == '(') {
        it2 = it1 + 1;

        // Trim preceeding spaces
        while (it2 != mask_string.end() && *it2 == ' ')
          ++it2;

        int paren_count = 0;

        for (; it1 != mask_string.end(); ++it1) {
          if (*it1 == '(')
            ++paren_count;
          else if (*it1 == ')') {
            --paren_count;
            if (paren_count == 0)
              break;
          }
        }
        it3 = it1;

        // Trim trailing spaces
        while (it3 != it2 && *(it3 - 1) == ' ')
          --it3;

        // Find next argument start
        for (; it1 != mask_string.end(); ++it1)
          if (*it1 == ':' || *it1 == ',')
            break;
      }
      else
        it2 = it3 = it1;

      const std::string arg(it2, it3);

      parseArg(name, arg);

      it0 = it1 + 1;
    } while (it1 != mask_string.end());
  }

  return m_optionMask;
}


void
OptionMaskParser::parseArg(
  const std::string &  name,
  const std::string &  arg) const
{
  OptionMaskNameMap::const_iterator mask_entry = m_optionMaskNameMap.find(name);

  if (mask_entry != m_optionMaskNameMap.end()) m_optionMask |= (*mask_entry).second.m_mask;
  else {
    Mask  mask_hex = 0;
    std::istringstream mask_hex_stream(name.c_str());
    if (mask_hex_stream >> std::resetiosflags(std::ios::basefield) >> mask_hex)
      m_optionMask |= mask_hex;
    else
      m_status = false;
  }
}


std::ostream &
OptionMaskParser::describe(
  std::ostream &    os) const
{
  os << "Specify a comma separated list of:" << std::endl;
  for (OptionMaskNameMap::const_iterator it = m_optionMaskNameMap.begin(); it != m_optionMaskNameMap.end(); ++it)
    (*it).second.describe(os);

  return os;
}


std::ostream &
OptionMaskName::describe(
  std::ostream &  os) const
{
  return os << "  " << std::left << std::setw(20) << m_name << "\t" << m_description << std::endl;
}

} // namespace diag
} // namespace stk
