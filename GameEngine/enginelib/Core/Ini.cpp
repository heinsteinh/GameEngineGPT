#include <Core/Ini.h>
#include <Utils/ExceptionDebug.h>
#include <Utils/StringUtils.h>
#include <ftspch.h>

namespace fts
{

    // Const are implicitly static in c++, but not in c.
    // So for consistancy, i added static anyway.
    static const char SECTION_SEP      = '.';
    static const char COMMENTS_SYMBOL0 = '#';
    static const char COMMENTS_SYMBOL1 = ';';

    inline bool IsCommentSymbol(char ch)
    {
        return ch == COMMENTS_SYMBOL0 || ch == COMMENTS_SYMBOL1;
    }

    // inline Exception GetException(size_t line_number, const std::string& msg) {
    //     return Exception(fmt::format("Line {}: {}", line_number, msg));
    // }

    void Ini::OutputStream(std::ostream& stream) const
    {
        std::string last_section;
        bool is_first_line = true;
        for(auto iter = m_values.begin(); iter != m_values.end(); iter++)
        {
            std::string key   = iter->first;
            std::string value = iter->second;
            str::Trim(key);
            str::Trim(value);

            std::size_t section_pos = key.find_first_of(SECTION_SEP);
            if(section_pos == std::string::npos)
                throw EXCEPTION("Key/value pair without section");
            std::string section = key.substr(0, section_pos);
            key                 = key.substr(section_pos + 1);

            if(section != last_section)
            {
                if(!is_first_line)
                    stream << '\n';

                stream << "[" << section << "]\n";
                last_section = section;

                is_first_line = false;
            }

            stream << key << " = " << value << '\n';
        }
        std::flush(stream);
    }

    void Ini::Load(const std::string& filename)
    {
        std::ifstream file;
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        try
        {
            file.open(filename);
        }
        catch(std::ifstream::failure& e)
        {
            throw EXCEPTION(filename);
        }
        file.exceptions(std::ifstream::badbit);
        Clear();
        ParseStream(file);
    }

    void Ini::Save(const std::string& filename) const
    {
        std::ofstream file;
        file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
        try
        {
            file.open(filename);
        }
        catch(std::ofstream::failure& e)
        {
            throw EXCEPTION(filename);
        }
        OutputStream(file);
    }

    void Ini::Clear()
    {
        m_values.clear();
        m_sections.clear();
    }

    void Ini::Set(const std::string& section, const std::string& name,
                  const std::string& value)
    {
        std::string key = MakeKey(section, name);
        m_values[key]   = value;
    }

    void Ini::SetInteger(const std::string& section, const std::string& name,
                         int value)
    {
        Set(section, name, std::to_string(value));
    }

    void Ini::SetReal(const std::string& section, const std::string& name,
                      double value)
    {
        Set(section, name, std::to_string(value));
    }

    void Ini::SetFloat(const std::string& section, const std::string& name,
                       float value)
    {
        Set(section, name, std::to_string(value));
    }

    void Ini::SetBoolean(const std::string& section, const std::string& name,
                         bool value)
    {
        Set(section, name, value ? "true" : "false");
    }

    std::string Ini::Get(const std::string& section, const std::string& name,
                         const std::string& default_value) const
    {
        std::string key = MakeKey(section, name);
        return m_values.count(key) ? m_values.at(key) : default_value;
    }

    int Ini::GetInteger(const std::string& section, const std::string& name,
                        int default_value) const
    {
        std::string val_str = Get(section, name, std::to_string(default_value));
        const char* value   = val_str.c_str();
        char* end;
        // This parses "1234" (decimal) and also "0x4D2" (hex)
        long n = strtol(value, &end, 0);
        return end > value ? n : default_value;
    }

    double Ini::GetReal(const std::string& section, const std::string& name,
                        double default_value) const
    {
        std::string val_str = Get(section, name, std::to_string(default_value));
        const char* value   = val_str.c_str();
        char* end;
        double n = strtod(value, &end);
        return end > value ? n : default_value;
    }

    float Ini::GetFloat(const std::string& section, const std::string& name,
                        float default_value) const
    {
        std::string val_str = Get(section, name, std::to_string(default_value));
        const char* value   = val_str.c_str();
        char* end;
        float n = strtof(value, &end);
        return end > value ? n : default_value;
    }

    bool Ini::GetBoolean(const std::string& section, const std::string& name,
                         bool default_value) const
    {
        std::string val_str = Get(section, name, std::to_string(default_value));
        // Convert to lower case to make string comparisons case-insensitive
        std::transform(val_str.begin(), val_str.end(), val_str.begin(), ::tolower);
        if(val_str == "true" || val_str == "yes" || val_str == "on" || val_str == "1")
            return true;
        else if(val_str == "false" || val_str == "no" || val_str == "off" || val_str == "0")
            return false;
        else
            return default_value;
    }

    void Ini::ParseStream(std::istream& stream)
    {
        int line_number = 0;
        std::string section;
        for(std::string line; std::getline(stream, line); ++line_number)
        {
            str::Trim(line);

            // skip empty line
            if(line.empty())
                continue;

            // skip comments
            if(IsCommentSymbol(line.front()))
                continue;

            if(line.front() == '[' && line.back() == ']')
            {
                section = line.substr(1, line.size() - 2);
                if(section.empty())
                    throw EXCEPTION(line_number, "no section");
                m_sections.emplace(section);
                continue;
            }
            size_t sep_pos = line.find_first_of('=');
            if(sep_pos == std::string::npos)
                throw EXCEPTION(line_number, "Invalid line");

            std::string name = line.substr(0, sep_pos);
            str::Trim(name);
            if(name.empty())
                throw EXCEPTION(line_number, "Empty key");

            std::string value = line.substr(sep_pos + 1);
            str::Trim(value);

            std::string key = MakeKey(section, name);
            if(m_values.count(key))
            {
                throw EXCEPTION(line_number, "Duplicate key");
            }
            m_values[key] = value;
        }
    }

    std::string Ini::MakeKey(const std::string& section, const std::string& name)
    {
        std::string key = section + SECTION_SEP + name;
        // Convert to lower case to make section/name lookups case-insensitive
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        return key;
    }

} // namespace fts
